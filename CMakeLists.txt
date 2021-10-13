# Project definition
cmake_minimum_required(VERSION 3.21)
project(opengl_template)

# Search all directories that contain headers
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# End of configuration
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_FLAGS "-Wall")

# Get all *.c files
FILE(GLOB_RECURSE Sources RELATIVE ${CMAKE_SOURCE_DIR} "sources/*.c")

# Get all *.h files
FILE(GLOB_RECURSE Headers *.h)

# Add everything to executable
add_executable(${PROJECT_NAME} ${Headers} main.cpp ${Sources})

# Get all header dirs to include
HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${header_dir_list} # Recursive
)

# Packages

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
