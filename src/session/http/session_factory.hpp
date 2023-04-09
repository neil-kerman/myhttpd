#ifndef HTTP_SESSION_FACTORY_HPP
#define HTTP_SESSION_FACTORY_HPP

#include <tinyxml2.h>
#include <boost/asio.hpp>

#include "session/session_factory.hpp"
#include "resource.hpp"

namespace myhttpd::session::http{

    class session_factory : public myhttpd::session::session_factory {

    public:
        struct session_config {
            unsigned int keepalive_time = 30;
        };

    private:
        boost::asio::io_context& _ctx;

        resource _resource;

        session_config _session_config;

        server& _server;

    public:
        virtual std::unique_ptr<myhttpd::session::session> 
            create_session(std::unique_ptr<myhttpd::network::connection> conn);

    public:
        session_factory(tinyxml2::XMLElement* config, boost::asio::io_context& ctx, server &ser);

        virtual ~session_factory() = default;
    };
}

#endif // HTTP_SESSION_FACTORY_HPP
