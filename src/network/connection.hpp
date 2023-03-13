#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <cstddef>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/system.hpp>

#include "alias.hpp"

namespace myhttpd::network {
            
    class connection {

    public:
        struct mutable_buffer {
            void* data;
            const std::size_t size;
        };

        struct const_buffer {
            const void* data;
            const std::size_t size;
        };

        /* Read event handler */
        typedef std::function<
                    void (const asio_error_code& error, std::size_t bytes_transferred)
                > read_handler;

        /* Write event handler */
        typedef std::function<
                    void (const asio_error_code& error, std::size_t bytes_transferred)
                > write_handler;

        /* Receive event handler */
        typedef std::function<
                    void(const asio_error_code& error, std::size_t bytes_transferred)
                > receive_handler;

        /* send event handler */
        typedef std::function<
                    void(const asio_error_code& error, std::size_t bytes_transferred)
                > send_handler;

        /* Wait event handler */
        typedef std::function<void (const asio_error_code&code)> wait_handler;

    public:
        virtual void async_read_some(mutable_buffer buf, read_handler handler) = 0;

        virtual void async_write_some(const_buffer buf, write_handler handler) = 0;

        virtual void async_receive(mutable_buffer buf, receive_handler handler) = 0;

        virtual void async_send(const_buffer buf, send_handler handler) = 0;

        virtual void async_wait(socket_wait_type type, wait_handler handler) = 0;

        virtual void cancel() = 0;

    public:
        virtual ~connection() {}
    };
}

#endif // CONNECTION_HPP
