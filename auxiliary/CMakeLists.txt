cmake_minimum_required(VERSION 3.0.0)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/myhttpd.config 
    DESTINATION ${runtime_output_dir}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/web-root 
    DESTINATION ${home_dir}
)