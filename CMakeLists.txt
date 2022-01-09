# Project definition
cmake_minimum_required(VERSION 3.21)
project(opengl_template)

# Search all directories that contain headers
macro(HEADER_DIRECTORIES return_list)
  file(GLOB_RECURSE new_list *.h)
  set(dir_list "")
  foreach(file_path ${new_list})
    get_filename_component(dir_path ${file_path} PATH)
    set(dir_list ${dir_list} ${dir_path})
  endforeach()
  list(REMOVE_DUPLICATES dir_list)
  set(${return_list} ${dir_list})
endmacro()

# End of configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -fcoroutines -fconcepts")

# Get all *.c files
file(GLOB_RECURSE Sources RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# Get all *.h files
file(GLOB_RECURSE Headers *.h)

# Add everything to executable
add_executable(${PROJECT_NAME} ${Headers} main.cpp ${Sources})

# Get all header dirs to include
header_directories(header_dir_list)

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

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} glm)

# Guideline support library
find_package(GSL REQUIRED)
target_link_libraries(${PROJECT_NAME} GSL::gsl GSL::gslcblas)

# Fast and Reliable logging
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

# Uploading textures to OpenGL

find_library(SOIL2 soil2-debug)
# target_link_libraries(${PROJECT_NAME} ${SOIL2})