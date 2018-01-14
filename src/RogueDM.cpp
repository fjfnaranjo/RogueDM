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
 * \file RogueDM.cpp
 * \brief File containing the main application class (RogueDM) definitions.
 */

#include "RogueDM.hpp"

namespace roguedm {

RogueDM::RogueDM(int argc, char *argv[]) {

  // Basic app execution return code.
  status = 0;

  // Flag to avoid the main program execution (because of certain params).
  int doJump = 0;

  // I18N and UTF8 support (gettext - gnu i18n).
  setlocale(LC_ALL, "");

  // Configuration
  Config *configuration = Config::instance(RDM_CONFIG_MODE_CREATE);

  // Prints usage and/or version information when requested (gnu compliant).
  for(int c = argc; c > 1 && !doJump; c--) {

    if(!strcmp(argv[c-1], RDM_STR_USAGE_VERSION)) {

      printf(_(RDM_STR_VERSION));
      printf("\n");
      doJump = 1;

    } else if(!strcmp(argv[c-1], RDM_STR_USAGE_HELP)) {

      printf(_(RDM_STR_VERSION));
      printf("\n");
      printf(_(RDM_STR_USAGE), argv[0]);
      printf("\n");
      doJump = 1;

    } else if (!strcmp(argv[c-1], RDM_STR_USAGE_LOCAL)) {

      configuration->setDoNotUseNetworking(1);

    } else {

      printf(_(RDM_STR_USAGE_UKNOWN), argv[c-1]);

    }

  }

  if(!doJump) {

    // Prepare and exec the game state machine.
    StateInterface *initState = new InitialState();
    StateMachine *gameSM = new StateMachine(initState);
    gameSM->start();

    // The status from machine will be used by main to get the app return code.
    status = gameSM->getStatus();

    delete gameSM;

  }

  configuration = Config::instance(RDM_CONFIG_MODE_DELETE);

}

RogueDM::~RogueDM() {}

int RogueDM::getStatus() {
  return status;
}

} // namespace roguedm
