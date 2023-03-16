cmake_minimum_required(VERSION 3.0.0)
project(myhttpd VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_UNIT_TESTS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/DEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/RELEASE")

find_package(Boost COMPONENTS system context filesystem)
find_package(OpenSSL)
find_package(glog CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)

if(BUILD_UNIT_TESTS)
    find_package(gtest CONFIG REQUIRED)
    enable_testing()
    include(CTest)
endif()

add_subdirectory(src)
