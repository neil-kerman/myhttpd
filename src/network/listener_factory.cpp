
#include "listener_factory.hpp"
#include "tcp_listener.hpp"
#include "ssl_listener.hpp"

namespace myhttpd::network {

    std::unique_ptr<listener> listener_factory::create_listenr(
        tinyxml2::XMLElement* config, 
        boost::asio::io_context& ctx) {

        std::string name = config->Name();
        auto addr = config->Attribute("address");
        auto port = config->IntAttribute("port");

        if (name == "tcp") {

            return std::make_unique<tcp_listener>(addr, port, ctx);

        } else if (name == "tls") {

            auto tcp_lis = std::make_unique<tcp_listener>(addr, port, ctx);
            auto version = config->Attribute("tls_version");
            auto key_file = config->Attribute("key_file");
            auto cert_file = config->Attribute("cert_file");
            return std::make_unique<ssl_listener>(std::move(tcp_lis), version, key_file, cert_file);

        } else {

            return nullptr;
        }
    }
}