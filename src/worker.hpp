#ifndef WORKER_HPP 
#define WORKER_HPP

#include <memory>
#include <unordered_map>
#include <thread>
#include <boost/asio/io_context.hpp>
#include <tinyxml2.h>

#include "protocol/manager.hpp"
#include "network/connection.hpp"


namespace myhttpd {

    class worker {

    private:
        boost::asio::io_context _ctx;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;

        std::unique_ptr<std::thread> _working_thread = nullptr;

        std::unordered_map<std::string, std::unique_ptr<service::manager>> _service_managers;

    private:
        void _init_session_factories(tinyxml2::XMLElement* config);

        boost::asio::ip::tcp::socket reset_io_context(boost::asio::ip::tcp::socket soc);

    public:
        void transfer_socket(std::unique_ptr<network::connection> conn);

    public:
        void start();

        void join();

    public:
        worker(tinyxml2::XMLElement* config);
    };
}

#endif // WORKER_HPP