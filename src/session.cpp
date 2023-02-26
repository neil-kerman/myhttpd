#include <glog/logging.h>

#include "session.hpp"

session::session(
    std::unique_ptr<basic_connection> conn, std::unique_ptr<protocol> pro, 
    st_handler st
) 
: _conn(std::move(_conn)), _pro(std::move(_pro)), _st_handler(st), 
_id(boost::uuids::random_generator()()) {
    
}

session::~session() {}

void session::start() {

}

void session::terminate() {

}

boost::uuids::uuid session::get_id() {
    return this->_id;
}