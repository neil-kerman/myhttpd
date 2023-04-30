#include <glog/logging.h>
#include <functional>

#include "tcp_acceptor.hpp"

namespace myhttpd::network {

    void tcp_acceptor::_accept_handler(const asio_error_code error, boost::asio::ip::tcp::socket soc) {

        if (!error) {

            this->_server.pass_connection(std::make_unique<tcp_connection>(std::move(soc)));
            this->_raw_acceptor.async_accept(
                std::bind(&tcp_acceptor::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
            );

        } else if (error == asio_error::operation_aborted){
            
        } else {

            this->_raw_acceptor.async_accept(
                std::bind(&tcp_acceptor::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
            );
        }
    }

    void tcp_acceptor::start_async_accept() {

        this->_raw_acceptor.listen();
        this->_raw_acceptor.async_accept(
            std::bind(&tcp_acceptor::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

    using namespace boost::asio::ip;

    tcp_acceptor::tcp_acceptor(std::string address, int port, boost::asio::io_context& ctx, server& ser)
    :_raw_acceptor(tcp::acceptor(ctx, tcp::endpoint(address::from_string(address), port))), _server(ser) {

        this->_raw_acceptor.listen();
    }
}