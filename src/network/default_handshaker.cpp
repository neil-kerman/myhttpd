#include <functional>

#include "default_handshaker.hpp"

namespace myhttpd::network {

    std::unique_ptr<default_handshaker> default_handshaker::create_instance(tinyxml2::XMLElement* config) {

        return std::make_unique<default_handshaker>(listener::get_listener_tag_from_config(config));
    }

    void default_handshaker::async_handshake(boost::asio::ip::tcp::socket soc, handshaking_handler handler) {

        handler(std::make_unique<default_connection>(std::move(soc)));
    }

    default_handshaker::default_handshaker(listener_tag tag): handshaker(tag) {

    }
}