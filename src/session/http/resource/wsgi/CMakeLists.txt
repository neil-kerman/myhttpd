cmake_minimum_required(VERSION 3.18)

file(GLOB sources "*.cpp")
add_library(wsgi STATIC ${sources})
find_package(Python3 COMPONENTS Interpreter Development Development.Module Development.Embed REQUIRED)
find_package(Boost COMPONENTS python310 REQUIRED)
target_link_libraries(
    wsgi
    Python3::Python
    Python3::Module
)