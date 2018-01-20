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

#ifndef ROGUEDM_HPP
#define ROGUEDM_HPP

// Gettext alias and predefined strings for CLI interface and game.
#include "strings.hpp"

// Game state machine related includes
#include "StateMachine.hpp"
#include "InitialState.hpp"

#include "Config.hpp"

#include <cstdio>
#include <cstring>

namespace roguedm {

/**
 * \brief Application management class.
 *
 * This class is used to manage the program arguments and all the generic steps
 * an application need to manage (sets I18N, ...).
 */
class RogueDM {

  public:

    /**
     * This constructor handles base application related tasks like the program
     * arguments. Also call the state machine and lets it to manage the rest.
     * \param argc Number of program arguments (copy from main()).
     * \param argv Value of program arguments (copy from main()).
     */
    RogueDM(int argc, char *argv[]);

    /**
     * Default destructor (project guidelines requires always a destructor,
     * even if it will be empty).
     */
    virtual ~RogueDM();

    /**
     * Get the right application exit status code.
     * \return Status code to be returned with cstdlib exit().
     */
    int getStatus();

  private:

    /** Copy operator (private because is disabled by default). */
    RogueDM(const RogueDM&);

    /** Assing operator (private because is disabled by default). */
    void operator=(const RogueDM&);

    /** Member to store the desired application exit code. */
    int status;

};

} // namespace roguedm

#endif // ROGUEDM_HPP
