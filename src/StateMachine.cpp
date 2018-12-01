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
 * \file StateMachine.cpp
 * \brief File used to hold the StateMachine class definitions.
 */

#include "StateMachine.hpp"

#include <memory>

#include "InitialState.hpp"

namespace roguedm {

StateMachine::StateMachine() {
  lastResponse = {0, RDM_STATE_NO_STATE};
  nextState = std::make_shared<InitialState>();
}

StateMachine::StateMachine(StateInterfaceReference firstState) {
  lastResponse = {0, RDM_STATE_NO_STATE};
  nextState = std::move(firstState);
}

// Iterate through states until one of them return RDM_STATE_NO_STATE.
// The value returned by the states is taken as the next desired state.
// Also fetch the state result code for application returned value.
int StateMachine::run() {
  do {
    lastResponse = nextState->execute();
    nextState = lastResponse.nextState;
  } while (RDM_STATE_NO_STATE!=nextState);
  return lastResponse.status;
}

} // namespace roguedm
