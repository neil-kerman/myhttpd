cmake_minimum_required(VERSION 3.0.0)

set(sources)
file(GLOB sources "*.cpp")
add_library(http STATIC ${sources})