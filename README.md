# myhttpd
My graduation design, a simple toy web server based on asio.  
## Dependent libraries
- boost.asio[ssl]
- boost.filesystem
- boost.interprocess
- boost.uuid
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
2. CGI
3. MQTT
4. FastCGI
