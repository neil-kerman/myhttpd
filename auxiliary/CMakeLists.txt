cmake_minimum_required(VERSION 3.18)

file(GLOB_RECURSE config "config/*")
file(GLOB_RECURSE web_root "web-root/*")
file(GLOB_RECURSE wsgi_root "wsgi-root/*")
#set(all_aux_files)
list(APPEND all_aux_files ${config})
list(APPEND all_aux_files ${web_root})
list(APPEND all_aux_files ${wsgi_root})

set(all_aux_files_output)
foreach(file_path ${all_aux_files})
    file(RELATIVE_PATH relative_file_path ${CMAKE_CURRENT_SOURCE_DIR} ${file_path})
    list(APPEND all_aux_files_output ${home_dir}/${relative_file_path})
    #string(REGEX REPLACE "(/)" "_" dep_name ${relative_file_path})
    #add_custom_target(${dep_name} DEPENDS ${home_dir}/${relative_file_path})
    #add_dependencies(auxiliary ${dep_name})
    
endforeach()

add_custom_target(auxiliary DEPENDS ${all_aux_files_output})

add_custom_command(
    OUTPUT
        ${all_aux_files_output}
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_CURRENT_SOURCE_DIR}/config 
        ${home_dir}/config
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_CURRENT_SOURCE_DIR}/web-root 
        ${home_dir}/web-root
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_CURRENT_SOURCE_DIR}/wsgi-root 
        ${home_dir}/wsgi-root
    COMMENT
        "Copy auxiliay files."
    DEPENDS
        ${all_aux_files}
    VERBATIM
)
