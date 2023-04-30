#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <tinyxml2.h>

#include "network/acceptor.hpp"
#include "network/server.hpp"
#include "worker.hpp"

namespace myhttpd {

    class server: public network::server {

    private:
        boost::asio::io_context _ctx;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;

        std::unique_ptr<std::thread> _thread = nullptr;

        std::list<std::unique_ptr<network::acceptor>> _acceptors;

        std::list<std::unique_ptr<worker>> _workers;

    private:
        void _init_acceptors(tinyxml2::XMLElement* config);

    public:
        virtual void pass_connection(std::unique_ptr<network::connection> conn);

    public:
        void start();
    
        void join();

    public:
        server(tinyxml2::XMLElement *config);
    };
}



#endif // SERVER_H