cmake_minimum_required(VERSION 3.0.0)

if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/error_page")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/error_page")
endif()

execute_process(
    COMMAND "python"
    "${CMAKE_CURRENT_SOURCE_DIR}/error_page_gen.py" 
    "${CMAKE_CURRENT_SOURCE_DIR}/error_page_template.html" 
    "${CMAKE_CURRENT_BINARY_DIR}/error_page"
)
    
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/error_page_c")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/error_page_c")
endif()

file(GLOB error_pages "${CMAKE_CURRENT_BINARY_DIR}/error_page/*")

foreach(error_page ${error_pages})
    execute_process(
        COMMAND "python" 
        "${CMAKE_CURRENT_SOURCE_DIR}/to_c_array.py"
        "${error_page}"
        "${CMAKE_CURRENT_BINARY_DIR}/error_page_c"
    )
endforeach()

file(GLOB error_pages_c_arrey "${CMAKE_CURRENT_BINARY_DIR}/error_page_c/*.c")

add_library(const ${error_pages_c_arrey})