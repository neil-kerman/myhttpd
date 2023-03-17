#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <tinyxml2.h>

#include "config.hpp"
#include "server.hpp"

void glog_init(const char *arg) {
    google::InitGoogleLogging(arg);
    FLAGS_alsologtostderr = 1;
}

int main(int, char* argv[]) {
    glog_init(argv[0]);
    tinyxml2::XMLDocument config_file;
    config_file.LoadFile("myhttpd.config");
    auto config = config_file.RootElement();
    myhttpd::server _server(config);
    _server.start();
    _server.event_loop();
}