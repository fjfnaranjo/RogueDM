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

namespace roguedm_main {

MainState::MainState() {
  status = 0;
}

MainState::~MainState() {}

roguedm::StateResponse MainState::execute() {

  // SDL init checks
  if(0!=SDL_Init(SDL_INIT_VIDEO)) {
    status = 1;
    SDL_LogError(
      SDL_LOG_CATEGORY_APPLICATION,
      _(RDM_STR_SDL_ERROR),
	  SDL_GetError()
	);
    return {status, RDM_STATE_NO_STATE};
  }

  Game *gameInstance = new Game();

  roguedm::IOLocal *ioLocalInstance =
    roguedm::IOLocal::instance(RDM_IOLOCAL_MODE_CREATE);
  int ioLocalErrorCode = ioLocalInstance->getErrorCode();
  if(0!=ioLocalErrorCode) {
    SDL_Quit();
    status = ioLocalErrorCode;
    return {status, RDM_STATE_NO_STATE};
  }

  roguedm::IORemote *ioRemoteInstance =
    roguedm::IORemote::instance(RDM_IOREMOTE_MODE_CREATE);
  int ioRemoteErrorCode = ioRemoteInstance->getErrorCode();
  if(0!=ioRemoteErrorCode) {
    SDL_Quit();
    status = ioRemoteErrorCode;
    return {status, RDM_STATE_NO_STATE};
  }


  int update;
  update = SDL_GetTicks();

  int done = 0;
  while(!done) {

    // input checking and scene drawing (game loop)
    if ((SDL_GetTicks() - update) > 33) {
      update = SDL_GetTicks();
      ioLocalInstance->update();
      // TODO: Check if remote and game instances should issue a 'done' also
      done = ioLocalInstance->mustHalt();
      ioRemoteInstance->update();
      gameInstance->update();
    }

  }

  //Deletes
  delete gameInstance;
  roguedm::IORemote::instance(RDM_IOREMOTE_MODE_DELETE);
  roguedm::IOLocal::instance(RDM_IOLOCAL_MODE_DELETE);

  // Quit SDL
  SDL_Quit();

  return {status, RDM_STATE_NO_STATE};

}

} // namespace roguedm_main
