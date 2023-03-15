cmake_minimum_required(VERSION 3.0.0)

set(SRC_LIST)

file(GLOB SRC_LIST "*.cpp")

add_library(http STATIC ${SRC_LIST})

target_link_libraries(
    http
    ${BOOST_LIBRARIES}
    glog::glog
    tinyxml2::tinyxml2
)