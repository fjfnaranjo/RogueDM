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

#include "Application.hpp"

#include <cstdio>
#include <cstring>
#include <locale>

#include "gettext.hpp"
#include "main/Main.hpp"
#include "strings.hpp"
#include "StageInterface.hpp"

namespace roguedm {

bool Application::process_arguments (
  int argc, char* argv[],
  const ConfigReference& configuration
) {

  bool skipLaunch = false;

  // Notice how this parses arguments from the end of the list
  for (int c = argc; c > 1 && !skipLaunch; c--) {

    if (!strcmp (argv[c - 1], RDM_STR_USAGE_VERSION)) {    // --usage
      printf (_ (RDM_STR_VERSION_STRING));
      printf (_ (RDM_STR_VERSION_FULL));
      printf ("\n");
      skipLaunch = true;
    }

    else if (!strcmp (argv[c - 1], RDM_STR_USAGE_HELP)) {  // --help
      printf (_ (RDM_STR_VERSION_STRING));
      printf (_ (RDM_STR_VERSION_FULL));
      printf ("\n");
      printf (_ (RDM_STR_USAGE), argv[0]);
      printf ("\n");
      skipLaunch = true;
    }

    else if (!strcmp (argv[c - 1], RDM_STR_USAGE_LOCAL)) { // --local
      configuration->setDoNotUseNetworking (true);
    }

    else {                                                 // Unknown argument
      printf (_ (RDM_STR_USAGE_UKNOWN), argv[c - 1]);
      skipLaunch = true;
    }

  }
  return skipLaunch;
}

int Application::run(int argc, char *argv[]) {

  setlocale(LC_ALL, ""); // TODO: Remove all STL locale handling

  // Load configuration and parse program arguments
  ConfigReference configuration = Config::instance();
  bool skipLaunch = process_arguments(argc, argv, configuration);

  if(!skipLaunch) {

    // Run the different application stages and handle their chaining
    StageInterfaceReference currentStage =
        std::make_shared<roguedm_main::Main>();
    StageResponse lastResponse;
    do {
      lastResponse = currentStage->execute();
      currentStage = lastResponse.nextStage;
    } while (lastResponse.status!=0 and RDM_STAGE_EXIT!=currentStage);
    return lastResponse.status;

  }

  return 0;

}

} // namespace roguedm
