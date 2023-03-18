#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>

#include "connection.hpp"

namespace myhttpd::network {

    class tcp_connection: public connection {

    private:
        boost::asio::ip::tcp::socket _stream;

    public:
        virtual void async_read_some(mutable_buffer buf, read_handler handler);

        virtual void async_write_some(const_buffer buf, write_handler handler);

        virtual void async_receive(mutable_buffer buf, receive_handler handler);

        virtual void async_send(const_buffer buf, send_handler handler);

        virtual void async_wait(socket_wait_type type, wait_handler handler);

        virtual endpoint get_local_enpoint();

        virtual endpoint get_remote_endpoint();

        virtual void cancel();

        virtual bool is_open();

    public:
        tcp_connection(boost::asio::ip::tcp::socket stream);

        virtual ~tcp_connection();
    };
}

#endif // TCP_CONNECTION_HPP
