#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>

#include "connection.hpp"

namespace myhttpd::network {

    class default_connection: public connection {

    private:
        std::unique_ptr<boost::asio::ip::tcp::socket> _stream;

    public:
        virtual void async_read_some(mutable_buffer buf, reading_handler handler);

        virtual void async_write_some(const_buffer buf, writing_handler handler);

        virtual void async_receive(mutable_buffer buf, receiving_handler handler);

        virtual void async_send(const_buffer buf, sending_handler handler);

        virtual void async_wait(socket_wait_type type, waiting_handler handler);

        virtual std::string get_type();

        virtual std::string get_remote_address();

        virtual int get_remote_port();

        virtual std::string get_local_address();

        virtual int get_local_port();

        virtual void cancel();

        virtual bool is_open();

    public:
        default_connection(boost::asio::ip::tcp::socket stream);

        virtual ~default_connection() = default;
    };
}

#endif // TCP_CONNECTION_HPP
