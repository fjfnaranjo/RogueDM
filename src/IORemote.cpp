// Copyright 2010 Google Code project roguedm development team.

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
 * \file IORemote.cpp
 * \brief File containing the IORemote class definitions.
 */

#include "IORemote.hpp"

namespace roguedm {

// Set up networking.
IORemote::IORemote() {

  // Scope configuration to check if network must be disabled.
  configuration = Config::instance(RDM_CONFIG_MODE_CREATE);
  errorCode = 0;

  if(configuration->getDoNotUseNetworking()) {
    printf(_(RDM_STR_NOT_NETWORKING));
    return;
  }

  // Init SDL Net
  if(-1==SDLNet_Init()) {
    errorCode = 3;
    printf(_(RDM_STR_SDL_NET_ERROR), SDLNet_GetError());
  }

};

// Set down networking.
IORemote::~IORemote() {

  // Do NOT close SDLNet if networking is disabled.
  if(configuration->getDoNotUseNetworking())
    return;

  if(0==errorCode)
    SDLNet_Quit();

};

// Singleton instance manager.
IORemote* IORemote::instance(int instanceMode) {

  static IORemote *instance;

  if(instance) {
    switch(instanceMode) {
      case RDM_IOREMOTE_MODE_RESET:
        delete instance;
        instance = new IORemote();
        break;
      case RDM_IOREMOTE_MODE_DELETE:
        delete instance;
      case RDM_IOREMOTE_MODE_CREATE:
        break;
      default:
        return 0;
        break;
    }
  } else {
    switch(instanceMode) {
      case RDM_IOREMOTE_MODE_CREATE:
        instance = new IORemote();
        break;
      case RDM_IOREMOTE_MODE_DELETE:
      case RDM_IOREMOTE_MODE_RESET:
      default:
        return 0;
        break;
    }
  }

  return instance;

}

int IORemote::getErrorCode() {
  return errorCode;
}

void IORemote::update() {}
int IORemote::processCommand(const Sentence& a) {
  return 0;
}
const int IORemote::autocomplete(Sentence& a) {
  return 0;
}
const std::vector<Sentence>* IORemote::autocompleteListOptions(
  const Sentence& a
) {
  return new std::vector<Sentence>;
}

} // namespace roguedm
