#include <string>
#include <functional>
#include <strstream>

#include "http_transceiver_1_1.hpp"

namespace myhttpd {

    bool http_transceiver_1_1::line_detecter::match(const char value) {
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

    void http_transceiver_1_1::line_detecter::reset() {
        this->status = 0;
    }

    void http_transceiver_1_1::_header_receive_handler(connection::error_code code, std::size_t bytes_transferred) {
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
                        line.append(new_data.base + offset, i + offset + 1);
                    } else {
                        line.append(new_data.base + offset, i + offset + 1);
                    }
                    offset = i;
                    if (this->_receiving_message == NULL) {
                        this->_receiving_message.reset(new http_message());
                    }
                    if (line != "\r\n") {
                        this->_receiving_message->header.push_back(std::move(line));
                    } else {
                        this->_receiving_message->header.push_back(std::move(line));
                        this->_receive_handler(success, std::move(this->_receiving_message));
                        this->_header_receive_buffer.commit(bytes_transferred);
                        this->_header_receive_buffer.consume(offset + 1);
                        return;
                    }
                }
            }

            this->_header_receive_buffer.commit(bytes_transferred);
            this->_header_receive_buffer.consume(offset + 1);
            
            auto next_buffer = this->_header_receive_buffer.get_available_block();
            this->_conn->async_read_some(
                next_buffer.base, next_buffer.size,
                std::bind(
                    &http_transceiver_1_1::_header_receive_handler, this, 
                    std::placeholders::_1, std::placeholders::_2
                )
            );
            return;
        } else {
            
        }
    }

    

    void http_transceiver_1_1::_content_receive_handler(connection::error_code code, std::size_t bytes_transferred) {
        
    }

    void http_transceiver_1_1::_header_send_handler(connection::error_code code, std::size_t bytes_transferred) {
        
    }

    void http_transceiver_1_1::_content_send_handler(connection::error_code code, std::size_t bytes_transferred) {

    }

    void http_transceiver_1_1::async_receive(receive_handler handler) {
        this->_receive_handler = handler;
        auto next_buffer = this->_header_receive_buffer.get_available_block();
        this->_conn->async_read_some(
            next_buffer.base, next_buffer.size,
            std::bind(
                &http_transceiver_1_1::_header_receive_handler, this,
                std::placeholders::_1, std::placeholders::_2
            )
        );
    }

    void http_transceiver_1_1::async_send(std::unique_ptr<http_message> msg, send_handler handler) {
        this->_send_handler = handler;
    }

    http_transceiver_1_1::http_transceiver_1_1(std::unique_ptr<connection> &conn) 
    : _conn(conn) {}

    http_transceiver_1_1::~http_transceiver_1_1() {
        
    }

}

