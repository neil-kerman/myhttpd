cmake_minimum_required(VERSION 3.0.0)

file(GLOB http_sources "*.cpp")
add_library(http STATIC ${http_sources})
unset(http_sources)