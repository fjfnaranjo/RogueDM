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

#include <SDL.h>

#include "stages/GuiStage.hpp"
#include "strings.hpp"
#include "StageInterface.hpp"

namespace roguedm {

Application::Application() {
  exitStatus = 0;
  keepRunning = true;
  argumentsError = false;
}

void Application::process_verbosity(int argc, char* argv[]) {
  SDL_LogPriority level = SDL_LOG_PRIORITY_WARN;
  for (int c = 1; c < argc; c++)
    if (0==std::string(argv[c]).compare(RDM_STR_USAGE_VERBOSE))
      level = SDL_LOG_PRIORITY_VERBOSE;
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, level);
}

void Application::process_arguments (
  int argc, char* argv[],
  const ConfigSharedPtr& configuration
) {

  bool skipNextArguments = false;

  for (int c = 1; c < argc && !skipNextArguments; c++) {

    auto currentArgument = std::string(argv[c]);

    if (0==currentArgument.compare(RDM_STR_USAGE_VERSION)) {    // --usage
      std::cout << format_string(
        RDM_STR_VERSION_STRING, RDM_STR_VERSION_FULL
      );
      skipNextArguments = true;
      keepRunning = false;
    }

    else if (0==currentArgument.compare(RDM_STR_USAGE_HELP)) {  // --help
      std::cout << format_string(
        RDM_STR_VERSION_STRING, RDM_STR_VERSION_FULL
      );
      std::cout << format_string(
        RDM_STR_USAGE, argv[0]
      );
      skipNextArguments = true;
      keepRunning = false;
    }

    else if (0==currentArgument.compare(RDM_STR_USAGE_LOCAL)) { // --local
      configuration->setSettingBoolValue("general", "skipNetworking", true);
    }

    else if (0==currentArgument.compare(RDM_STR_USAGE_VERBOSE)) { // --verbose
      ;
    }

    else {                                                 // Unknown argument
      SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        RDM_STR_USAGE_UKNOWN,
        currentArgument.c_str()
      );
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

  ConfigSharedPtr configuration = Config::instance();

  // Load configuration
  if(!configuration->loadFromFile()) {
    SDL_LogError(
      SDL_LOG_CATEGORY_APPLICATION,
      RDM_STR_CFG_LOAD_ERROR,
      configuration->getConfigurationLoadError().c_str()
    );
    exitStatus = RDM_ERR_SETTINGS_ERROR;
    keepRunning = false;
  }

  process_verbosity(argc, argv);

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
    } while (lastResponse.status!=0 && RDM_STAGE_EXIT!=currentStage);
    exitStatus = lastResponse.status;

  }

  return exitStatus;

}

} // namespace roguedm
