#ifndef TLS_ACCEPTOR_HPP
#define TLS_ACCEPTOR_HPP

#include <boost/asio/ssl.hpp>
#include <tinyxml2.h>
#include <memory>

#include "handshaker.hpp"

namespace myhttpd::network {

    class tls_handshaker: public handshaker {

    private:
        boost::asio::ssl::context _tls_ctx;

    public:
        static std::unique_ptr<tls_handshaker> create_instance(tinyxml2::XMLElement* config);

    public:
        virtual void async_handshake(boost::asio::ip::tcp::socket soc, handshaking_handler handler);

    public:
        tls_handshaker(listener_tag tag, boost::asio::ssl::context tls_ctx);

        virtual ~tls_handshaker() = default;
    };
}

#endif // TLS_ACCEPTOR_HPP
