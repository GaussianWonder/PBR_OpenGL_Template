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

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "")

add_definitions("-std=c++20")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -fcoroutines -fconcepts")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES glfw/3.3.6 GENERATORS CMakeDeps)
conan_cmake_configure(REQUIRES opengl/system GENERATORS CMakeDeps)
conan_cmake_configure(REQUIRES glew/2.2.0 GENERATORS CMakeDeps)
conan_cmake_configure(REQUIRES glm/0.9.9.5 GENERATORS CMakeDeps)
conan_cmake_configure(REQUIRES ms-gsl/3.1.0 GENERATORS CMakeDeps)
conan_cmake_configure(REQUIRES spdlog/1.9.2 GENERATORS CMakeDeps)
conan_cmake_configure(REQUIRES tinyobjloader/1.0.6 GENERATORS CMakeDeps)
conan_cmake_configure(REQUIRES stb/cci.20210713 GENERATORS CMakeDeps)
conan_cmake_configure(REQUIRES imgui/1.86 GENERATORS CMakeDeps)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE . BUILD missing SETTINGS ${settings})

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

# Get all *.c files
file(GLOB_RECURSE Sources RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE SBindings RELATIVE ${CMAKE_SOURCE_DIR} "bindings/*.cpp")

# Get all *.h files
file(GLOB_RECURSE Headers *.h)

# Add everything to executable
add_executable(${PROJECT_NAME} ${Headers} main.cpp ${Sources} ${SBindings})

# Get all header dirs to include
header_directories(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${header_dir_list}
)

# Packages
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

find_package(opengl CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC opengl::opengl)

find_package(GLEW CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC GLEW::GLEW)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

# Guideline support library
# find_package(gsl REQUIRED)
# target_link_libraries(${PROJECT_NAME} gsl)

# Fast and Reliable logging
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

# Tiny object loader
find_package(tinyobjloader CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC tinyobjloader)

# STB
find_package(stb CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC stb::stb)

# ImGUI
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
