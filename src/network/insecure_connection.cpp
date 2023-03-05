#include "insecure_connection.hpp"

namespace myhttpd {

    insecure_connection::insecure_connection(boost::asio::ip::tcp::socket soc)
    : _soc(std::move(soc)) {}

    insecure_connection::~insecure_connection() {}

    void insecure_connection::async_write_some(const char *buf, std::size_t size, write_handler handler) {
        this->_soc.async_write_some(boost::asio::buffer(buf, size), [handler](const boost::system::error_code &code, std::size_t bytes_transferred) {
            error_code ecode = (!code) ? error_code::success : error_code::error;
            handler(ecode, bytes_transferred);
        });
    }

    void insecure_connection::async_read_some(char *buf, std::size_t size, read_handler handler) {
        this->_soc.async_read_some(boost::asio::buffer(buf, size), [handler](const boost::system::error_code &code, std::size_t bytes_transferred) {
            error_code ecode = (!code) ? error_code::success : error_code::error;
            handler(ecode, bytes_transferred);
        });
    }

    void insecure_connection::async_wait(wait_type type, wait_handler handler) {
        this->_soc.async_wait(type, [handler](const boost::system::error_code &code) {
            error_code ecode = (!code) ? error_code::success : error_code::error;
            handler(ecode);
        });
    }

    connection::endpoint insecure_connection::get_remote_endpoint() {
        return this->_soc.remote_endpoint(); 
    }
}