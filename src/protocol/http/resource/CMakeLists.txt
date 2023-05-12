cmake_minimum_required(VERSION 3.18)

file(GLOB sources "*.cpp")
add_library(resource STATIC ${sources})

if(ENABLE_FILESYSTEM)
    
    message("filesystem enabled")
    add_subdirectory(filesystem)
    target_link_libraries(resource filesystem)
    target_compile_definitions(resource PRIVATE "ENABLE_FILESYSTEM=1")
    
else()
    
    message("filesystem disabled")
    target_compile_definitions(resource PRIVATE "ENABLE_FILESYSTEM=0")
endif()

if(ENABLE_PROXY)

    add_subdirectory(proxy)
    target_link_libraries(resource proxy)
    target_compile_definitions(resource PRIVATE "ENABLE_PROXY=1")

else()

    target_compile_definitions(resource PRIVATE "ENABLE_PROXY=0")
endif()

if(ENABLE_REVERS_PROXY)

    add_subdirectory(revers_proxy)
    target_link_libraries(resource revers_proxy)
    target_compile_definitions(resource PRIVATE "ENABLE_REVERS_PROXY=1")
    
else()

    target_compile_definitions(resource PRIVATE "ENABLE_REVERS_PROXY=0")
endif()

if(ENABLE_WSGI)

    message("wsgi enabled")
    add_subdirectory(wsgi)
    target_link_libraries(resource wsgi)
    target_compile_definitions(resource PRIVATE "ENABLE_WSGI=1")
    
else()

    message("wsgi disabled")
    target_compile_definitions(resource PRIVATE "ENABLE_WSGI=0")
endif()
