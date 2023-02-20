#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <set>

#include "listener.hpp"
#include "session.hpp"
#include "resource.hpp"

class server {
private:
    boost::asio::io_context _io;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;
    std::vector<std::unique_ptr<listener>> _listeners;
    boost::thread_group _workers;
    int _workers_num;
    boost::property_tree::ptree _config;
public:
    server(boost::property_tree::ptree config);
    void run();
};
#endif // __SERVER_H__