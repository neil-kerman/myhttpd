#include <glog/logging.h>

#include "tcp_acceptor.hpp"

namespace myhttpd::network {

    void tcp_acceptor::async_accept(accept_handler handler) {
        this->_raw_acceptor.async_accept(
            [handler](const boost::system::error_code &error, boost::asio::ip::tcp::socket peer) {
                if (!error) {
                    handler(error, std::make_unique<tcp_connection>(std::move(peer)));
                } else {
                    handler(error, nullptr);
                }
            }
        );
    }

    tcp_acceptor::tcp_acceptor(std::string address, int port, boost::asio::io_context& ctx)
    : _raw_acceptor(
        boost::asio::ip::tcp::acceptor(
            ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address), port)
        )
    ) {
        this->_raw_acceptor.listen();
        DLOG(INFO) << "A tcp_acceptor created, which listening at the local endpoint: " << address << ":" << port;
    }

    tcp_acceptor::~tcp_acceptor() {

    }
}