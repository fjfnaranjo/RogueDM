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

#include <SDL.h>
#include <SDL_net.h>

#include "../strings.hpp"

namespace roguedm {

Network::Network() {
  initSuccess = false;
  config = Config::instance();
}

Network::~Network() {
  if(initSuccess)
    SDLNet_Quit();
}

bool Network::initNetwork() {
  if(-1==SDLNet_Init()) {
    SDL_Log(RDM_STR_SDL_NET_ERROR, SDLNet_GetError());
    return false;
  }
  initSuccess = true;
  return true;
}

void Network::update() {}

bool Network::processCommand(const Command&) {
  return RDM_CMD_PROCESS_UNKNOWN;
}

bool Network::identifyCommand(const Sentence&, Command&) const {
  return RDM_CMD_IDENTIFY_UNKNOWN;
}

CommandList Network::getCompletionCandidates(
  const Command&
) const {
  return CommandList();
}

} // namespace roguedm
