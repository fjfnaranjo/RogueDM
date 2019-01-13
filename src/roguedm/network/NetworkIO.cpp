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

#include "NetworkIO.hpp"

#include <locale>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "../gettext.hpp"
#include "../strings.hpp"

namespace roguedm {

// Set up networking.
NetworkIO::NetworkIO() {

  // Scope configuration to check if network must be disabled.
  configuration = Config::instance();
  errorCode = 0;

  if(configuration->getDoNotUseNetworking()) {
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
NetworkIO::~NetworkIO() {

  // Do NOT close SDLNet if networking is disabled.
  if(configuration->getDoNotUseNetworking())
    return;

  if(0==errorCode)
    SDLNet_Quit();

};

int NetworkIO::getErrorCode() {
  return errorCode;
}

void NetworkIO::update() {}
int NetworkIO::processCommand(const Sentence& a) {
  return 0;
}
const int NetworkIO::autocomplete(Sentence& a) {
  return 0;
}
const SentenceListReference NetworkIO::autocompleteListOptions(
  const Sentence& a
) {
  return std::make_shared<SentenceList>();
}

} // namespace roguedm
