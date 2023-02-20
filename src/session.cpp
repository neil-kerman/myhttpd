#include "session.hpp"

session::session(std::unique_ptr<connection> &&conn, std::function<void (session &ses)> terminated_handler)
: _connection(std::move(conn)), _terminated_handler(terminated_handler), _id(boost::uuids::random_generator()()) {
    
}

boost::uuids::uuid session::id() {
    return this->_id;
}