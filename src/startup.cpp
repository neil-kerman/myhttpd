#include "server.hpp"

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <boost/property_tree/xml_parser.hpp>

int main(int, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_alsologtostderr = 1;
    LOG(INFO) << "Start.";
    using namespace boost::property_tree;
    ptree config;
    read_xml("myhttpd.config", config);
    server _server(config);
    _server.run();
}
