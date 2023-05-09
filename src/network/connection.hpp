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

            char* data;
            const std::size_t size;
        };

        struct const_buffer {

            const char* data;
            const std::size_t size;
        };

        typedef boost::asio::ip::tcp::endpoint endpoint;

        /* Read event handler */
        typedef std::function<
                    void (const asio_error_code error, std::size_t bytes_transferred)
                > reading_handler;

        /* Write event handler */
        typedef std::function<
                    void (const asio_error_code error, std::size_t bytes_transferred)
                > writing_handler;

        /* Receive event handler */
        typedef std::function<
                    void(const asio_error_code error, std::size_t bytes_transferred)
                > receiving_handler;

        /* send event handler */
        typedef std::function<
                    void(const asio_error_code error, std::size_t bytes_transferred)
                > sending_handler;

        /* Wait event handler */
        typedef std::function<void (const asio_error_code code)> waiting_handler;

    public:
        virtual void async_read_some(mutable_buffer buf, reading_handler handler) = 0;

        virtual void async_write_some(const_buffer buf, writing_handler handler) = 0;

        virtual void async_receive(mutable_buffer buf, receiving_handler handler) = 0;

        virtual void async_send(const_buffer buf, sending_handler handler) = 0;

        virtual void async_wait(socket_wait_type type, waiting_handler handler) = 0;

        virtual std::string get_type() = 0;

        virtual std::string get_remote_address() = 0;

        virtual int get_remote_port() = 0;

        virtual std::string get_local_address() = 0;

        virtual int get_local_port() = 0;

        virtual void cancel() = 0;

        virtual bool is_open() = 0;

    public:
        virtual ~connection() = default;
    };
}

#endif // CONNECTION_HPP
