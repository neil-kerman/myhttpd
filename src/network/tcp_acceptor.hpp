#ifndef TCP_ACCEPTOR_HPP
#define TCP_ACCEPTOR_HPP

#include <boost/asio.hpp>

#include "acceptor.hpp"
#include "tcp_connection.hpp"

namespace myhttpd::network {

    class tcp_acceptor: public acceptor {

    private:
        boost::asio::ip::tcp::acceptor _raw_acceptor;
        
    public:
        virtual void async_accept(accept_handler handler);

    public:
        tcp_acceptor(std::string address, int port, boost::asio::io_context &ctx);

        virtual ~tcp_acceptor() = default;
    };
}

#endif // TCP_ACCEPTOR_HPP
