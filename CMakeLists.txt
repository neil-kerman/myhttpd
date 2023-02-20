cmake_minimum_required(VERSION 3.0.0)
project(myhttpd VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost COMPONENTS thread context)
find_package(glog CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(gtest CONFIG REQUIRED)

set(SRC_LIST)
file(GLOB SRC_LIST "src/*.cpp")

add_executable(${PROJECT_NAME} 
    ${SRC_LIST}
)

message("SRC: ${SRC_LIST}")

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    glog::glog
    gflags::gflags
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})