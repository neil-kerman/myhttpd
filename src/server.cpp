#include "server.hpp"
#include "http.hpp"

#include <glog/logging.h>
#include <gflags/gflags.h>

using namespace boost::asio::ip;

server::server(tinyxml2::XMLElement *config)
: _work_guard(this->_io.get_executor()) {
    this->_workers_num = config->IntAttribute("workers");
    auto lis_cfgs = config->FirstChildElement("listeners");
    auto lis = lis_cfgs->FirstChildElement();
    //The memory of XMLElement objects are managed by the XMLDocument object which created themslevs,
    //Thus no need to delete it in this scope.
    while(lis) {
        this->_listeners.push_back(std::make_unique<listener>(this->_io, lis));
        lis = lis->NextSiblingElement();
    }
}

void server::run() {
    for(auto &lis: this->_listeners){
        
    }
    for(int i = 0; i < this->_workers_num; i++) {
        this->_workers.create_thread(std::bind(&server::_worker_func, this));
    }
}

void server::_worker_func() {
    LOG(INFO) << "A worker thread started";
    this->_io.run();
}

void server::join() {
    this->_workers.join_all();
}