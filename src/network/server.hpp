#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include <memory>

#include "connection.hpp"

namespace myhttpd::network {

    class server {

    public:
        virtual void pass_connection(std::unique_ptr<connection> conn) = 0;

    public:
        virtual ~server() = default;
    };
}

#endif // NETWORK_SERVER