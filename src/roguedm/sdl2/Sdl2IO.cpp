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

#include "Sdl2IO.hpp"

#include <algorithm>
#include <cmath>
#include <locale>
#include <memory>
#include <string>

#include "mbtools.hpp"
#include "../commands/CommandHandlers.hpp"
#include "../macros.hpp"
#include "../strings.hpp"
#include "../version.hpp"

namespace roguedm_gui {

Sdl2IO::Sdl2IO() {

  initSuccess = false;

  gui = std::make_unique<Gui>();

  appDone = 0;

  ticks = 0;

  window = nullptr;
  renderer = nullptr;

}

Sdl2IO::~Sdl2IO() {
  resetSdl2IO();
}

bool Sdl2IO::initSdl2IO() {

  if (0 != SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
    RDM_STR_SDL_ERROR,
                 SDL_GetError());
    return false;
  }

  int createStatus = SDL_CreateWindowAndRenderer(800, 500, SDL_WINDOW_RESIZABLE,
                                                 &window, &renderer);

  if (createStatus || NULL == window || NULL == renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
    RDM_STR_SDL_ERROR,
                 SDL_GetError());
    SDL_Quit();
    return false;
  }

  if (!gui->initGui(renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
    RDM_STR_SDL_ERROR,
                 SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  // More SDL Init
  ticks = SDL_GetTicks();
  gui->resetScreenSize(window);
  SDL_StartTextInput();

  initSuccess = true;
  return true;

}

void Sdl2IO::resetSdl2IO() {
  if (initSuccess) {
    gui->resetGui();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    initSuccess = false;
  }
}

void Sdl2IO::update() {

  // FPS lock
  if ((SDL_GetTicks() - ticks) < 33)
    return;
  ticks = SDL_GetTicks();

  // Process all events
  eventsManager();

  // Repaint main interface
  SDL_RenderClear(renderer);

  gui->update(renderer);

  SDL_RenderPresent(renderer);

}

bool Sdl2IO::processCommand(const roguedm::Command &command) {
  if (command.name == RDM_CMD_QUIT) {
    appDone = 1;
    return RDM_CMD_PROCESS_DONE;
  }
  return RDM_CMD_PROCESS_UNKNOWN;
}

bool Sdl2IO::identifyCommand(const roguedm::Sentence& sentence,
                             roguedm::Command &command) const {

  // quit command completion
  if (sentence.size() > 0 && sentence[0].content == RDM_CMD_QUIT) {
    command.name = RDM_CMD_QUIT;
    command.params.clear();
    return RDM_CMD_IDENTIFY_DONE;
  }

  return RDM_CMD_IDENTIFY_UNKNOWN;

}

roguedm::CommandList Sdl2IO::getCompletionCandidates(
    const roguedm::Command &command) const {

  if (0 == command.name.size() && 0 == command.params.size()) {

    roguedm::CommandList options;

    roguedm::Command quitCmd;
    quitCmd.name = RDM_CMD_QUIT;
    options.push_back(quitCmd);

    return options;

  }

  return roguedm::CommandList();

}

int Sdl2IO::mustHalt() {
  return appDone;
}

void Sdl2IO::eventsManager() {

  // TODO(fjfnaranjo): Fix memory leak with event

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        appDone = 1;
        break;
      case SDL_WINDOWEVENT:
        switch (event.window.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            gui->resetScreenSize(window);
            break;
        }
        break;
      // TODO(fjfnaranjo): Resolve substitution/replace mode
      case SDL_TEXTINPUT:
        gui->processText(event);
        break;
      case SDL_KEYDOWN:
        gui->processKey(event.key);
        break;
    }
  }

}

}  // namespace roguedm_gui
