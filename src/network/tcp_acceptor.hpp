#ifndef TCP_ACCEPTOR_HPP
#define TCP_ACCEPTOR_HPP

#include <boost/asio.hpp>

#include "acceptor.hpp"
#include "tcp_connection.hpp"
#include "server.hpp"

namespace myhttpd::network {

    class tcp_acceptor: public acceptor {

    private:
        boost::asio::ip::tcp::acceptor _raw_acceptor;

        server& _server;

    private:
        void _accept_handler(const asio_error_code& error, boost::asio::ip::tcp::socket soc);

    public:
        virtual void start_async_accept();

    public:
        tcp_acceptor(std::string address, int port, boost::asio::io_context &ctx, server& ser);

        virtual ~tcp_acceptor() = default;
    };
}

#endif // TCP_ACCEPTOR_HPP
