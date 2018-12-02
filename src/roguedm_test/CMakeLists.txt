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

add_subdirectory("${roguedm_SOURCE_DIR}/roguedm_test/googletest/"
                 "${roguedm_BINARY_DIR}/roguedm_test/googletest-build")

add_executable (roguedm_test test.cpp)

target_link_libraries(roguedm_test roguedm_lib gtest)

enable_testing()
add_test(NAME RogueDMTests COMMAND roguedm_test)

