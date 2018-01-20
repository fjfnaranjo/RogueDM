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
 * \file StateInterface.hpp
 * \brief File used to contain the StateInterface interface.
 */

#ifndef STATEINTERFACE_HPP
#define STATEINTERFACE_HPP

#define RDM_STATE_NO_STATE 0

namespace roguedm {

/**
 * \interface StateInterface
 * \brief C++ interface for the state machine different states.
 *
 * This is the interface that every class must extends if it want to
 * work as state to StateMachine class.
 */
class StateInterface
{

  public:

    /** Contains the state relevant processes. */
    virtual StateInterface* execute() =0;

    /** Get the right application exit status code. */
    virtual int getStatus() =0;

  protected:

    /** Cleared constructor to avoid interface use as a class. */
    StateInterface() {};

  private:

    /** Copy operator (private because is disabled by default). */
    StateInterface(const StateInterface&);

    /** Assing operator (private because is disabled by default). */
    void operator=(const StateInterface&);

};

} // namespace roguedm

#endif // STATEINTERFACE_HPP
