#include "server.hpp"
#include "http.hpp"

#include <glog/logging.h>
#include <gflags/gflags.h>

using namespace boost::asio::ip;

server::server(boost::property_tree::ptree config)
: _work_guard(this->_io.get_executor()) {
    this->_workers_num = config.get<int>("server.<xmlattr>.workers");
}

void server::run() {
    LOG(INFO) << "Server has started";
}
