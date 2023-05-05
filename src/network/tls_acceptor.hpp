#ifndef TLS_ACCEPTOR_HPP
#define TLS_ACCEPTOR_HPP

#include <boost/asio/ssl.hpp>

#include "acceptor.hpp"
#include "tcp_acceptor.hpp"
#include "tls_connection.hpp"
#include "server.hpp"

namespace myhttpd::network {

    class tls_acceptor: public acceptor {

    private:
        boost::asio::ssl::context _tls_ctx;

        boost::asio::io_context& _ctx;

        boost::asio::ip::tcp::acceptor _raw_acceptor;

        server& _server;

    private:
        void _accept_handler(const asio_error_code error, boost::asio::ip::tcp::socket soc);

    public:
        virtual void start_async_accept();

    public:
        tls_acceptor(std::string address, int port, boost::asio::io_context &ctx, boost::asio::ssl::context tls_ctx, server& ser);

        virtual ~tls_acceptor() = default;
    };
}

#endif // TLS_ACCEPTOR_HPP
