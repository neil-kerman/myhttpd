cmake_minimum_required(VERSION 3.0.0)

add_custom_target(auxiliary)

add_custom_command(
    TARGET auxiliary PRE_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_CURRENT_SOURCE_DIR}/config 
        ${home_dir}/config
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_CURRENT_SOURCE_DIR}/web-root 
        ${home_dir}/web-root
    COMMENT
        "Copy auxiliay files."
    VERBATIM
)