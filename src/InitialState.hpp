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
 * \file InitialState.hpp
 * \brief File containing the InitialState class declarations.
 */

#ifndef INITIALSTATE_HPP
#define INITIALSTATE_HPP

#include "StateInterface.hpp"
#include "main/MainState.hpp"

namespace roguedm {

/**
 * \brief This is the initial state class.
 *
 * Currently just a place holder only used to call the MainState state class.
 * \see StateInterface
 * \see StateInterface.execute()
 * \see MainState
 */
class InitialState : public StateInterface
{

  public:

    /**
     * Default constructor (project guidelines requires always a constructor,
     * even if it will be empty).
     */
    InitialState();

    /**
     * Default destructor (project guidelines requires always a destructor,
     * even if it will be empty).
     */
    virtual ~InitialState();

    /**
     * \brief This method hold the initial state tasks, currently only calls the
     *        MainState state class.
     *
     * Overrided from StateInterface.
     * \see StateInterface
     * \see StateInterface.execute()
     * \see MainState
     */
    StateInterface* execute();

    /**
     * Get the right state exit status code.
     * \return Status code to be returned with cstdlib exit().
     */
    int getStatus();

  private:

    /** Copy operator (private because is disabled by default). */
    InitialState(const InitialState&);

    /** Assing operator (private because is disabled by default). */
    void operator=(const InitialState&);

    /** Member to store the state desired application exit code. */
    int status;

};

} // namespace roguedm

#endif // INITIALSTATE_HPP
