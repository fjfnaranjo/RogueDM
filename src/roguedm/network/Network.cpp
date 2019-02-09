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

Network::Network() {
  initSuccess = false;
  config = Config::instance();
}

Network::~Network() {
  if(initSuccess)
    SDLNet_Quit();
};

bool Network::initNetwork() {
  if(-1==SDLNet_Init()) {
    SDL_Log(_(RDM_STR_SDL_NET_ERROR), SDLNet_GetError());
    return false;
  }
  initSuccess = true;
  return true;
};

void Network::update() {}

int Network::processCommand(const Sentence& a) {
  return RDM_COMMAND_UNKNOWN;
}

int Network::autocomplete(Sentence& a) const {
  return RDM_COMMAND_AC_NEXT;
}

SentenceListReference Network::autocompleteListOptions(
  const Sentence& a
) const {
  return std::make_shared<SentenceList>();
}

} // namespace roguedm
