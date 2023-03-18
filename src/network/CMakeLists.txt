cmake_minimum_required(VERSION 3.0.0)

file(GLOB network_sources "*.cpp")
add_library(network STATIC ${network_sources})
unset(network_sources)
