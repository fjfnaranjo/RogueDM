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
 * \brief File containing the program entry point.
 *
 * This file only holds the C++ main() function and delegates all the control
 * to a class. This emulates how Java's main() method force the class
 * paradigm even on application start.
 */

#include "roguedm/Application.hpp"

/**
 * \brief Common C++ program entry point.
 *
 * Loads the initial class of the application and delegates the parsing of the
 * arguments to it.
 */
int main(int argc, char *argv[]) {

  roguedm::Application app = roguedm::Application();
  return app.run(argc, argv);

}
