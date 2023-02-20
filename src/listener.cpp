#include "listener.hpp"

#include <glog/logging.h>

listener:: listener(boost::asio::io_context &io, boost::asio::ip::tcp::endpoint endp)
: _io(io), _ac(boost::asio::ip::tcp::acceptor(this->_io, endp)) {
    LOG(INFO) << "Listening at the local endpoint: " << this->_ac.local_endpoint();
}

listener::listener(listener &&lis) 
: _io(lis._io), _handler(lis._handler), _ac(std::move(lis._ac)) {}
