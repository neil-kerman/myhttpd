#ifndef PROTOCOL_MANAGER_HPP
#define PROTOCOL_MANAGER_HPP

#include <memory>
#include <boost/asio.hpp>

#include "session.hpp"
#include "network/connection.hpp"

namespace myhttpd::protocol {

    class manager {

    public:
        virtual void create_session(std::unique_ptr<myhttpd::network::connection> conn) = 0;

    public:
        virtual ~manager() = default;
    };
}

#endif // PROTOCOL_MANAGER_HPP
