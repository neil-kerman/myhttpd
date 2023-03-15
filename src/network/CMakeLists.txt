cmake_minimum_required(VERSION 3.0.0)

set(SRC_LIST)

file(GLOB SRC_LIST "*.cpp")

add_library(network STATIC ${SRC_LIST})

target_link_libraries(
    network
    ${BOOST_LIBRARIES}
    glog::glog
    tinyxml2::tinyxml2
    OpenSSL::SSL
    OpenSSL::Crypto
)