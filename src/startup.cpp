#include "server.hpp"

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <tinyxml2.h>

int main(int, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_alsologtostderr = 1;
    LOG(INFO) << "Start.";
    tinyxml2::XMLDocument config_file;
    config_file.LoadFile("myhttpd.config");
    auto config = config_file.RootElement();
    auto server_cfg = config->FirstChildElement("server");
    server _server(server_cfg);
    _server.run();
    _server.join();
}