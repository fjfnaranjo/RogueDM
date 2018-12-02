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
 * \file main.cpp
 * \brief File containing the application entry point.
 *
 * This file only holds the C++ main() function and delegates all the control
 * to a class. This emulates how Java's main() method force the class
 * paradigm even on application start.
 */

#include <cstdlib>

#include "roguedm/RogueDM.hpp"

/**
 * \brief Common C++ application entry point.
 *
 * The main method only loads the initial class of the application, named
 * RogueDM. RogueDM will do the arguments parsing, the locale initialization
 * and the creation of the game main state machine.
 */
int main(int argc, char *argv[]) {

  roguedm::RogueDM app = roguedm::RogueDM();
  int exitcode = app.run(argc, argv);
  exit(exitcode);

}
