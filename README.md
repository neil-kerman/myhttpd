# myhttpd
My graduation design, a simple web server based on asio.  
## Dependent libraries
- boost.asio[ssl]
- boost.filesystem
- tinyxml2
- glog  
## How to build
Download the source code and get into its directory, 
then run the command below: 
- `cmake -S . -B build -DBUILD_UNIT_TESTS=OFF`
- `cmake --build .`
