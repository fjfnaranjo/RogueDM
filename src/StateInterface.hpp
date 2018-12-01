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

#pragma once

// Useful macros.
#include "macros.hpp"

/**
 * \brief Value to report that no more states should be executed.
 */
#define RDM_STATE_NO_STATE nullptr

namespace roguedm {

// This is a forward declaration for usage in the StateResponse declaration.
class StateInterface;

/**
 * \struct StateResponse
 * \brief Response for states implementing the StateInterface.
 * \see StateInterface
 */
struct StateResponse {
  int status;
  StateInterface* nextState;
};

/**
 * \interface StateInterface
 * \brief Interface for the different states used by the StateMachine class.
 *
 * Every class must extends this interface if it wants to work as a state for
 * the StateMachine class.
 */
class StateInterface
{

  RDM_DECLARE_CLASS_AS_INTERFACE(StateInterface);

  public:

    /** The state main code. */
    virtual StateResponse execute() =0;

};

} // namespace roguedm
