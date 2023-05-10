#ifndef WORKER_HPP 
#define WORKER_HPP

#include <memory>
#include <unordered_map>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <tinyxml2.h>

#include "protocol/manager.hpp"
#include "network/listener.hpp"
#include "network/handshaker.hpp"


namespace myhttpd {

    class worker {

    private:
        boost::asio::io_context _ctx;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;

        std::unique_ptr<std::thread> _working_thread = nullptr;

        std::unordered_map<network::listener_tag, std::unique_ptr<network::handshaker>> _handshakers;

        std::unordered_map<std::string, std::unique_ptr<service::manager>> _service_managers;

    private:
        void _init_session_factories(tinyxml2::XMLElement* config);

        void _init_handshakers(tinyxml2::XMLElement* config);

        boost::asio::ip::tcp::socket reset_io_context(boost::asio::ip::tcp::socket soc);

    public:
        void transfer_socket(network::listener_tag tag, boost::asio::ip::tcp::socket soc);

    public:
        void start();

        void join();

    public:
        worker(tinyxml2::XMLElement* config);
    };
}

#endif // WORKER_HPP