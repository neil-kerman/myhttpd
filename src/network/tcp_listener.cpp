
#include "tcp_listener.hpp"
#include "tcp_connection.hpp"

namespace myhttpd::network {

    void tcp_listener::_accept_handler(const asio_error_code error, boost::asio::ip::tcp::socket soc) {

        if (!error) {

            this->_acceptor->async_accept(
                std::bind(&tcp_listener::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
            );
            this->_acception_handler(std::make_unique<tcp_connection>(std::move(soc)));

        } else {

        }
    }

    void tcp_listener::start_async_accept(tcp_acception_handler handler) {

        this->_acception_handler = handler;
        this->_acceptor->listen();
        this->_acceptor->async_accept(
            std::bind(&tcp_listener::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

    void tcp_listener::start_async_accept(acception_handler handler) {

        this->start_async_accept((tcp_acception_handler)handler);
    }

    namespace ip = boost::asio::ip;

    tcp_listener::tcp_listener(std::string addr, int port, boost::asio::io_context& ctx)
        : _acceptor(new ip::tcp::acceptor(ctx, ip::tcp::endpoint(ip::address::from_string(addr), port))) {

    }
}