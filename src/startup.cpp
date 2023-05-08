#include <tinyxml2.h>

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#endif

#include "server.hpp"

int main(int, char* argv[]) {

#ifdef WIN32

    HWND windowHandle = GetConsoleWindow();
    ShowWindow(windowHandle, SW_HIDE);

#endif

    tinyxml2::XMLDocument config_file;
    config_file.LoadFile("../config/myhttpd.config.xml");
    auto config = config_file.RootElement();
    myhttpd::server _server(config);
    _server.start();
    _server.join();
}