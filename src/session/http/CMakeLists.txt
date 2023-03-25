cmake_minimum_required(VERSION 3.25)

file(GLOB http_sources "*.cpp")
add_library(http STATIC ${http_sources})
unset(http_sources)


target_link_libraries(
    http
    Python3::Python
    Python3::Module
)