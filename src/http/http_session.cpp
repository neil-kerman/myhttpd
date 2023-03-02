#include <glog/logging.h>

#include "http_session.hpp"

namespace myhttpd {

    http_session::http_session(std::unique_ptr<connection> conn)
    : _conn(std::move(conn)){}

    http_session::~http_session() {}

    void http_session::start(terminated_handler handler) {

    }
}

