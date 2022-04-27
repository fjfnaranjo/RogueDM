# Copyright 2022 Fco. Jose Fdez. Naranjo AKA github.com/fjfnaranjo

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

# Generated version string header
set(${PROJECT_NAME}_VERSION 0.1.dev.0)
configure_file("${${PROJECT_NAME}_SOURCE_DIR}/src/roguedm/version.hpp.in"
               "${${PROJECT_NAME}_BINARY_DIR}/generated/version.hpp")

# Generated platform dependant directories header
set(${PROJECT_NAME}_CONFIG_DIRECTORY env/config CACHE PATH "User configuration")
set(${PROJECT_NAME}_SHARE_DIRECTORY env/share CACHE PATH "Application data")
configure_file("${${PROJECT_NAME}_SOURCE_DIR}/src/roguedm/paths.hpp.in"
               "${${PROJECT_NAME}_BINARY_DIR}/generated/paths.hpp")

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "*.cpp")
add_library(roguedm_lib ${${PROJECT_NAME}_SOURCES})

# Include generated sources
target_include_directories(roguedm_lib BEFORE PRIVATE
                           "${${PROJECT_NAME}_BINARY_DIR}/generated")

# Dynamic libraries and linking
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(Lua53 REQUIRED)
target_include_directories(roguedm_lib PUBLIC
	                       ${SDL2_INCLUDE_DIR}
                           ${SDL2_IMAGE_INCLUDE_DIR}
                           ${SDL2_TTF_INCLUDE_DIR}
                           ${SDL2_NET_INCLUDE_DIR}
                           ${LUA_INCLUDE_DIR})
target_link_libraries(roguedm_lib ${SDL2_LIBRARY}
                                  ${SDL2_IMAGE_LIBRARIES}
                                  ${SDL2_TTF_LIBRARIES}
                                  ${SDL2_NET_LIBRARIES}
                                  ${LUA_LIBRARIES})
