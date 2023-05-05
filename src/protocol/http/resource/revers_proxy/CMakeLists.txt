cmake_minimum_required(VERSION 3.18)

file(GLOB sources "*.cpp")
add_library(revers_proxy STATIC ${sources})