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
 * \file RogueDM.hpp
 * \brief File containing the main application class (RogueDM) declarations.
 */

#pragma once

namespace roguedm {

/**
 * \brief Main application class.
 *
 * This class is used to parse the program arguments and do all the generic
 * steps expected for a CLI application launch (help text, I18N init, ...).
 */
class RogueDM {

  public:

    /**
     * Run the main application code.
     * \param argc Number of program arguments (copied from main()).
     * \param argv Value of program arguments (copied from main()).
     * \return Status code to be returned with exit().
     */
    int run(int argc, char *argv[]);

};

} // namespace roguedm
