#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include <map>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <tinyxml2.h>

#include "acceptor.hpp"
#include "session.hpp"
#include "http/resource.hpp"

namespace myhttpd {
    class server {
    private:
        boost::asio::io_context _io;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;
        std::vector<std::unique_ptr<acceptor>> _acceptors;
        std::map<uuid, std::unique_ptr<session>> _sessions;
        boost::mutex _sessions_mtx;
        boost::thread_group _workers;
        int _workers_num;
        boost::property_tree::ptree _config;
    private:
        void _worker_func();
    public:
        server(tinyxml2::XMLElement *config);
        void run();
        void join();
    };
}


#endif // __SERVER_H__