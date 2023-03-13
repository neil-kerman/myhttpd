#ifndef ACCEPTOR_FACTORY_HPP
#define ACCEPTOR_FACTORY_HPP

#include <memory>
#include <tinyxml2.h>

#include "acceptor.hpp"

namespace myhttpd::network {
    class acceptor_facory {
    public:
        static std::unique_ptr<acceptor> create_acceptor(tinyxml2::XMLElement* config, boost::asio::io_context &ctx);
    };
}

#endif // ACCEPTOR_FACTORY_HPP
