#include "listener.hpp"

namespace myhttpd::network {

    listener listener::create_listener(tinyxml2::XMLElement* config, boost::asio::io_context& ctx) {

        std::string address = config->Attribute("address");
        int port = config->IntAttribute("port");
        return listener(address, port, get_listener_tag_from_config(config), ctx);
    }

    listener_tag listener::get_listener_tag_from_config(tinyxml2::XMLElement* config) {

        std::string address = config->Attribute("address");
        std::string port = config->Attribute("port");
        return address + ":" + port;
    }

    void listener::_accept_handler(const asio_error_code error, boost::asio::ip::tcp::socket soc) {

        if (!error) {

            this->_acceptor.async_accept(
                std::bind(&listener::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
            );
            this->_acception_handler(std::move(soc));

        } else {
            
        }
    }

    void listener::start_async_accept(acception_handler handler) {

        this->_acception_handler = handler;
        this->_acceptor.listen();
        this->_acceptor.async_accept(
            std::bind(&listener::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

    listener_tag listener::get_listener_tag() {

        return this->_tag;
    }

    namespace ip = boost::asio::ip;

    listener::listener(std::string address, int port, listener_tag tag, boost::asio::io_context& ctx)
        :_acceptor(ip::tcp::acceptor(ctx, ip::tcp::endpoint(ip::address::from_string(address), port))),
        _tag(tag) {

    }
}