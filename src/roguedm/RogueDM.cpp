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
 * \file RogueDM.cpp
 * \brief File containing the main application class (RogueDM) definitions.
 */

#include "RogueDM.hpp"

#include <cstdio>
#include <cstring>
#include <locale>

#include "gettext.hpp"
#include "strings.hpp"
#include "Config.hpp"
#include "StateMachine.hpp"

namespace roguedm {

int RogueDM::run(int argc, char *argv[]) {

  // Basic application return code.
  int status = 0;

  // Flag to abort the main program launch.
  bool abortLaunch = false;

  // I18N and UTF8 support (gettext - GNU I18N).
  setlocale(LC_ALL, "");

  // Load the configuration.
  ConfigReference configuration = Config::instance();

  // Prints usage and/or version information when requested (GNU compliant).
  for(int c = argc; c > 1 && !abortLaunch; c--) {

    if(!strcmp(argv[c-1], RDM_STR_USAGE_VERSION)) {

      printf(_(RDM_STR_VERSION_STRING));
      printf(_(RDM_STR_VERSION_FULL));
      printf("\n");
      abortLaunch = true;

    } else if(!strcmp(argv[c-1], RDM_STR_USAGE_HELP)) {

      printf(_(RDM_STR_VERSION_STRING));
      printf(_(RDM_STR_VERSION_FULL));
      printf("\n");
      printf(_(RDM_STR_USAGE), argv[0]);
      printf("\n");
      abortLaunch = true;

    } else if (!strcmp(argv[c-1], RDM_STR_USAGE_LOCAL)) {

      configuration->setDoNotUseNetworking(true);

    } else {

      printf(_(RDM_STR_USAGE_UKNOWN), argv[c-1]);
      abortLaunch = true;

    }

  }

  if(!abortLaunch) {

    // Execute the main state machine.
    StateMachine gameSM = StateMachine();
    status = gameSM.run();

  }

  return status;

}

} // namespace roguedm
