#include "transceiver.hpp"
#include "transceiver.hpp"
#include <string>
#include <functional>
#include <vector>

#include "transceiver.hpp"
#include "transmitting_content.hpp"

namespace myhttpd::session::http {

    bool transceiver::header_detecter::match(const char value) {

        switch (this->status) {

        case 0:
            if (value == '\r') {

                this->status++;
            }

            break;

        case 1:
            if (value == '\n') {

                this->status++;

            } else if (value == '\r') {

                this->status = 1;

            } else {

                this->status = 0;
            }

            break;

        case 2:
            if (value == '\r') {

                this->status++;

            } else if (value == '\r') {

                this->status = 1;

            } else {

                this->status = 0;
            }

            break;

        case 3:
            if (value == '\n') {

                this->status++;

            } else if (value == '\r') {

                this->status = 1;

            } else {

                this->status = 0;
            }

            break;

        default:
            break;
        }

        return this->status == 4;
    }

    void transceiver::header_detecter::reset() {

        this->status = 0;
    }

    /* This procedure allows receive more than one http message at onec by a linked buffer */
    void transceiver::_header_receive_handler(const asio_error_code &error, std::size_t bytes_transferred) {

        if (!error) {

            /*
             * The variable "i" contains the match pointer on this buffer block.
             * If the whole buffer are scaned and no message has fully received,
             * this function will call the next async receive.
             */
            auto new_data = this->_header_receive_buffer.get_available_block();

            for (std::size_t i = 0; i < bytes_transferred; i++) {

                /* If the "\r\n\r\n" is detected */
                if (this->_match.match(((char*)new_data.data)[i])) {

                    /* Calculate the header size */
                    std::size_t header_size = i + 1 + this->_header_receive_buffer.size();
                    /* Copy header */
                    this->_header_receive_buffer.commit(bytes_transferred);
                    std::string header = this->_take_header(header_size);
                    auto msg = this->_decode_header(header);
                    auto content_length = this->_get_content_length(msg);
                    /* Check if there are content of message */

                    if (content_length != 0) {

                        auto buf = std::make_shared<std::vector<char>>();
                        buf->reserve(content_length);
                        auto content = std::make_shared<transmitting_content>(buf);
                        msg->set_content(content);

                        if (this->_header_receive_buffer.size() != 0) {

                            auto bufed_data_size = this->_header_receive_buffer.size();
                            auto size_to_fetch = (bufed_data_size < content_length) ? bufed_data_size : content_length;
                            auto blks = this->_header_receive_buffer.get_data(size_to_fetch);
                            std::size_t offset = 0;

                            for (auto &blk : blks) {

                                std::memcpy(buf->data() + offset, blk.data, blk.size);
                                offset += blk.size;
                            }

                            content_length -= size_to_fetch;
                            this->_header_receive_buffer.consume(size_to_fetch);

                            if (content_length != 0) {

                                this->_conn->async_receive({buf->data() + offset, content_length}, 
                                    [content](const asio_error_code& error, std::size_t bytes_transferred) {
                                        content->deliver(error);
                                    }
                                );

                            } else {

                                asio_error_code error_;
                                content->deliver(error_);
                            }
                        }
                    }

                    this->_receive_handler(error, std::move(msg));
                    return;
                }
            }

            /* Continue to receive */
            this->_header_receive_buffer.commit(bytes_transferred);
            auto next_buffer = this->_header_receive_buffer.get_available_block();
            this->_conn->async_read_some(
                myhttpd::network::connection::mutable_buffer(next_buffer.data, next_buffer.size),
                std::bind(
                    &transceiver::_header_receive_handler, this, 
                    std::placeholders::_1, std::placeholders::_2
                )
            );
            return;

        } else {

            this->_receive_handler(error, NULL);
        }
    }

    const std::string transceiver::_take_header(const std::size_t size) {

        std::string header;
        header.reserve(size);
        auto buf_blocks = this->_header_receive_buffer.get_data(size);
        std::size_t remainder_size = size;

        for (auto& blk : buf_blocks) {

            if (blk.size < remainder_size) {

                header.append((char*)blk.data, blk.size);
                remainder_size -= blk.size;

            } else {

                header.append((char*)blk.data, remainder_size);
                break;
            }
        }

        this->_header_receive_buffer.consume(size);
        return header;
    }

    std::size_t transceiver::_get_content_length(const std::unique_ptr<message> &msg) {

        auto cl_attr = msg->find_attribute("content-length");

        if (cl_attr != msg->end_attribute()) {

            return (std::size_t)std::stoll(cl_attr->second);

        } else {

            return 0;
        }
    }

