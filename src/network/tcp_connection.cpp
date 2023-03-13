#include <boost/asio/buffer.hpp>

#include "tcp_connection.hpp"

namespace myhttpd::network {

    void tcp_connection::async_read_some(mutable_buffer buf, read_handler handler) {
        this->_stream.async_read_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tcp_connection::async_write_some(const_buffer buf, write_handler handler) {
        this->_stream.async_write_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tcp_connection::async_receive(mutable_buffer buf, receive_handler handler) {
        this->_stream.async_receive(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tcp_connection::async_send(const_buffer buf, send_handler handler) {
        this->_stream.async_send(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tcp_connection::async_wait(socket_wait_type type, wait_handler handler) {
        this->_stream.async_wait(type, handler);
    }

    void tcp_connection::cancel() {
        this->_stream.cancel();
    }

    tcp_connection::tcp_connection(boost::asio::ip::tcp::socket stream)
    : _stream(std::move(stream)) {
    }

    tcp_connection::~tcp_connection() {
    }
}