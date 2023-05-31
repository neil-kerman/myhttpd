cmake_minimum_required(VERSION 3.18)

file(GLOB sources "*.cpp")
add_library(timer STATIC ${sources})