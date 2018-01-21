# Copyright 2018 Fco. Jose Fdez. Naranjo AKA github.com/fjfnaranjo

# This file is part of RogueDM.
#
# RogueDM is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RogueDM is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with RogueDM.  If not, see <http://www.gnu.org/licenses/>.

# CMake version check
cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

# Project basic definitions
project(roguedm VERSION 1.0 LANGUAGES C CXX)
add_compile_options(-std=c++11)

# Verbose version
set(roguedm_VERSION_MAJOR 1)
set(roguedm_VERSION_MINOR 0)
set(roguedm_VERSION_PRE a1)
set(roguedm_VERSION_DEV dev0)

# Pass information to code
configure_file("${roguedm_SOURCE_DIR}/src/version.hpp.in"
               "${roguedm_BINARY_DIR}/src/version.hpp")
include_directories(BEFORE ${roguedm_BINARY_DIR}/src)

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