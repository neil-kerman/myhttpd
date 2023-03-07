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
            auto& buf = this->_header_receive_buffer;
            this->_buffer_counter += bytes_transferred;
            /* Last block */
            auto blk = buf.back().data();
            for(int counter = 0; counter < bytes_transferred; match_ptr++) {

                if(match.match(blk[(match_ptr + this->_buffer_offset) % BUFFER_BLOCK_SIZE])) {

                    /* Prepare memory for a attribute */
                    std::string attribute;
                    attribute.reserve(this->match_ptr + 1);

                    /* Copy the first block */
                    if(this->_buffer_offset != 0) {
                        attribute.append(buf.front().data() + this->_buffer_offset, BUFFER_BLOCK_SIZE - this->_buffer_offset);
                        buf.pop_front();
                        this->match_ptr -= BUFFER_BLOCK_SIZE - this->_buffer_offset;
                        this->_buffer_counter -= BUFFER_BLOCK_SIZE;
                        this->_buffer_offset = 0;
                    }

                    /* Copy full blocks in middle */
                    auto it = buf.begin();
                    std::size_t full_blks = buf.size() - 1;
                    for(std::size_t i = 0; i < full_blks; i++) {
                        attribute.append((*it).data(), BUFFER_BLOCK_SIZE);
                        it++;
                        buf.pop_front();
                        this->_buffer_counter -= BUFFER_BLOCK_SIZE;
                        this->match_ptr -= BUFFER_BLOCK_SIZE;
                    }

                    /* Copy the last block */
                    if(this->match_ptr != BUFFER_BLOCK_SIZE) {
                        attribute.append(buf.back().data(), this->match_ptr);
                    } else {
                        attribute.append(buf.back().data(), BUFFER_BLOCK_SIZE);
                        buf.pop_front();
                        this->_buffer_counter = 0;
                    }
                    this->match_ptr = 0;
                    this->_buffer_offset = this->_buffer_counter;

                    /* Set title for the http message if it's not have one */
                    if (this->_receiving_message->get_title().size()) {
                        this->_receiving_message->set_title(attribute);
                    } else {
                        /* If this is a empty line */
                        if (attribute == "\r\n") {
                            /* Http message delivering procedure BEGIN */
                            if (!this->_receive_requests.empty()) {
                                this->_inbox.push_back(std::move(this->_receiving_message));
                                this->_receiving_message = std::make_unique<http_message>();
                            } else {
                                while ((!this->_inbox.empty()) && (!this->_receive_requests.empty())) {
                                    auto req = this->_receive_requests.front();
                                    this->_receive_requests.pop_front();
                                    auto msg = std::move(this->_inbox.front());
                                    this->_inbox.pop_front();
                                    req(http_transceiver::error_code::success, std::move(msg));
                                }
                            }
                            /* Http message delivering procedure END */
                        } else {
                            /* Attribute decoding procedure BEGIN */
                            auto name_size = attribute.find(':', 0);
                            int space_count = 0;
                            for (std::size_t i = name_size + 1; (i < attribute.size()) && attribute[i] == ' '; i++) {
                                space_count++;
                            }
                            auto value_offset = name_size + 1;
                            auto value_size = attribute.size() - space_count - name_size - 1;
                            auto name = attribute.substr(0, name_size);
                            auto value = attribute.substr(value_offset, value_size);
                            if (_receiving_message->has_attribute(name)) {
                                auto& attr = _receiving_message->get_attribute(name);
                                attr.push_back(value);
                            } else {
                                this->_receiving_message->set_attrubute(name, { value });
                            }
                            /* Attribute decoding procedure END */
                        }
                    }
                }
            }

            /* Allocate a new block conditionally */
            /* Start another receiving request to the system */
            if(this->_buffer_counter % BUFFER_BLOCK_SIZE == 0) {
                /* Allocate a new block */
                this->_header_receive_buffer.push_back(std::array<char, BUFFER_BLOCK_SIZE>());
                this->_conn->async_read_some(
                    buf.back().data(), 
                    BUFFER_BLOCK_SIZE, 
                    std::bind(&http_transceiver_1_1::_header_receive_handler, this, std::placeholders::_1, std::placeholders::_2)
                );
            } else {
                /* The buffer are not fulled, then fill up the rest of the buffer */
                this->_conn->async_read_some(
                    buf.back().data() + (this->_buffer_counter % BUFFER_BLOCK_SIZE), 
                    BUFFER_BLOCK_SIZE - (this->_buffer_counter % BUFFER_BLOCK_SIZE), 
                    std::bind(&http_transceiver_1_1::_header_receive_handler, this, std::placeholders::_1, std::placeholders::_2)
                );
            }
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
        if(!this->_inbox.empty()) {
            handler(http_transceiver::error_code::success, std::move(this->_inbox.front()));
            this->_inbox.pop_front();
        } else {
            this->_receive_requests.push_back(handler);
        }
    }

    void http_transceiver_1_1::async_send(std::unique_ptr<http_message> msg, send_handler handler) {

    }

    http_transceiver_1_1::http_transceiver_1_1(std::unique_ptr<connection> &conn) 
    : _conn(conn) {
        auto& buf = this->_header_receive_buffer;
        buf.push_back(std::array<char, BUFFER_BLOCK_SIZE>());
        this->_conn->async_read_some(
            buf.back().data(), 
            BUFFER_BLOCK_SIZE, 
            std::bind(&http_transceiver_1_1::_header_receive_handler, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

    http_transceiver_1_1::~http_transceiver_1_1() {
        
    }

}

