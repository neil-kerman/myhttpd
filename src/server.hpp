#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include <memory>
#include <set>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <tinyxml2.h>

#include "listener.hpp"
#include "session.hpp"
#include "resource.hpp"

class server {
private:
    boost::asio::io_context _io;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;
    std::vector<std::unique_ptr<listener>> _listeners;
    std::set<session> _sessions;
    boost::mutex _sessions_mtx;
    boost::thread_group _workers;
    int _workers_num;
    boost::property_tree::ptree _config;
private:
    void _worker_func();
    void _async_accept_handler(std::unique_ptr<listener> &lis, std::unique_ptr<connection> conn);
public:
    server(tinyxml2::XMLElement *config);
    void run();
    void join();
};
#endif // __SERVER_H__