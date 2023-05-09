#ifndef TCP_ACCEPTOR_HPP
#define TCP_ACCEPTOR_HPP

#include <boost/asio.hpp>
#include <memory>

#include "listener.hpp"
#include "handshaker.hpp"
#include "default_connection.hpp"

namespace myhttpd::network {

    class default_handshaker: public handshaker {

    public:
        static std::unique_ptr<default_handshaker> create_instance(tinyxml2::XMLElement* config);

    public:
        virtual void async_handshake(boost::asio::ip::tcp::socket soc, handshaking_handler handler);

    public:
        default_handshaker(listener_tag tag);

        virtual ~default_handshaker() = default;
    };
}

#endif // TCP_ACCEPTOR_HPP
