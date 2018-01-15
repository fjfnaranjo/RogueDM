# CMake version check
cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

# Project basic definitions
project(roguedm VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

# Verbose version
set(roguedm_VERSION_MAJOR 1)
set(roguedm_VERSION_MINOR 0)
set(roguedm_VERSION_PRE a1)
set(roguedm_VERSION_DEV dev0)

# Pass information to code
configure_file("${roguedm_SOURCE_DIR}/src/version.hpp.in"
               "${roguedm_BINARY_DIR}/src/version.hpp")

# Path for CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${roguedm_SOURCE_DIR}/cmake")

# Dynamic libraries 
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)

# Targets
file(GLOB_RECURSE roguedm_SOURCES "src/*.cpp")
add_executable(roguedm ${roguedm_SOURCES})
include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR}
                    ${SDL2_NET_INCLUDE_DIR})
target_link_libraries(roguedm ${SDL2_LIBRARY}
                              ${SDL2_IMAGE_LIBRARIES}
                              ${SDL2_TTF_LIBRARIES}
                              ${SDL2_NET_LIBRARIES})