cmake_minimum_required(VERSION 3.18)

file(GLOB http_sources "*.cpp")
add_library(http STATIC ${http_sources})

add_subdirectory(const)
target_link_libraries(http const)

add_subdirectory(resource)
target_link_libraries(http resource)

add_subdirectory(security)
target_link_libraries(http security)

add_subdirectory(transceiver)
target_link_libraries(http transceiver)