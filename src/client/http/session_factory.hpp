#ifndef HTTP_SESSION_FACTORY_HPP
#define HTTP_SESSION_FACTORY_HPP

#include <tinyxml2.h>
#include <boost/asio.hpp>

#include "client/session_factory.hpp"
#include "resource.hpp"

namespace myhttpd::http{

    class session_factory : public myhttpd::session_factory {

    public:
        struct session_config {
            unsigned int keepalive_time;
        };

    private:
        boost::asio::io_context& _ctx;

        resource _resource;

        session_config _session_config;

    public:
        virtual std::unique_ptr<myhttpd::session> 
            create_session(std::unique_ptr<myhttpd::connection> conn);

    public:
        session_factory(tinyxml2::XMLElement* config, boost::asio::io_context& ctx);

        virtual ~session_factory();
    };
}

#endif // HTTP_SESSION_FACTORY_HPP
