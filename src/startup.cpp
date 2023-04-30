#include <glog/logging.h>
#include <tinyxml2.h>
#include <boost/filesystem.hpp>

#include "server.hpp"

void glog_init(const char *arg) {

    google::InitGoogleLogging(arg);
}

int main(int, char* argv[]) {

    glog_init(argv[0]);
    FLAGS_logtostdout = 1;
    tinyxml2::XMLDocument config_file;
    config_file.LoadFile("../config/myhttpd.config.xml");
    auto config = config_file.RootElement();
    myhttpd::server _server(config);
    _server.start();
    _server.join();
}