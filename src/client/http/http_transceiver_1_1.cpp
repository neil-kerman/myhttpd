#include <string>
#include <functional>
#include <strstream>

#include "http_transceiver_1_1.hpp"

namespace myhttpd {

    void http_transceiver_1_1::_header_receive_handler(connection::error_code code, std::size_t bytes_transferred) {
        if(!code) {
            auto &buf = this->_header_receive_buffer;
            this->_bytes_counter += bytes_transferred;
            /* Last block */
            auto blk = buf.back().data();
            for(int counter = 0; counter < bytes_transferred; match_ptr++) {
                if(match.match(blk[(match_ptr + this->match_offset) % BUFFER_BLOCK_SIZE])) {
                    /* TODO: optimize for small request headers(<=4K) */

                    /* Prepare memory for a header */
                    std::string header;
                    header.reserve(this->match_ptr + 1);

                    /* Copy the first block */
                    if(this->match_offset != 0) {
                        header.append(buf.front().data() + this->match_offset, BUFFER_BLOCK_SIZE - this->match_offset);
                        buf.pop_front();
                        this->match_ptr -= BUFFER_BLOCK_SIZE - this->match_offset;
                        this->_bytes_counter -= BUFFER_BLOCK_SIZE;
                        this->match_offset = 0;
                    }

                    /* Copy full blocks in middle */
                    auto it = buf.begin();
                    std::size_t full_blks = buf.size() - 1;
                    for(std::size_t i = 0; i < full_blks; i++) {
                        header.append((*it).data(), BUFFER_BLOCK_SIZE);
                        it++;
                        buf.pop_front();
                        this->_bytes_counter -= BUFFER_BLOCK_SIZE;
                        this->match_ptr -= BUFFER_BLOCK_SIZE;
                    }

                    /* Copy the last block */
                    if(this->match_ptr != BUFFER_BLOCK_SIZE) {
                        header.append(buf.back().data(), this->match_ptr);
                    } else {
                        header.append(buf.back().data(), BUFFER_BLOCK_SIZE);
                        buf.pop_front();
                        this->_bytes_counter = 0;
                    }
                    this->match_ptr = 0;
                    this->match_offset = this->_bytes_counter;

                    /* Deliver this http request */
                    if(!this->_receive_requests.empty()) {
                        auto &rc_req = this->_receive_requests.front();
                        rc_req(http_transceiver::error_code::success, std::make_unique<http_request>(std::move(header)));
                        this->_receive_requests.pop_front();
                    } else {
                        this->_inbox.push_back(std::make_unique<http_request>(std::move(header)));
                    }
                    break;
                }
            }
            /* Allocate a new block conditionally*/
            if(this->_bytes_counter % BUFFER_BLOCK_SIZE == 0) {
                this->_header_receive_buffer.push_back(std::array<char, BUFFER_BLOCK_SIZE>());
                this->_conn->async_read_some(
                    buf.back().data(), 
                    BUFFER_BLOCK_SIZE, 
                    std::bind(&http_transceiver_1_1::_header_receive_handler, this, std::placeholders::_1, std::placeholders::_2)
                );
            } else {
                this->_conn->async_read_some(
                    buf.back().data() + (this->_bytes_counter % BUFFER_BLOCK_SIZE), 
                    BUFFER_BLOCK_SIZE - (this->_bytes_counter % BUFFER_BLOCK_SIZE), 
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

    void http_transceiver_1_1::async_send(std::unique_ptr<http_response> response, send_handler handler) {
        std::size_t header_size = sizeof("HTTP/")


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

