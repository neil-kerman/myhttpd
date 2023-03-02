#ifndef BASIC_CONNECTION_HPP
#define BASIC_CONNECTION_HPP

#include <cstddef>
#include <functional>
#include <boost/asio/buffer.hpp>
#include <boost/system.hpp>

namespace myhttpd {

    
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
            
    class connection {
    public:
        virtual ~connection() {}
        virtual void async_wait(boost::asio::socket_base::wait_type type, wait_handler handler) = 0;
        virtual void async_read(const boost::asio::mutable_buffer &buf, read_handler handler) = 0;
        virtual void async_write(boost::asio::const_buffer &buf, write_handler handler) = 0;
    };
}

#endif // BASIC_CONNECTION_HPP
