#include "transceiver.hpp"
#include <string>
#include <functional>
#include <vector>

#include "transceiver.hpp"
#include "transmitting_content.hpp"

namespace myhttpd::http {

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

                    auto msg = std::make_unique<message>(std::move(header));
                    auto content_length = this->_get_content_length(msg);
                    /* Check if there are content of message */
                    if (content_length != 0) {
                        auto buf = std::make_shared<std::vector<char>>();
                        buf->reserve(content_length);
                        auto content = std::make_shared<transmitting_content>(buf);
                        if (content_length <= this->_header_receive_buffer.size()) {

                        } else {

                        }
                    } else {
                        this->_receive_handler(error, std::move(msg));
                    }
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

    std::string transceiver::_to_string_header(const std::shared_ptr<message>& msg) {
        auto title = msg->get_title();
        std::size_t size = title.size() + 4;
        for (auto attr = msg->begin_attribute(); attr != msg->end_attribute(); attr++) {
            size += attr->first.size() + attr->second.size() + 4;
        }
        std::string buf;
        buf.reserve(size);
        buf.append(title).append("\r\n");
        for (auto attr = msg->begin_attribute(); attr != msg->end_attribute(); attr++) {
            buf.append(attr->first).append(": ").append(attr->second).append("\r\n");
        }
        buf.append("\r\n");
        return buf;
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
                        auto msg = std::make_unique<message>(std::move(header));
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

    void transceiver::async_send(std::shared_ptr<message> msg, send_handler handler) {
        if (msg->find_attribute("content-length") == msg->end_attribute() && msg->has_content()) {
            msg->insert_attribute("content-length", std::to_string(msg->get_content()->get_size()));
        }
        auto header = this->_to_string_header(msg);
        this->_conn->async_send({header.data(), header.size()},
            [msg, &conn = this->_conn, handler, header](const asio_error_code& error, std::size_t bytes_transferred) {
                if (!error) {
                    if (msg->has_content()) {
                        msg->get_content()->async_wait_ready(
                            [msg, &conn, handler](const asio_error_code& error, network::connection::const_buffer buf) {
                                conn->async_send(buf,
                                    [msg, handler](const asio_error_code& error, std::size_t bytes_transferred) {
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

    transceiver::transceiver(std::unique_ptr<myhttpd::network::connection> &conn) 
    : _conn(conn) {}
}

