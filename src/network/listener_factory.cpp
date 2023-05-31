#include <string>

#include "listener_factory.hpp"
#include "tcp_listener.hpp"
#include "ssl_listener.hpp"

namespace myhttpd::network {

    std::unique_ptr<listener> listener_factory::create_listenr(
        tinyxml2::XMLElement* config, 
        boost::asio::io_context& ctx) {

        std::string type = config->Attribute("type");
        auto addr = config->Attribute("address");
        auto port = config->IntAttribute("port");

        if (type == "tcp") {

            return std::make_unique<tcp_listener>(addr, port, ctx);

        } else if (type == "ssl") {

            auto* ssl_conf = config->FirstChildElement("ssl");
            auto tcp_lis = std::make_unique<tcp_listener>(addr, port, ctx);
            auto version = ssl_conf->Attribute("version");
            auto key_file = ssl_conf->Attribute("key_file");
            auto cert_file = ssl_conf->Attribute("cert_file");
            return std::make_unique<ssl_listener>(std::move(tcp_lis), version, key_file, cert_file);

        } else {

            return nullptr;
        }
    }
}