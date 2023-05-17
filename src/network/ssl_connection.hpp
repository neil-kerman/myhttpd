#ifndef TLS_CONNECTION_HPP
#define TLS_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "tcp_connection.hpp"

namespace myhttpd::network {

    typedef boost::asio::ssl::stream<asio_tcp_socket> asio_ssl_stream;

    class ssl_connection: public connection {

    private:
        std::unique_ptr<tcp_connection> _tcp_conn;

        boost::asio::ssl::context& _ssl_ctx;

        std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> _stream;

    public:
        virtual void async_read_some(mutable_buffer buf, reading_handler handler);

        virtual void async_write_some(const_buffer buf, writing_handler handler);

        virtual void async_receive(mutable_buffer buf, receiving_handler handler);

        virtual void async_send(const_buffer buf, sending_handler handler);

        virtual void async_wait(socket_wait_type type, waiting_handler handler);

        virtual std::string get_remote_address();

        virtual int get_remote_port();

        virtual std::string get_local_address();

        virtual int get_local_port();

        virtual void cancel();

        virtual bool is_open();

        virtual bool is_secure();

        virtual void async_init(boost::asio::io_context& ctx, init_handler handler);

    public:
        ssl_connection(std::unique_ptr<tcp_connection> conn, boost::asio::ssl::context& tls_ctx);

        virtual ~ssl_connection() = default;
    };
}

#endif // TLS_CONNECTION_HPP
