cmake_minimum_required(VERSION 3.0.0)
project(myhttpd VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/DEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/RELEASE")

find_package(Boost COMPONENTS system context filesystem)
find_package(OpenSSL)
find_package(glog CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(gtest CONFIG REQUIRED)

add_subdirectory(src)
