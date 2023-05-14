#include <string>
#include <functional>
#include <vector>
#include <boost/bind.hpp>

#include "transceiver.hpp"
#include "transmitting_content.hpp"

namespace myhttpd::service::http {

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

    std::vector<char> transceiver::_encode_header(message& msg) {

        auto title = msg.get_title();
        std::size_t size = title.size() + 4;

        for (auto attr = msg.begin_attribute(); attr != msg.end_attribute(); attr++) {

            size += (attr->first.size() + attr->second.size() + 4);
        }

        std::vector<char> buf;
        buf.resize(size);
        char* ptr = buf.data();
        std::memcpy(ptr, title.data(), title.size());
        ptr += title.size();
        ptr[0] = '\r';
        ptr[1] = '\n';
        ptr += 2;

        for (auto attr = msg.begin_attribute(); attr != msg.end_attribute(); attr++) {

            std::string name = header_key_formalize(attr->first);
            auto& value = attr->second;
            std::memcpy(ptr, name.data(), name.size());
            ptr += name.size();
            ptr[0] = ':';
            ptr[1] = ' ';
            ptr += 2;
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

    std::unique_ptr<message> transceiver::_decode_header(const std::vector<char>& buf) {
        
        std::string_view view(buf.data(), buf.size());
        auto msg = std::make_unique<message>();
        auto title_size = view.find('\r', 0);
        msg->set_title(std::string(view.substr(0, title_size)));
        auto offset = title_size + 2;

        while (view[offset] != '\r') {

            auto name_size = view.find(':', offset) - offset;
            auto name = view.substr(offset, name_size);
            offset += name_size + 1;
            offset = view.find_first_not_of(' ', offset);
            auto value_size = view.find('\r', offset) - offset;
            std::string value(view.data() + offset, value_size);
            msg->insert_attribute((std::string)name, std::move(value));
            offset += value_size + 2;
        }

        return msg;
    }

    void transceiver::_header_receiving_handler(
        header_matcher matcher, receiving_handler rcv_handler,
        const asio_error_code error, std::size_t bytes_transferred) {

        this->_receiving_busy = false;

        if (!error) {

            /*
             * The variable "i" contains the match pointer on this buffer block.
             * If the whole buffer are scaned and no message has fully received,
             * this function will call the next async receive.
             */
            auto new_data = this->_header_receive_buffer.get_available_block();

            for (std::size_t i = 0; i < bytes_transferred; i++) {

                /* If the "\r\n\r\n" is detected */
                if (matcher.match(((char*)new_data.data)[i])) {

                    /* Calculate the header size */
                    std::size_t header_size = i + 1 + this->_header_receive_buffer.size();
                    this->_header_receive_buffer.commit(bytes_transferred);
                    /* Copy header */
                    std::vector<char> header_buf;
                    header_buf.resize(header_size);
                    this->_header_receive_buffer.take_data(header_buf.data(), header_size);
                    this->_header_receive_buffer.consume(header_size);
                    /* Decode header */
                    auto msg = this->_decode_header(header_buf);
                    /* Receive content */
                    this->_receive_content(*msg);
                    /* Invoke callback */
                    rcv_handler(transceiver_error_code::sucess, std::move(msg));
                    return;
                }
            }

            /* Continue to receive */
            this->_header_receive_buffer.commit(bytes_transferred);
            auto next_buffer = this->_header_receive_buffer.get_available_block();
            this->_receiving_busy = true;
            this->_conn.async_read_some(
                myhttpd::network::connection::mutable_buffer(next_buffer.data, next_buffer.size),
                std::bind(
                    &transceiver::_header_receiving_handler, this, matcher, rcv_handler,
                    std::placeholders::_1, std::placeholders::_2
                )
            );

        } else {

            /* Invoke callback with error */
            rcv_handler(transceiver_error_code::error, nullptr);
        }
    }

    void transceiver::_content_receiving_handler(std::shared_ptr<transmitting_content> content, 
        const asio_error_code error, std::size_t bytes_transferred) {

        this->_receiving_busy = false;

        if (this->_canceled) {

            if (!this->is_busy()) {

                this->_cancellation_handler();
            }

            return;
        }

        if (!error) {

            if (content->commit(bytes_transferred)) {

                this->_receiving_busy = true;
                this->_conn.async_receive(content->get_buffer(), 
                    std::bind(
                        &transceiver::_content_receiving_handler, 
                        this, content, std::placeholders::_1, std::placeholders::_2
                    )
                );
            }

        } else {

            content->cancel();
        }
    }

    void transceiver::_content_sending_handler(sending_handler handler, std::size_t remaining, 
        std::shared_ptr<content> content, const asio_error_code error, std::size_t bytes_transferred) {

        this->_sending_busy = false;

        if (this->_canceled) {

            if (!this->is_busy()) {

                this->_cancellation_handler();
            }

            return;
        }

        if (!error) {

            remaining -= bytes_transferred;

            if (remaining) {

                this->_sending_busy = true;
                content->async_read(

                    [this, handler, remaining, content]
                    (const content::content_error_code error, network::connection::const_buffer buf) {

                        if (!error) {

                            this->_sending_busy = false;

                            if (this->_canceled) {

                                if (!this->is_busy()) {

                                    this->_cancellation_handler();
                                }

                                return;
                            }

                            this->_sending_busy = true;
                            this->_conn.async_send(
                                buf,
                                std::bind(&transceiver::_content_sending_handler,
                                    this, handler, remaining, content,
                                    std::placeholders::_1, std::placeholders::_2
                                )
                            );
                        }
                    }, 

                    content->get_content_langth() - remaining
                );

            } else {

                handler(transceiver_error_code::sucess);
            }

        } else {

            handler(transceiver_error_code::error);
        }
    }

    void transceiver::async_receive(receiving_handler handler) {

        this->_receiving_busy = true;
        header_matcher matcher;

        if (this->_header_receive_buffer.size() != 0) {

            /* Firstly, trying to detect header from buffer */
            auto blks = this->_header_receive_buffer.get_data_blocks();
            std::size_t match_ptr = 0;
            bool loop_flag = true;
            auto blk_it = blks.begin();

            if (blk_it != blks.end()) {

                for (std::size_t i = 0; i < blk_it->size && loop_flag; i++) {

                    if (matcher.match(((char*)blk_it->data)[i])) {

                        loop_flag = false;
                        auto header_size = match_ptr + 1;
                        std::vector<char> header_buf;
                        header_buf.resize(header_size);
                        /* Copy header from transceiver buffer */
                        this->_header_receive_buffer.take_data(header_buf.data(), header_size);
                        this->_header_receive_buffer.consume(header_size);
                        /* Decode header  */
                        auto msg  = this->_decode_header(header_buf);
                        /* Receive content */
                        this->_receive_content(*msg);
                        /* Invoke callback */
                        handler(transceiver_error_code::sucess, std::move(msg));
                        return;
                    }

                    match_ptr++;
                }
            }
        }

        /* No full header has detected, receive more data from connection */
        this->_receiving_busy = true;
        auto next_buffer = this->_header_receive_buffer.get_available_block();
        this->_conn.async_read_some(
            myhttpd::network::connection::mutable_buffer(next_buffer.data, next_buffer.size),
            std::bind(
                &transceiver::_header_receiving_handler, this, matcher, handler,
                std::placeholders::_1, std::placeholders::_2
            )
        );
    }

    void transceiver::async_send(std::unique_ptr<message> msg, sending_handler handler) {


        this->_sending_busy = true;
        std::shared_ptr<message> msg_ptr(msg.release());
        auto encoded_header = std::make_shared<std::vector<char>>(this->_encode_header(*msg_ptr));
        this->_conn.async_send({ encoded_header->data(), encoded_header->size()},
            [this, handler, encoded_header, msg_ptr]

            (const asio_error_code& error, std::size_t bytes_transferred) {

                this->_sending_busy = false;

                if (this->_canceled) {

                    if (!this->is_busy()) {

                        this->_cancellation_handler();
                    }

                    return;
                }

                if (!error) {

                    if (msg_ptr->has_content()) {

                        this->_send_content(msg_ptr->get_content(), handler);

                    } else {

                        handler(transceiver_error_code::error);
                    }

                } else {

                    handler(transceiver_error_code::error);
                }
            }
        );
    }

    void transceiver::_receive_content(message& msg) {

        auto content_length = msg.get_content_length();

        /* Check if there are content of message */
        if (content_length != 0) {

            /* Create buffer for content data */
            auto content = std::make_shared<transmitting_content>(content_length);
            msg.set_content(content);

            /* Calculate how many data available in transceiver buffer */
            const auto bufed_data_size = this->_header_receive_buffer.size();
            const auto size_to_take = (bufed_data_size < content_length) ? bufed_data_size : content_length;
            const auto& offset = size_to_take;

            /* Copy from transceiver buffer to content buffer */
            if (size_to_take != 0) {

                this->_header_receive_buffer.take_data(content->get_buffer().data, size_to_take);
                this->_header_receive_buffer.consume(size_to_take);
                content->commit(size_to_take);
            }

            auto size_to_receive = content_length - size_to_take;
            /* Receive rest of data from the connection */
            this->_sending_busy = true;
            asio_error_code code;
            this->_content_receiving_handler(content, code, 0);
        }
    }

    void transceiver::_send_content(std::shared_ptr<content> content, sending_handler handler) {

        auto remaining = content->get_content_langth();
        this->_sending_busy = true;
        asio_error_code code;
        this->_content_sending_handler(handler, remaining, content, code, 0);

    }

    void transceiver::async_wait(socket_wait_type type, waiting_handler handler) {


        this->_waiting_busy = true;
        this->_conn.async_wait(type, 
            
            [this, handler](const asio_error_code error) {

                this->_waiting_busy = false;

                if (this->_canceled) {

                    if (!this->is_busy()) {

                        this->_cancellation_handler();
                    }

                    return;
                }
                
                if (!error) {

                    handler(transceiver_error_code::sucess);

                } else {

                    handler(transceiver_error_code::error);
                }
            }
        );

        return;
    }

    void transceiver::cancel(cancellation_handler handler) {

        if (!this->is_busy()) {

            handler();

        } else {
            
            this->_cancellation_handler = handler;
            this->_canceled = true;
            this->_conn.cancel();
        }

        return;
    }

    bool myhttpd::service::http::transceiver::is_busy() {

        return this->_receiving_busy || this->_sending_busy || this->_waiting_busy;
    }

    transceiver::transceiver(myhttpd::network::connection &conn)
    :_conn(conn) {

        return;
    }

    bool transceiver::header_matcher::match(const char value) {

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

    void transceiver::header_matcher::reset() {

        this->status = 0;
        return;
    }
}
