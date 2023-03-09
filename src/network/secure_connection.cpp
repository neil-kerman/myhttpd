#include "secure_connection.hpp"

namespace myhttpd {
    secure_connection::secure_connection(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket soc)
    : _soc(std::move(soc), ctx) {}

    secure_connection::~secure_connection() {}

    void secure_connection::async_write_some(const char *buf, std::size_t size, write_handler handler) {
        this->_soc.async_write_some(boost::asio::buffer(buf, size), 
            [handler](const boost::system::error_code &code, std::size_t bytes_transferred) {
            if (!code) {
                handler(success, bytes_transferred);
            } else if (code == boost::asio::error::operation_aborted) {
                handler(canceled, bytes_transferred);
            } else {
                handler(error, bytes_transferred);
            }
        });
    }

    void secure_connection::async_read_some(char *buf, std::size_t size, read_handler handler) {
        this->_soc.async_read_some(boost::asio::buffer(buf, size), 
            [handler](const boost::system::error_code &code, std::size_t bytes_transferred) {
            if (!code) {
                handler(success, bytes_transferred);
            } else if (code == boost::asio::error::operation_aborted) {
                handler(canceled, bytes_transferred);
            } else {
                handler(error, bytes_transferred);
            }
        });
    }

    void secure_connection::async_wait(wait_type type, wait_handler handler) {
        using boost::asio::socket_base;
        socket_base::wait_type type_;
        auto lam = [handler](const boost::system::error_code& code) {
            if (!code) {
                handler(success);
            } else if (code == boost::asio::error::operation_aborted) {
                handler(canceled);
            } else {
                handler(error);
            }
        };
        if (type == connection::wait_read) {
            type_ = socket_base::wait_read;
            this->_soc.next_layer().async_wait(type_, lam);
        } else if (type == connection::wait_write) {
            type_ = socket_base::wait_write;
            this->_soc.next_layer().async_wait(type_, lam);
        }
    }

    void secure_connection::cancel() {
        this->_soc.next_layer().cancel();
    }

    void secure_connection::async_handshake(handshake_handler handler) {
        using boost::asio::ssl::stream;
        using boost::asio::ip::tcp;
        this->_soc.async_handshake(stream<tcp::socket>::server, [handler](const boost::system::error_code &code) {
            if (!code) {
                handler(success);
            } else if (code == boost::asio::error::operation_aborted) {
                handler(canceled);
            } else {
                handler(error);
            }
        });
    }
}


