#include <glog/logging.h>
#include <boost/regex.hpp>

#include "http_session.hpp"

namespace myhttpd {

    http_session::http_session(std::unique_ptr<connection> conn, resource &res)
    : _conn(std::move(conn)), _transceiver(std::make_unique<http_transceiver_1_1>(this->_conn)),
    _resource(res) {}

    http_session::~http_session() {}

    void http_session::start(terminated_handler handler) {
        this->receive_request();
    }

    void http_session::receive_request() {
        this->_transceiver->async_receive([&](http_transceiver::error_code code, std::unique_ptr<http_message> request) {
            LOG(INFO) << request->header.front();
            this->receive_request();
        });
    }
}

