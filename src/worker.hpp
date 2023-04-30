#ifndef WORKER_HPP 
#define WORKER_HPP

#include <memory>
#include <map>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <thread>
#include <tinyxml2.h>

#include "session/session.hpp"
#include "session/session_factory.hpp"
#include "session/server.hpp"


namespace myhttpd {

    class worker : public session::server {

    private:
        boost::asio::io_context _ctx;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;

        std::unique_ptr<std::thread> _working_thread = nullptr;

        std::map<boost::uuids::uuid, std::shared_ptr<session::session>> _sessions;

        std::map<std::string, std::unique_ptr<myhttpd::session::session_factory>> _session_factories;

    private:
        void _init_session_factories(tinyxml2::XMLElement* config);

    public:
        virtual void handle_connection(std::unique_ptr<network::connection> conn);

        virtual void request_termination(session::session& sender);

    public:
        void start();

        void join();

    public:
        worker(tinyxml2::XMLElement* config);
    };
}

#endif // WORKER_HPP