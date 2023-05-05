#ifndef WORKER_HPP 
#define WORKER_HPP

#include <memory>
#include <unordered_map>
#include <thread>
#include <boost/asio/io_context.hpp>
#include <tinyxml2.h>

#include "protocol/manager.hpp"


namespace myhttpd {

    class worker {

    private:
        boost::asio::io_context _ctx;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;

        std::unique_ptr<std::thread> _working_thread = nullptr;

        std::unordered_map<std::string, std::unique_ptr<protocol::manager>> _session_factories;

    private:
        void _init_session_factories(tinyxml2::XMLElement* config);

    public:
        virtual void handle_connection(std::unique_ptr<network::connection> conn);

    public:
        void start();

        void join();

    public:
        worker(tinyxml2::XMLElement* config);
    };
}

#endif // WORKER_HPP