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
 * \file MainState.cpp
 * \brief File containing the MainState class definitions.
 */

#include "MainState.hpp"

#include <cstdio>
#include <memory>
#include <locale>

#include "Game.hpp"
#include "../gettext.hpp"
#include "../strings.hpp"
#include "../Config.hpp"
#include "../IOLocal.hpp"
#include "../IORemote.hpp"

namespace roguedm_main {

MainState::MainState() {
  status = 0;
}

MainState::~MainState() {}

roguedm::StateResponse MainState::execute() {

  auto gameInstance = std::make_unique<Game>();

  auto ioLocalInstance = roguedm::IOLocal::instance();
  int ioLocalErrorCode = ioLocalInstance->getErrorCode();
  if(0!=ioLocalErrorCode) {
    status = ioLocalErrorCode;
    return {status, RDM_STATE_NO_STATE};
  }

  auto ioRemoteInstance = roguedm::IORemote::instance();
  int ioRemoteErrorCode = ioRemoteInstance->getErrorCode();
  if(0!=ioRemoteErrorCode) {
    status = ioRemoteErrorCode;
    return {status, RDM_STATE_NO_STATE};
  }

  // input checking and scene drawing (game loop)
  int done = 0;
  while(!done) {
    ioLocalInstance->update();
    done = ioLocalInstance->mustHalt();
    ioRemoteInstance->update();
    // TODO: Check if remote and game instances should issue a 'done' also
    // done = ioRemoteInstance->mustHalt();
    gameInstance->update();
    // done = gameInstance->mustHalt();
  }

  return {status, RDM_STATE_NO_STATE};

}

} // namespace roguedm_main
