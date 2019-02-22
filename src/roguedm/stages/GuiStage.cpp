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

#include "GuiStage.hpp"

#include <memory>

#include "../strings.hpp"
#include "../Config.hpp"
#include "../game/Game.hpp"
#include "../network/Network.hpp"
#include "../sdl2/Sdl2IO.hpp"

namespace roguedm {

GuiStage::GuiStage() {
  config = Config::instance();
  status = 0;
}

StageResponse GuiStage::execute() {

  bool networkingEnabled =
    !config->getSettingBoolValue("general", "skipNetworking", false);

  auto gameInstance = std::make_unique<roguedm_game::Game>();

  auto sdl2Io = std::make_unique<roguedm_gui::Sdl2IO>();
  if(!sdl2Io->initSdl2IO()) {
    status = RDM_ERR_SDL2_ERROR;
    return {status, RDM_STAGE_EXIT};
  }

  auto network = std::make_unique<Network>();
  if(networkingEnabled) {
    if(!network->initNetwork()) {
      status = RDM_ERR_NETWORK_ERROR;
      return {status, RDM_STAGE_EXIT};
    }
  } else {
    SDL_Log(RDM_STR_NOT_NETWORKING);
  }

  // input checking and scene drawing (game loop)
  int done = 0;
  while(!done) {
    sdl2Io->update();
    done = sdl2Io->mustHalt();
    if(networkingEnabled)
      network->update();
    // TODO: Check if remote and game instances should issue a 'done' also
    // done = NetworkInstance->mustHalt();
    gameInstance->update();
    // done = gameInstance->mustHalt();
  }

  return {status, RDM_STAGE_EXIT};

}

} // namespace roguedm
