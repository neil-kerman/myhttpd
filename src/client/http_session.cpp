#include <glog/logging.h>
#include <boost/regex.hpp>

#include "http_session.hpp"

namespace myhttpd {

    http_session::http_session(std::unique_ptr<connection> conn)
    : _conn(std::move(conn)) {}

    http_session::~http_session() {}

    void http_session::start(terminated_handler handler) {
        this->receive_request();
    }

    void http_session::receive_request() {
        this->_buffers.clear();
        this->_buffers.push_back(std::array<char, 4096>());
        auto &block = this->_buffers.back();
        this->_conn->async_read_some(boost::asio::buffer(block.data(), block.size()), 
            [this, &block, &buffers = this->_buffers](const boost::system::error_code& error, std::size_t bytes_transferred) {
                if(!error) {
                    bool found_header_end_signature = false;
                    std::string data(block.data(), bytes_transferred);
                    LOG(INFO) << data;
                    // static const boost::regex patt{"\\r\\n\\r\\n"};
                    // boost::smatch match;
                    // if(boost::regex_search(data, match, patt)) {
                    //     match.position();
                    // } else {

                    // }
                    this->_terminated_handler();
                }
            }
        );
    }
}

