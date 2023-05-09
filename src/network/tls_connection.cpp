#include <boost/asio.hpp>

#include "tls_connection.hpp"

namespace myhttpd::network {

    void tls_connection::async_read_some(mutable_buffer buf, reading_handler handler) {

        this->_stream->async_read_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tls_connection::async_write_some(const_buffer buf, writing_handler handler) {

        this->_stream->async_write_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tls_connection::async_receive(mutable_buffer buf, receiving_handler handler) {

        boost::asio::async_read(*(this->_stream), boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tls_connection::async_send(const_buffer buf, sending_handler handler) {

        boost::asio::async_write(*(this->_stream), boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tls_connection::async_wait(socket_wait_type type, waiting_handler handler) {

        this->_stream->next_layer().async_wait(type, handler);
    }

    std::string tls_connection::get_type() {

        return "tls";
    }

    std::string tls_connection::get_remote_address() {

        return this->_stream->next_layer().remote_endpoint().address().to_string();
    }

    int tls_connection::get_remote_port() {

        return this->_stream->next_layer().remote_endpoint().port();
    }

    std::string tls_connection::get_local_address() {

        return this->_stream->next_layer().local_endpoint().address().to_string();
    }

    int tls_connection::get_local_port() {

        return this->_stream->next_layer().local_endpoint().port();
    }

    void tls_connection::cancel() {

        this->_stream->next_layer().cancel();
    }

    bool tls_connection::is_open() {

        return this->_stream->next_layer().is_open();
    }

    tls_connection::tls_connection(boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream):
        _stream(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(std::move(stream))) {

    }
}


