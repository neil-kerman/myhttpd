cmake_minimum_required(VERSION 3.18)

file(GLOB network_sources "*.cpp")
add_library(network STATIC ${network_sources})
unset(network_sources)

target_link_libraries(
    network
        OpenSSL::SSL 
        OpenSSL::Crypto
)