#ifndef SESSION_FACTORY_HPP
#define SESSION_FACTORY_HPP

#include <memory>
#include <boost/asio.hpp>

#include "session.hpp"
#include "network/connection.hpp"


namespace myhttpd {

    class session_factory {

    public:
        virtual std::unique_ptr<session> create_session(std::unique_ptr<myhttpd::network::connection> conn) = 0;

    public:
        virtual ~session_factory() = default;
    };
}

#endif // SESSION_FACTORY_HPP
