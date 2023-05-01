#include <tinyxml2.h>
#include <boost/filesystem.hpp>

#include "server.hpp"

int main(int, char* argv[]) {

    tinyxml2::XMLDocument config_file;
    config_file.LoadFile("../config/myhttpd.config.xml");
    auto config = config_file.RootElement();
    myhttpd::server _server(config);
    _server.start();
    _server.join();
}