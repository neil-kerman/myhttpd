cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(network)

add_subdirectory(session)

add_subdirectory(exception)

add_subdirectory(util)

add_subdirectory(const)

configure_file(config.hpp.in config.hpp)

add_executable(${PROJECT_NAME} "startup.cpp" "server.cpp")

target_link_libraries(
    ${PROJECT_NAME}
    ${Boost_LIBRARIES}
    network
    http
    const
    glog::glog
    gflags::gflags
    tinyxml2::tinyxml2
)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/default.config
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${PROJECT_NAME}.config
)
endif()

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/default.config
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.config
)
endif()