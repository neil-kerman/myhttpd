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
        return this->status == 2;
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
                if (this->match.match((new_data.base)[i])) {
                    /* Reset the detecter status */
                    this->match.reset();
                    std::string header;
                    /* Calculate the header size */
                    std::size_t header_size = i + 1;
                    /* Copy header */
                    if (this->_header_receive_buffer.size() != 0) {
                        /* Copy the data which has kept in the buffer */
                        header_size += this->_header_receive_buffer.size();
                        header.reserve(header_size);
                        auto data_blocks = this->_header_receive_buffer.get_data();
                        for (auto& block : data_blocks) {
                            header.append(block.base, block.size);
                        }
                        this->_header_receive_buffer.consume(this->_header_receive_buffer.size());
                    }
                    /* Copy the data which in the current buffer block */
                    header.append(new_data.base, i + 1);
                    this->_header_receive_buffer.commit(bytes_transferred);
                    this->_header_receive_buffer.consume(i + 1);

                    auto msg = std::make_unique<message>(std::move(header));
                    auto cl_attr = msg->find_attribute("content-length");
                    /* Check if there are content of message */
                    if (cl_attr != msg->end_attribute()) {
                        auto content_size_s = std::string(cl_attr->second.data, cl_attr->second.size);
                        auto content_size = (std::size_t)std::stoll(content_size_s);
                        if (content_size != 0) {
                            auto buf = std::make_shared<std::vector<char>>();
                            buf->reserve(content_size);
                            auto content = std::make_shared<transmitting_content>(buf);
                            if (content_size <= bytes_transferred - (i + 1)) {
                                
                            } else {

                            }
                        } else {
                            this->_receive_handler(error, std::move(msg));
                            return;
                        }
                    } else {
                        this->_receive_handler(error, std::move(msg));
                        return;
                    }
                }
            }
            /* Continue to receive */
            this->_header_receive_buffer.commit(bytes_transferred);
            
            auto next_buffer = this->_header_receive_buffer.get_available_block();
            this->_conn->async_read_some(
                myhttpd::network::connection::mutable_buffer(next_buffer.base, next_buffer.size),
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

    void transceiver::async_receive(receive_handler handler) {
        this->_receive_handler = handler;
        this->match.reset();
        
    }

    void transceiver::async_send(std::shared_ptr<message> msg, send_handler handler) {
        
        
    }

    void transceiver::async_wait(socket_wait_type type, wait_handler handler) {
        this->_conn->async_wait(type, handler);
    }

    void transceiver::cancel() {
        this->_conn->cancel();
    }

    transceiver::transceiver(std::unique_ptr<myhttpd::network::connection> conn) 
    : _conn(std::move(conn)) {}
}

