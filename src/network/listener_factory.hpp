#include <tinyxml2.h>
#include <boost/asio.hpp>

#include "listener.hpp"

namespace myhttpd::network {

    class listener_factory {

    public:
        static std::unique_ptr<listener> create_listenr(
            tinyxml2::XMLElement* config, 
            boost::asio::io_context& ctx
        );
    };
}