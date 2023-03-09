cmake_minimum_required(VERSION 3.0.0)
project(myhttpd VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/DEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/RELEASE")

find_package(Boost COMPONENTS system context regex filesystem)
find_package(openssl)
find_package(glog CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)


configure_file(src/config.hpp.in config.hpp)
include_directories(
    ${PROJECT_BINARY_DIR} 
    "./src"
)

set(SRC_LIST)
file(GLOB_RECURSE SRC_LIST "./src/*.cpp")

link_libraries(
    ${Boost_LIBRARIES}
    glog::glog
    gflags::gflags
    tinyxml2::tinyxml2
    OpenSSL::SSL
    OpenSSL::Crypto
)


add_executable(${PROJECT_NAME} ${SRC_LIST})

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/resource/default.config
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${PROJECT_NAME}.config
)
endif()

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/resource/default.config
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.config
)
endif()

include(CTest)

enable_testing()

find_package(gtest CONFIG REQUIRED)

add_subdirectory(test)