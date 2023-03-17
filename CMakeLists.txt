cmake_minimum_required(VERSION 3.0.0)
project(myhttpd VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_UNIT_TESTS OFF)

if(CMAKE_BUILD_TYPE)
    set(build_type)
    string(TOUPPER ${CMAKE_BUILD_TYPE} build_type)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${build_type} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
    set(runtime_output_dir ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
endif()

if(MSVC)
    add_compile_options(-D_WIN32_WINNT=0x0601)
endif()

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
