#include <string>
#include <functional>
#include <strstream>

#include "transceiver_1_1.hpp"

namespace myhttpd::http {

    bool transceiver_1_1::line_detecter::match(const char value) {
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
        default:
            break;
        }
        return this->status == 2;
    }

    void transceiver_1_1::line_detecter::reset() {
        this->status = 0;
    }

    void transceiver_1_1::_header_receive_handler(connection::error_code code, std::size_t bytes_transferred) {

        if (!code) {
            std::size_t offset = 0;
            auto new_data = this->_header_receive_buffer.get_available_block();
            for (std::size_t i = 0; i < bytes_transferred; i++) {
                if (this->match.match((new_data.base)[i])) {
                    this->match.reset();
                    std::string line;
                    std::size_t line_size = i - offset + 1;
                    if (this->_header_receive_buffer.size() != 0) {
                        line_size += this->_header_receive_buffer.size();
                        line.reserve(line_size);
                        auto data_blocks = this->_header_receive_buffer.get_data();
                        for (auto& block : data_blocks) {
                            line.append(block.base, block.size);
                        }
                        this->_header_receive_buffer.consume(this->_header_receive_buffer.size());
                        line.append(new_data.base + offset, i - offset + 1);
                    } else {
                        line.append(new_data.base + offset, i - offset + 1);
                    }
                    offset = i + 1;
                    if (this->_receiving_message == NULL) {
                        this->_receiving_message.reset(new message());
                    }
                    if (line != "\r\n") {
                        this->_receiving_message->header.push_back(std::move(line));
                    } else {
                        this->_receiving_message->header.push_back(std::move(line));
                        this->_receive_handler(success, std::move(this->_receiving_message));
                        this->_header_receive_buffer.commit(bytes_transferred);
                        this->_header_receive_buffer.consume(offset);
                        return;
                    }
                }
            }

            this->_header_receive_buffer.commit(bytes_transferred);
            this->_header_receive_buffer.consume(offset);
            
            auto next_buffer = this->_header_receive_buffer.get_available_block();
            this->_conn->async_read_some(
                next_buffer.base, next_buffer.size,
                std::bind(
                    &transceiver_1_1::_header_receive_handler, this, 
                    std::placeholders::_1, std::placeholders::_2
                )
            );
            return;
        } else if (code == connection::canceled) {
            this->_receive_handler(canceled, NULL);
        } else {
            this->_receive_handler(error, NULL);
        }
    }

    void transceiver_1_1::_content_receive_handler(connection::error_code code, std::size_t bytes_transferred) {
        
    }

    void transceiver_1_1::async_receive(receive_handler handler) {
        this->_receive_handler = handler;
        auto next_buffer = this->_header_receive_buffer.get_available_block();
        this->_conn->async_read_some(
            next_buffer.base, next_buffer.size,
            std::bind(
                &transceiver_1_1::_header_receive_handler, this,
                std::placeholders::_1, std::placeholders::_2
            )
        );
    }

    void transceiver_1_1::async_send(std::unique_ptr<message> msg, send_handler handler) {
        this->_send_handler = handler;
        std::size_t header_size = 0;
        for (auto& line : msg->header) {
            header_size += line.size();
        }
        this->_header_sending_buffer.reserve(header_size);
        std::ostrstream stream(this->_header_sending_buffer.data(), header_size);
        for (auto& line : msg->header) {
            stream << line;
        }
        this->_conn->async_write_some(this->_header_sending_buffer.data(), header_size, 
            [handler = this->_send_handler](connection::error_code code, std::size_t bytes_transferred) {
                if (!code) {
                    handler(success);
                } else if (code == connection::canceled) {
                    handler(canceled);
                } else {
                    handler(error);
                }
            }
        );
    }

    void transceiver_1_1::async_wait(wait_type type, wait_handler handler) {
    }

    void transceiver_1_1::cancel() {
    }

    transceiver_1_1::transceiver_1_1(std::unique_ptr<connection> &conn) 
    : _conn(conn) {}

    transceiver_1_1::~transceiver_1_1() {
        
    }

}

