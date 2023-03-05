#include "secure_connection.hpp"

namespace myhttpd {
    secure_connection::secure_connection(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket soc)
    : _soc(std::move(soc), ctx) {}

    secure_connection::~secure_connection() {}

    void secure_connection::async_write_some(const char *buf, std::size_t size, write_handler handler) {
        this->_soc.async_write_some(boost::asio::buffer(buf, size), [handler](const boost::system::error_code &code, std::size_t bytes_transferred) {
            error_code ecode = (!code) ? error_code::success : error_code::error;
            handler(ecode, bytes_transferred);
        });
    }

    void secure_connection::async_read_some(char *buf, std::size_t size, read_handler handler) {
        this->_soc.async_read_some(boost::asio::buffer(buf, size), [handler](const boost::system::error_code &code, std::size_t bytes_transferred) {
            error_code ecode = (!code) ? error_code::success : error_code::error;
            handler(ecode, bytes_transferred);
        });
    }

    void secure_connection::async_wait(wait_type type, wait_handler handler) {
        this->_soc.next_layer().async_wait(type, [handler](const boost::system::error_code &code) {
            error_code ecode = (!code) ? error_code::success : error_code::error;
            handler(ecode);
        });
    }   

    void secure_connection::async_handshake(handshake_handler handler) {
        using boost::asio::ssl::stream;
        using boost::asio::ip::tcp;
        this->_soc.async_handshake(stream<tcp::socket>::server, [handler](const boost::system::error_code &code) {
            error_code ecode = (!code) ? error_code::success : error_code::error;
            handler(ecode);
        });
    }

    connection::endpoint secure_connection::get_remote_endpoint() {
        return this->_soc.lowest_layer().remote_endpoint();
    }
}


