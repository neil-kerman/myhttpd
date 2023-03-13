#include <boost/asio.hpp>

#include "tls_connection.hpp"

namespace myhttpd::network {

    void tls_connection::async_read_some(mutable_buffer buf, read_handler handler) {
        this->_stream.async_read_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tls_connection::async_write_some(const_buffer buf, write_handler handler) {
        this->_stream.async_write_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tls_connection::async_receive(mutable_buffer buf, receive_handler handler) {
        boost::asio::async_read(this->_stream, boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tls_connection::async_send(const_buffer buf, send_handler handler) {
        boost::asio::async_write(this->_stream, boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tls_connection::async_wait(socket_wait_type type, wait_handler handler) {
        this->_stream.next_layer().async_wait(type, handler);
    }

    void tls_connection::cancel() {
        this->_stream.next_layer().cancel();
    }

    tls_connection::tls_connection(boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream)
    : _stream(std::move(stream)) {
    }

    tls_connection::~tls_connection() {
    }
}


