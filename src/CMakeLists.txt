cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

link_libraries(
    ${Boost_LIBRARIES}
    glog::glog
    gflags::gflags
    tinyxml2::tinyxml2
    OpenSSL::SSL 
    OpenSSL::Crypto
)

add_subdirectory(network)
add_subdirectory(session)
add_subdirectory(exception)
add_subdirectory(util)
add_subdirectory(const)

configure_file(config.hpp.in config.hpp)

set(sources)
file(GLOB sources "*.cpp")
add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(
    ${PROJECT_NAME}
    network
    http
    const
)

if(runtime_output_dir)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/default.config
                ${runtime_output_dir}/${PROJECT_NAME}.config
    )
endif()