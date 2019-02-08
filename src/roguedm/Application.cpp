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

#include <iostream>
#include <locale>
#include <string>

#include <SDL2/SDL.h>

#include "stages/GuiStage.hpp"
#include "strings.hpp"
#include "StageInterface.hpp"

namespace roguedm {

void Application::process_arguments (
  int argc, char* argv[],
  const ConfigReference& configuration
) {

  bool skipNextArguments = false;

  // Notice how this parses arguments from the end of the list
  for (int c = argc; c > 1 && !skipNextArguments; c--) {

    auto current_argument = std::string(argv[c - 1]);

    if (0==current_argument.compare(RDM_STR_USAGE_VERSION)) {    // --usage
      std::cout << format_string(
        _ (RDM_STR_VERSION_STRING), RDM_STR_VERSION_FULL
      );
      skipNextArguments = true;
      keepRunning = false;
    }

    else if (0==current_argument.compare(RDM_STR_USAGE_HELP)) {  // --help
      std::cout << format_string(
        _ (RDM_STR_VERSION_STRING), RDM_STR_VERSION_FULL
      );
      std::cout << format_string(
        _ (RDM_STR_USAGE), argv[0]
      );
      skipNextArguments = true;
      keepRunning = false;
    }

    else if (0==current_argument.compare(RDM_STR_USAGE_LOCAL)) { // --local
      configuration->setSettingBoolValue("general", "skipNetworking", true);
    }

    else {                                                 // Unknown argument
      std::cout << format_string(_ (RDM_STR_USAGE_UKNOWN), argv[c - 1]);
      skipNextArguments = true;
      argumentsError = true;
      exitStatus = RDM_ERR_ARGS_ERROR;
      keepRunning = false;
    }

  }
}

int Application::run(int argc, char *argv[]) {

  // System locale
  setlocale(LC_ALL, "");

  ConfigReference configuration;

  // Load configuration
  try {
    configuration = Config::instance();
  } catch (const ConfigException& e) {
    SDL_LogError(
      SDL_LOG_CATEGORY_APPLICATION,
      e.what()
    );
    exitStatus = RDM_ERR_SETTINGS_ERROR;
    keepRunning = false;
  }

  // Parse program arguments
  if(keepRunning)
    process_arguments(argc, argv, configuration);

  if(keepRunning) {

    // Run the different application stages and handle their chaining
    StageInterfaceReference currentStage =
        std::make_shared<GuiStage>();
    StageResponse lastResponse;
    do {
      lastResponse = currentStage->execute();
      currentStage = lastResponse.nextStage;
    } while (lastResponse.status!=0 and RDM_STAGE_EXIT!=currentStage);
    exitStatus = lastResponse.status;

  }

  return exitStatus;

}

} // namespace roguedm
