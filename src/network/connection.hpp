#ifndef BASIC_CONNECTION_HPP
#define BASIC_CONNECTION_HPP

#include <cstddef>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/system.hpp>

namespace myhttpd {
            
    class connection {
    public:
        typedef boost::asio::socket_base::wait_type wait_type;
        /* Wait event handler */
        typedef std::function<
                    void (const boost::system::error_code &code)
                > wait_handler;
        /* Read event handler */
        typedef std::function<
                    void (const boost::system::error_code &code, std::size_t bytes_transferred)
                > read_handler;
        /* Write event handler */
        typedef std::function<
                    void (const boost::system::error_code &code, std::size_t bytes_transferred)
                > write_handler;
    public:
        virtual ~connection() {}
        virtual void async_read_some(const boost::asio::mutable_buffer &buf, read_handler handler) = 0;
        virtual void async_write_some(boost::asio::const_buffer &buf, write_handler handler) = 0;
        virtual void async_wait(wait_type type, wait_handler handler) = 0;
        virtual boost::asio::ip::tcp::endpoint get_endpoint() = 0;
    };
}

#endif // BASIC_CONNECTION_HPP
