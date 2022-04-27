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

# General C++ features
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_EXTENSIONS OFF)
IF(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
ENDIF(NOT WIN32)

# Core lib
add_subdirectory(roguedm)

if(NOT BUILD_TESTING)

  # Main application executable
  add_executable(${PROJECT_NAME} main.cpp)
  target_link_libraries(${PROJECT_NAME} roguedm_lib)

else()

  # Testing executable
  add_subdirectory("${${PROJECT_NAME}_SOURCE_DIR}/ext/googletest/"
                   "${${PROJECT_NAME}_BINARY_DIR}/ext/googletest-build")
  add_executable(roguedm_test roguedm_test/test.cpp)
  target_link_libraries(roguedm_test roguedm_lib gtest)

endif()
