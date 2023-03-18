cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

link_libraries(
    ${Boost_LIBRARIES}
    glog::glog
    gflags::gflags
    tinyxml2::tinyxml2
    OpenSSL::SSL 
    OpenSSL::Crypto
)

configure_file(config.hpp.in config.hpp)

file(GLOB server_sources "./*.cpp")
add_executable(server ${server_sources})
unset(server_sources)

add_subdirectory(network)
add_subdirectory(session)
add_subdirectory(util)
add_subdirectory(const)

target_link_libraries(
    server
        network
        http
        const
)

