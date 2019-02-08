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

#include "Network.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "../strings.hpp"

namespace roguedm {

// Set up networking.
Network::Network() {

  // Scope configuration to check if network must be disabled.
  configuration = Config::instance();
  errorCode = 0;

  if(configuration->getSettingBoolValue("general", "skipNetworking", false)) {
    SDL_Log(_(RDM_STR_NOT_NETWORKING));
    return;
  }

  // Init SDL Net
  if(-1==SDLNet_Init()) {
    errorCode = 3;
    SDL_Log(_(RDM_STR_SDL_NET_ERROR), SDLNet_GetError());
  }

};

// Set down networking.
Network::~Network() {

  // Do NOT close SDLNet if networking is disabled.
  if(configuration->getSettingBoolValue("general", "skipNetworking", false))
    return;

  if(0==errorCode)
    SDLNet_Quit();

};

int Network::getErrorCode() {
  return errorCode;
}

void Network::update() {}
int Network::processCommand(const Sentence& a) {
  return RDM_COMMAND_UNKNOWN;
}
const int Network::autocomplete(Sentence& a) {
  return RDM_COMMAND_AC_NEXT;
}
const SentenceListReference Network::autocompleteListOptions(
  const Sentence& a
) {
  return std::make_shared<SentenceList>();
}

} // namespace roguedm
