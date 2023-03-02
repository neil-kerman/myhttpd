#include "secure_connection.hpp"

namespace myhttpd {
    secure_connection::secure_connection(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket soc) 
    : _soc(std::move(soc), ctx) {}

    secure_connection::~secure_connection() {}

    void secure_connection::async_write(boost::asio::const_buffer &buf, write_handler handler) {
        this->_soc.async_write_some(buf, handler);
    }

    void secure_connection::async_read(const boost::asio::mutable_buffer &buf, read_handler handler) {
        this->_soc.async_read_some(buf, handler);
    }

    void secure_connection::async_wait(boost::asio::socket_base::wait_type type, wait_handler handler) {
        this->_soc.next_layer().async_wait(type, handler);
    }   

    void secure_connection::async_handshake(handshake_handler handler) {
        using boost::asio::ssl::stream;
        using boost::asio::ip::tcp;
        this->_soc.async_handshake(stream<tcp::socket>::server, handler);
    }
}


