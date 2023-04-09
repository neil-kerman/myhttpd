#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include <map>
#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <tinyxml2.h>

#include "network/acceptor.hpp"
#include "network/server.hpp"
#include "session/session.hpp"
#include "session/session_factory.hpp"
#include "session/server.hpp"

namespace myhttpd {

    class server: public network::server, public session::server {

    private:
        boost::asio::io_context _ctx;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;

        std::list<std::unique_ptr<network::acceptor>> _acceptors;

        std::map<boost::uuids::uuid, std::unique_ptr<session::session>> _sessions;

        std::map<std::string, std::unique_ptr<myhttpd::session::session_factory>> _session_factories;

        std::vector<network::acceptor::accept_handler> _accept_handlers;

    private:
        void _init_acceptors(tinyxml2::XMLElement* config);

        void _init_session_factories(tinyxml2::XMLElement* config);

    public:
        virtual void pass_connection(std::unique_ptr<network::connection> conn);

        virtual void request_termination();

    public:
        void start();
    
        void event_loop();

    public:
        server(tinyxml2::XMLElement *config);
    };
}



#endif // __SERVER_H__