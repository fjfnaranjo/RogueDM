// Copyright 2022 Fco. Jose Fdez. Naranjo AKA github.com/fjfnaranjo

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
#include <memory>
#include <string>

#include <SDL.h>

#include "commands/CommandHandlers.hpp"
#include "game/Game.hpp"
#include "network/Network.hpp"
#include "sdl2/Sdl2IO.hpp"
#include "strings.hpp"
#include "version.hpp"

#define RDM_ARGS_CONTINUE 0
#define RDM_ARGS_EXIT     1
#define RDM_ARGS_ERROR    2

#define RDM_ERR_OK             0
#define RDM_ERR_SETTINGS_ERROR 1
#define RDM_ERR_ARGS_ERROR     2
#define RDM_ERR_SDL2_ERROR     3
#define RDM_ERR_NETWORK_ERROR  4

namespace roguedm {

void Application::parse_verbosity(int argc, char* argv[]) {
  SDL_LogPriority level = SDL_LOG_PRIORITY_WARN;
  for (int c = 1; c < argc; c++)
    if (0 == std::string(argv[c]).compare(RDM_STR_USAGE_VERBOSE))
      level = SDL_LOG_PRIORITY_VERBOSE;
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, level);
}

int Application::parse_arguments(int argc, char* argv[],
                                 const ConfigSharedPtr& configuration) {
  for (int c = 1; c < argc; c++) {
    auto currentArgument = std::string(argv[c]);

    if (0 == currentArgument.compare(RDM_STR_USAGE_VERSION)) {
      // --usage
      std::cout << format_string(RDM_STR_VERSION_STRING, RDM_STR_VERSION_FULL);
      return RDM_ARGS_EXIT;

    } else if (0 == currentArgument.compare(RDM_STR_USAGE_HELP)) {
      // --help
      std::cout << format_string(RDM_STR_VERSION_STRING, RDM_STR_VERSION_FULL);
      std::cout << format_string(RDM_STR_USAGE, argv[0]);
      return RDM_ARGS_EXIT;

    } else if (0 == currentArgument.compare(RDM_STR_USAGE_LOCAL)) {
      // --local
      configuration->setSettingBoolValue("general", "skipNetworking", true);

    } else if (0 == currentArgument.compare(RDM_STR_USAGE_VERBOSE)) {
      // --verbose

    } else {
      // Unknown argument
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, RDM_STR_USAGE_UKNOWN,
                   currentArgument.c_str());
      return RDM_ARGS_ERROR;
    }

  }

  return RDM_ARGS_CONTINUE;

}

int Application::run(int argc, char *argv[]) {

  // System locale
  setlocale(LC_ALL, "");

  parse_verbosity(argc, argv);

  ConfigSharedPtr config = Config::instance();

  // Load configuration
  if (!config->loadFromFile()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, RDM_STR_CFG_LOAD_ERROR,
                 config->getConfigurationLoadError().c_str());
    return RDM_ERR_SETTINGS_ERROR;
  }

  // Parse program arguments
  int argsStatus = parse_arguments(argc, argv, config);
  if (RDM_ARGS_ERROR == argsStatus)
    return RDM_ERR_ARGS_ERROR;

  // Main application loop
  if (RDM_ARGS_CONTINUE == argsStatus) {

    bool networkingEnabled = !config->getSettingBoolValue("general",
                                                          "skipNetworking",
                                                          false);

    auto gameInstance = std::make_shared<roguedm_game::Game>();

    auto sdl2Io = std::make_shared<roguedm_gui::Sdl2IO>();
    if (!sdl2Io->initSdl2IO()) {
      return RDM_ERR_SDL2_ERROR;
    }

    roguedm::CommandHandlers::instance()->registerCommandHandler(sdl2Io);

    auto network = std::make_shared<Network>();
    if (networkingEnabled) {
      if (!network->initNetwork()) {
        return RDM_ERR_NETWORK_ERROR;
      }
    } else {
      SDL_Log(RDM_STR_NOT_NETWORKING);
    }

    // input checking and scene drawing (game loop)
    int done = 0;
    while (!done) {
      sdl2Io->update();
      done = sdl2Io->mustHalt();
      if (networkingEnabled)
        network->update();
      // TODO(fjfnaranjo): Check if remote and game instances should issue a
      // 'done' also
      // done = NetworkInstance->mustHalt();
      gameInstance->update();
      // done = gameInstance->mustHalt();
    }

  }

  return RDM_ERR_OK;

}

}  // namespace roguedm
