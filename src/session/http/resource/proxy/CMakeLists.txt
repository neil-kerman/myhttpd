cmake_minimum_required(VERSION 3.18)

file(GLOB proxy "*.cpp")
add_library(proxy STATIC ${sources})