    std::string header_key_formalize(const std::string &key) {

        auto result = key;
        result[0] -= 32;
        auto next = key.find('-');

        while (next != std::string::npos && next < key.size() + 1) {

            result[next + 1] -= 32;
            next = key.find('-', next + 1);
        }

        return result;
    }

    std::shared_ptr<std::vector<char>> transceiver::_encode_header(message &msg) {

        auto title = msg.get_title();
        std::size_t size = title.size() + 4;

        for (auto attr = msg.begin_attribute(); attr != msg.end_attribute(); attr++) {

            size += attr->first.size() + attr->second.size() + 4;
        }

        auto buf = std::make_shared<std::vector<char>>();
        buf->resize(size);
        char* ptr = buf->data();
        std::memcpy(ptr, title.data(), title.size());
        ptr += title.size();
        ptr[0] = '\r';
        ptr[1] = '\n';
        ptr += 2;

        for (auto attr = msg.begin_attribute(); attr != msg.end_attribute(); attr++) {

            std::string name = header_key_formalize(attr->first);
            std::string& value = attr->second;
            std::memcpy(ptr, name.data(), name.size());
            ptr += name.size();
            ptr[0] = ':';
            ptr += 1;
            std::memcpy(ptr, value.data(), value.size());
            ptr += value.size();
            ptr[0] = '\r';
            ptr[1] = '\n';
            ptr += 2;
        }

        ptr[0] = '\r';
        ptr[1] = '\n';
        return buf;
    }

    std::unique_ptr<message> transceiver::_decode_header(std::string &buf) {
        
        auto msg = std::make_unique<message>();
        auto title_size = buf.find('\r', 0);
        msg->set_title(std::string(buf.substr(0, title_size)));
        auto offset = title_size + 2;

        while (buf[offset] != '\r') {

            auto name_size = buf.find(':', offset) - offset;
            std::string name = buf.substr(offset, name_size);
            offset += name_size + 1;
            offset = buf.find_first_not_of(' ', offset);
            auto value_size = buf.find('\r', offset) - offset;
            std::string value(buf.data() + offset, value_size);
            msg->insert_attribute(std::move(name), std::move(value));
            offset += value_size + 2;
        }

        return msg;
    }

    void transceiver::async_receive(receive_handler handler) {

        this->_receive_handler = handler;
        this->_match.reset();

        if (this->_header_receive_buffer.size() != 0) {

            /* Tying to find header from buffer */
            auto blks = this->_header_receive_buffer.get_data();
            std::size_t match_ptr = 0;
            bool loop_flag = true;

            for (auto blk_it = blks.begin(); blk_it != blks.end() && loop_flag; blk_it++) {

                for (std::size_t i = 0; i < blk_it->size && loop_flag; i++) {

                    if (this->_match.match(((char*)blk_it->data)[i])) {

                        loop_flag = false;
                        std::string header = this->_take_header(match_ptr + 1);
                        auto msg = this->_decode_header(header);
                        auto content_length = _get_content_length(msg);

                        if (content_length != 0) {
                            
                        } else {

                            asio_error_code error;
                            handler(error, std::move(msg));
                        }
                    }

                    match_ptr++;
                }
            }
        }

        auto next_buffer = this->_header_receive_buffer.get_available_block();
        this->_conn->async_read_some(
            myhttpd::network::connection::mutable_buffer(next_buffer.data, next_buffer.size),
            std::bind(
                &transceiver::_header_receive_handler, this,
                std::placeholders::_1, std::placeholders::_2
            )
        );
    }

    void transceiver::async_send(std::unique_ptr<message> msg, send_handler handler) {

        this->_sending_msg.release();
        this->_sending_msg = std::move(msg);
        auto encoded_header = this->_encode_header(*(this->_sending_msg));
        
        this->_conn->async_send({ encoded_header->data(), encoded_header->size()},
            [this, handler, encoded_header]

            (const asio_error_code& error, std::size_t bytes_transferred) {

                if (!error) {

                    if (this->_sending_msg->has_content()) {

                        this->_sending_msg->get_content()->async_wait_ready(

                            [this, handler](const asio_error_code& error, network::connection::const_buffer buf) {

                                this->_conn->async_send(buf,

                                    [this, handler](const asio_error_code& error, std::size_t bytes_transferred) {

                                        this->_sending_msg.release();
                                        handler(error);
                                    }
                                );
                            }
                        );

                    } else {

                        handler(error);
                    }

                } else {

                    handler(error);
                }
            }
        );
    }

    void transceiver::async_wait(socket_wait_type type, wait_handler handler) {

        this->_conn->async_wait(type, handler);
    }

    void transceiver::cancel() {

        this->_conn->cancel();
    }

    transceiver::transceiver(std::unique_ptr<myhttpd::network::connection> &conn): 
        _conn(conn) {

    }
}

