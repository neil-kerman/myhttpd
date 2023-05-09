#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <memory>
#include <functional>
#include <tinyxml2.h>

#include "listener.hpp"
#include "connection.hpp"

namespace myhttpd::network {

    class handshaker {

    public:
        typedef std::function<void(std::unique_ptr<connection>)> handshaking_handler; 

    private:
        listener_tag _tag;

    public:
        virtual void async_handshake(boost::asio::ip::tcp::socket soc, handshaking_handler handler) = 0;

        listener_tag get_listener_tag() { return this->_tag; }

    public:
        handshaker(listener_tag tag) : _tag(tag) {}

        virtual ~handshaker() = default;
    };
}

#endif // ACCEPTOR_HPP
