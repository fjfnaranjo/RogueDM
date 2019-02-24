// Copyright 2018 Fco. Jose Fdez. Naranjo AKA github.com/fjfnaranjo

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

/**
 * \file mbtools.hpp
 * \brief Functions to handle std::string objects with UTF8 encoded text.
 */

#pragma once

#include <string>

namespace roguedm_gui {

/**
 * Get the total character length for a std::string having the current
 * multibyte context into account.
 *
 * \return Count of characters or the error value returned by mbrtowc in its
 *         last failed operation.
 */
std::size_t multibyteLenght(const std::string &string);

/**
 * Get a character by index in a std::string having the current multibyte
 * context into account.
 *
 * \return A single character or an empty string if something failed.
 */
std::string multibyteCharacterByIndex(const std::string &string, const std::size_t position);

/**
 * Get a substr by index and size in a std::string having the current multibyte
 * context into account.
 *
 * \return The substr or an empty string if something failed.
 */
std::string multibyteSubstr(const std::string &string, const std::size_t start, const std::size_t size);

} // namespace roguedm_gui
