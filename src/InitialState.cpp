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
 * \file InitialState.cpp
 * \brief File containing the InitialState class definitions.
 */

// Game state machine related includes
#include "InitialState.hpp"

namespace roguedm {

InitialState::InitialState() {
  status = 0;
}

InitialState::~InitialState() {}

// Currently, this state only serve as a transition to the main state.
StateInterface* InitialState::execute() {
  return new roguedm_main::MainState();
}

int InitialState::getStatus() {
  return status;
}

} // namespace roguedm
