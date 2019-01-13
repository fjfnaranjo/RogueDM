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

#include "Main.hpp"

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

Main::Main() {
  status = 0;
}

Main::~Main() {}

roguedm::StageResponse Main::execute() {

  auto gameInstance = std::make_unique<Game>();

  auto ioLocalInstance = roguedm::IOLocal::instance();
  int ioLocalErrorCode = ioLocalInstance->getErrorCode();
  if(0!=ioLocalErrorCode) {
    status = ioLocalErrorCode;
    return {status, RDM_STAGE_EXIT};
  }

  auto ioRemoteInstance = roguedm::IORemote::instance();
  int ioRemoteErrorCode = ioRemoteInstance->getErrorCode();
  if(0!=ioRemoteErrorCode) {
    status = ioRemoteErrorCode;
    return {status, RDM_STAGE_EXIT};
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

  return {status, RDM_STAGE_EXIT};

}

} // namespace roguedm_main
