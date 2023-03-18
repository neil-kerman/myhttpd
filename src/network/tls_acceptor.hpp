#ifndef TLS_ACCEPTOR_HPP
#define TLS_ACCEPTOR_HPP

#include <boost/asio/ssl.hpp>

#include "acceptor.hpp"
#include "tcp_acceptor.hpp"
#include "tls_connection.hpp"

namespace myhttpd::network {

    class tls_acceptor: public acceptor {

    private:
        boost::asio::ssl::context _tls_ctx;

        boost::asio::io_context& _ctx;

        boost::asio::ip::tcp::acceptor _raw_acceptor;

    public:
        virtual void async_accept(accept_handler handler);

    public:
        tls_acceptor(std::string address, int port, boost::asio::io_context &ctx, boost::asio::ssl::context tls_ctx);

        virtual ~tls_acceptor() = default;
    };
}

#endif // TLS_ACCEPTOR_HPP
