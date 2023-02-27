#include <glog/logging.h>

#include "session.hpp"

namespace myhttpd {
    session::session(std::unique_ptr<connection::connection> conn, st_handler st) 
    : _conn(std::move(_conn)), _st_handler(st), _id(boost::uuids::random_generator()()) {
        
    }

    session::~session() {}

    void session::terminate() {

    }

    boost::uuids::uuid session::get_id() {
        return this->_id;
    }
}
