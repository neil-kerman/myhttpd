#ifndef ACCEPTOR_FACTORY_HPP
#define ACCEPTOR_FACTORY_HPP

#include <memory>
#include <tinyxml2.h>

#include "handshaker.hpp"

namespace myhttpd::network {

    class handshaker_facory {

    public:
        static std::unique_ptr<handshaker> create_handshakre_instance(tinyxml2::XMLElement* config);
    };
}

#endif // ACCEPTOR_FACTORY_HPP
