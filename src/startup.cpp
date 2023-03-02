#include "server.hpp"

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <tinyxml2.h>

void glog_init(const char *arg) {
    google::InitGoogleLogging(arg);
    FLAGS_alsologtostderr = 1;
}

int main(int, char* argv[]) {
    glog_init(argv[0]);
    tinyxml2::XMLDocument config_file;
    config_file.LoadFile("myhttpd.config");
    auto config = config_file.RootElement();
    auto server_cfg = config->FirstChildElement("myhttpd-config");
    myhttpd::server _server(server_cfg);
    _server.start();
    _server.event_loop();
}