# myhttpd
My graduation design, a simple toy web server based on asio.
WSGI application can run on this server.
## Dependent libraries
- boost
- OpenSSL
- Python3
- tinyxml2
- glog
## How to build
Before build this project, make sure you have installed: latest version of CMake, a C++20 supported compiler and a python interpreter. 

Download the source code and get into its directory, 
then ues commands below:
```
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
## TODO list
1. Complete GET/POST method
