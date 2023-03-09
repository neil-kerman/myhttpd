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
        enum error_code {
            success = 0,
            canceled = boost::asio::error::operation_aborted,
            error,
        };

        enum wait_type {
            wait_read = boost::asio::socket_base::wait_read,
            wait_write = boost::asio::socket_base::wait_write,
        };


        /* Wait event handler */
        typedef std::function<
                    void (error_code code)
                > wait_handler;

        /* Read event handler */
        typedef std::function<
                    void (error_code code, std::size_t bytes_transferred)
                > read_handler;

        /* Write event handler */
        typedef std::function<
                    void (error_code code, std::size_t bytes_transferred)
                > write_handler;

    public:
        virtual void async_read_some(char *buf, std::size_t size, read_handler handler) = 0;

        virtual void async_write_some(const char *buf, std::size_t size, write_handler handler) = 0;

        virtual void async_wait(wait_type type, wait_handler handler) = 0;

        virtual void cancel() = 0;

    public:
        virtual ~connection() {}
    };
}

#endif // BASIC_CONNECTION_HPP
