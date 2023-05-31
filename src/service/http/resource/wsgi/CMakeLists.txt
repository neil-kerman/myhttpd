cmake_minimum_required(VERSION 3.18)

file(GLOB sources "*.cpp")
add_library(wsgi STATIC ${sources})

target_link_libraries(
    wsgi 
    Python3::Python
    Python3::Module
)