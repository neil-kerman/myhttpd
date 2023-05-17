#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <tinyxml2.h>

#include "network/listener.hpp"
#include "worker.hpp"

namespace myhttpd {

    class server {

    private:
        boost::asio::io_context _ctx;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;

        std::unique_ptr<std::thread> _thread = nullptr;

        std::list<std::unique_ptr<network::listener>> _listeners;

        std::list<std::unique_ptr<worker>> _workers;

    private:
        void _init_listeners(tinyxml2::XMLElement* config);

        void _init_workers(tinyxml2::XMLElement* config);

    public:
        void start();
    
        void join();

    public:
        server(tinyxml2::XMLElement *config);
    };
}



#endif // SERVER_HPP