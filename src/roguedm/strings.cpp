// Copyright 2022 Fco. Jose Fdez. Naranjo AKA github.com/fjfnaranjo

// This file is part of RogueDM.
//
// RogueDM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RogueDM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RogueDM.  If not, see <http://www.gnu.org/licenses/>.

#include "strings.hpp"

#include <cstdarg>
#include <string>

#define RDM_STR_MAX 255

namespace roguedm {

std::string format_string(const char* format, ...) {
  char buffer[RDM_STR_MAX];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, RDM_STR_MAX, format, args);
  va_end(args);
  return std::string(buffer);
}

}  // namespace roguedm
