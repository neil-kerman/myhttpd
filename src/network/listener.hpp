#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <boost/asio.hpp>
#include <functional>
#include <tinyxml2.h>

#include "alias.hpp"

namespace myhttpd::network {

    typedef std::string listener_tag;

    class listener {

    public:
        typedef std::function<void(boost::asio::ip::tcp::socket)> acception_handler;

    private:
        boost::asio::ip::tcp::acceptor _acceptor;

        listener_tag _tag;

        acception_handler _acception_handler;

    public:
        static listener create_listener(tinyxml2::XMLElement* config, boost::asio::io_context& ctx);

        static listener_tag get_listener_tag_from_config(tinyxml2::XMLElement* config);

    private:
        void _accept_handler(const asio_error_code error, boost::asio::ip::tcp::socket soc);

    public:
        void start_async_accept(acception_handler handler);

        listener_tag get_listener_tag();

    private:
        listener(std::string address, int port, listener_tag tag, boost::asio::io_context& ctx);
    };
}

#endif // LISTENER_HPP