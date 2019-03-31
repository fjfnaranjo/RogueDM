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

project(roguedm VERSION 1.0 LANGUAGES CXX)

add_compile_options(-std=c++14)

IF (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
ENDIF (NOT WIN32)

IF (PYTHON_EXECUTABLE)
  set(CMAKE_EXE_LINKER_FLAGS "-ldl -lm -lutil")
ENDIF (PYTHON_EXECUTABLE)

# Verbose version (also update in project above)
set(roguedm_VERSION_MAJOR 1)
set(roguedm_VERSION_MINOR 0)
set(roguedm_VERSION_PRE a1)
set(roguedm_VERSION_DEV dev0)
configure_file("${roguedm_SOURCE_DIR}/roguedm/version.hpp.in"
               "${roguedm_BINARY_DIR}/roguedm/version.hpp")

# Basic directories sensible to platform
set(roguedm_CONFIG_DIRECTORY env/config CACHE PATH "User configuration")
set(roguedm_SHARE_DIRECTORY env/share CACHE PATH "Application data")
configure_file("${roguedm_SOURCE_DIR}/roguedm/paths.hpp.in"
               "${roguedm_BINARY_DIR}/roguedm/paths.hpp")


# Include parametrized sources
include_directories(BEFORE ${roguedm_BINARY_DIR}/src)

# Sources selection
file(GLOB_RECURSE roguedm_SOURCES "roguedm/*.cpp")
file(GLOB_RECURSE roguedm_SOURCES_MAIN "roguedm/main/*.cpp")
add_library (roguedm_lib ${roguedm_SOURCES} ${roguedm_SOURCES_MAIN})
add_executable(roguedm_app main_python.cpp)

# Dynamic libraries and linking
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${roguedm_SOURCE_DIR}/../cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(PythonLibs REQUIRED Development)
include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR}
                    ${SDL2_NET_INCLUDE_DIR}
                    ${PYTHON_INCLUDE_DIR})
target_link_libraries(roguedm_lib ${SDL2_LIBRARY}
                                  ${SDL2_IMAGE_LIBRARIES}
                                  ${SDL2_TTF_LIBRARIES}
                                  ${SDL2_NET_LIBRARIES}
                                  ${PYTHON_LIBRARY})
target_link_libraries(roguedm_app roguedm_lib)

if(ROGUEDM_TESTING)
  add_subdirectory(roguedm_test)
endif()
