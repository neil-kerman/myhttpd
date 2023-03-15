#ifndef TLS_CONNECTION_HPP
#define TLS_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "connection.hpp"

namespace myhttpd::network {

    class tls_connection: public connection {

    private:
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> _stream;

    public:
        virtual void async_read_some(mutable_buffer buf, read_handler handler);

        virtual void async_write_some(const_buffer buf, write_handler handler);

        virtual void async_receive(mutable_buffer buf, receive_handler handler);

        virtual void async_send(const_buffer buf, send_handler handler);

        virtual void async_wait(socket_wait_type type, wait_handler handler);

        virtual endpoint get_local_enpoint();

        virtual endpoint get_remote_endpoint();

        virtual void cancel();

    public:
        tls_connection(boost::asio::ssl::stream<boost::asio::ip::tcp::socket> _stream);

        virtual ~tls_connection();
    };
}

#endif // TLS_CONNECTION_HPP
