cmake_minimum_required(VERSION 3.18)
project(myhttpd VERSION 0.1.0)

option(BUILD_UNIT_TESTS OFF)
option(PERFORMANCE_LOGGING ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS ON)

if(PERFORMANCE_LOGGING)
    add_compile_definitions(PERFORMANCE_LOGGING)
endif()

if(CMAKE_BUILD_TYPE)
    set(build_type)
    string(TOUPPER ${CMAKE_BUILD_TYPE} build_type)
    set(runtime_output_dir ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
    set(home_dir ${runtime_output_dir}/..)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${build_type} ${runtime_output_dir})
    unset(build_type)
endif()

if(WIN32)
    add_compile_definitions(WIN32)
elseif(LINUX)
    add_compile_definitions(LINUX)
elseif(UNIX)
    add_compile_definitions(UNIX)
endif()

if(MSVC)
    add_compile_options(-D_WIN32_WINNT=0x0601)
endif()

set(Python3_USE_STATIC_LIBS FALSE)

find_package(OpenSSL REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development Development.Module Development.Embed REQUIRED)
find_package(Boost COMPONENTS context filesystem python310 REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)

if(BUILD_UNIT_TESTS)
    find_package(gtest CONFIG REQUIRED)
    enable_testing()
    include(CTest)
endif()

add_custom_target(myhttpd ALL)

add_subdirectory(src)
add_subdirectory(auxiliary)

add_dependencies(myhttpd server)
add_dependencies(myhttpd auxiliary)