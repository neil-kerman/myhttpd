#include <string>

#include "handshaker_factory.hpp"
#include "default_handshaker.hpp"
#include "tls_handshaker.hpp"

namespace myhttpd::network {

    std::unique_ptr<handshaker> handshaker_facory::create_handshakre_instance(tinyxml2::XMLElement* config) {

        std::string name = config->Name();

        if (name == "tcp") {

            return default_handshaker::create_instance(config);

        } else if (name == "tls") {

            return tls_handshaker::create_instance(config);

        } else {

            return nullptr;
        }
    }
}