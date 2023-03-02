#include "insecure_connection.hpp"

namespace myhttpd {

    insecure_connection::insecure_connection(ordinary_soc soc)
    : _soc(std::move(soc)) {}

    insecure_connection::~insecure_connection() {}

    void insecure_connection::async_write(boost::asio::const_buffer &buf, write_handler handler) {
        this->_soc.async_write_some(buf, handler);
    }

    void insecure_connection::async_read(const boost::asio::mutable_buffer &buf, read_handler handler) {
        this->_soc.async_read_some(buf, handler);
    }

    void insecure_connection::async_wait(boost::asio::socket_base::wait_type type, wait_handler handler) {
        this->_soc.async_wait(type, handler);
    }
}