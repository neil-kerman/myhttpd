cmake_minimum_required(VERSION 3.18)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

message("boost include dir: ${Boost_LIBRARIES}")

link_libraries(
    ${Boost_LIBRARIES}
    tinyxml2::tinyxml2
)

configure_file(config.hpp.in config.hpp)

file(GLOB server_sources "./*.cpp")
add_executable(server ${server_sources})
unset(server_sources)

add_dependencies(server auxiliary)

add_subdirectory(network)
add_subdirectory(protocol)

target_link_libraries(
    server
        network
        http
)

