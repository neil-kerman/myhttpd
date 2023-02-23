#include "session.hpp"

#include <glog/logging.h>

session::session(std::unique_ptr<connection> conn, std::function<void (session &ses)> close_handler)
: _conn(std::move(conn)), _close_handler(close_handler), _id(boost::uuids::random_generator()()) {
    LOG(INFO) << "A new session has created, id: " << this->_id;
}

session::session(session &&ses)
:_id(ses._id), _conn(std::move(ses._conn)), _close_handler(ses._close_handler) {}

session::~session() {
    LOG(INFO) << "A session has destroied, id: " << this->_id;
}

bool session::operator<(const session &right) const{
    return (_id < right._id);
}