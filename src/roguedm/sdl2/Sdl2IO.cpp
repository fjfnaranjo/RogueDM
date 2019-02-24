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

#include "Sdl2IO.hpp"

#include <algorithm>
#include <cmath>
#include <locale>
#include <vector>

#include "mbtools.hpp"
#include "../macros.hpp"
#include "../strings.hpp"

namespace roguedm_gui {

Sdl2IO::Sdl2IO() {

  initSuccess = false;

  config = roguedm::Config::instance();

  gui = std::make_unique<Gui>();

  appDone = 0;

  ticks = 0;

}

Sdl2IO::~Sdl2IO() {
  if(initSuccess) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
}

bool Sdl2IO::initSdl2IO() {

  if(0!=SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(
      SDL_LOG_CATEGORY_APPLICATION,
      RDM_STR_SDL_ERROR,
      SDL_GetError()
    );
    return false;
  }

  int createStatus = SDL_CreateWindowAndRenderer(
    800, 500,
    SDL_WINDOW_RESIZABLE,
    &window,
    &renderer
  );

  if(createStatus || NULL==window || NULL==renderer) {
    SDL_LogError(
      SDL_LOG_CATEGORY_APPLICATION,
      RDM_STR_SDL_ERROR,
      SDL_GetError()
    );
    SDL_Quit();
    return false;
  }

  if(!gui->initGui(renderer)) {
    SDL_LogError(
      SDL_LOG_CATEGORY_APPLICATION,
      RDM_STR_SDL_ERROR,
      SDL_GetError()
    );
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  // Local command handler
  registerCommandHandler(this);

  // More SDL Init
  ticks = SDL_GetTicks();
  gui->resetScreenSize(window);
  SDL_StartTextInput();

  initSuccess = true;
  return true;

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

int Sdl2IO::processCommand(const roguedm::Sentence &a) {
  if(a.begin()[0].wordContent==RDM_CMD_QUIT && a.begin()[0].wordClass==RDM_WCLASS_COMMAND) {
    appDone = 1;
    return RDM_COMMAND_DONE;
  }
  // TODO: This must be RDM_COMMAND_UNKNOWN or you will suffer
  return RDM_COMMAND_DONE;
}

int Sdl2IO::autocomplete(roguedm::Sentence &a) const {

  // quit command completion
  if(a.begin()[0].wordContent==RDM_CMD_QUIT && a.begin()[0].wordClass==RDM_WCLASS_NORMAL) {
    a.begin()[0].wordClass=RDM_WCLASS_COMMAND;
    return RDM_COMMAND_AC_COMPLETED;
  }

  return RDM_COMMAND_AC_NEXT;

}

roguedm::SentenceList Sdl2IO::autocompleteListOptions(
  const roguedm::Sentence &a
) const {

  roguedm::SentenceList options;

  roguedm::Word psayCmd;
  psayCmd.wordContent = RDM_CMD_PSAY;
  psayCmd.wordClass = RDM_WCLASS_COMMAND;
  roguedm::Sentence psaySentence = {psayCmd};

  roguedm::Word quitCmd;
  quitCmd.wordContent = RDM_CMD_QUIT;
  quitCmd.wordClass = RDM_WCLASS_COMMAND;
  roguedm::Sentence quitSentence = {quitCmd};

  if(a.size()==1 && multibyteLenght(a.begin()[0].wordContent)==0) {

    options.push_back(psaySentence);
    options.push_back(quitSentence);

    return options;

  }

  return roguedm::SentenceList();

}

void Sdl2IO::registerCommandHandler(CommandHandlerInterface *c) {
  unregisterCommandHandler(c);
  commandHandlers.push_back(c);
}

void Sdl2IO::unregisterCommandHandler(CommandHandlerInterface *c) {
  commandHandlers.erase(
    std::remove(commandHandlers.begin(), commandHandlers.end(), c),
    commandHandlers.end()
  );
}

int Sdl2IO::mustHalt() {
  return appDone;
}

void Sdl2IO::eventsManager() {

  // TODO: Fix memory leak with event

  SDL_Event event;

  while ( SDL_PollEvent(&event) ) {
    switch (event.type) {
      case SDL_QUIT:
        appDone = 1;
        break;
      case SDL_WINDOWEVENT:
        switch (event.window.event)  {
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            gui->resetScreenSize(window);
            break;
        }
        break;
      // TODO: Resolve substitution/replace mode
      case SDL_TEXTINPUT:
        processText(&event);
        break;
      case SDL_KEYDOWN:
        processKey(&event);
        break;
    }
  }

}

void Sdl2IO::processText(SDL_Event* event) {

  roguedm::Word emptyWord;
  emptyWord.wordContent = RDM_WEMPTY;
  emptyWord.wordClass = RDM_WCLASS_NORMAL;

  std::string text = event->text.text;

  if(text == u8"") {

    gui->keySpace();

  } else {

    // TODO: SDL2New Implement SDL2 text input support

    gui->keyCharacter(text);

  }

}

void Sdl2IO::processKey(SDL_Event* event) {

  SDL_KeyboardEvent kevent = event->key;

  switch(kevent.keysym.sym) {

    // Enter
    case SDLK_RETURN:
      processLine();
      break;

    // Backspace
    case SDLK_BACKSPACE:
      gui->keyBackspace();
      break;

    // Delete
    case SDLK_DELETE:
      gui->keyDelete();
      break;

    // H-movement keys
    case SDLK_LEFT:
      if (!(kevent.keysym.mod&KMOD_CTRL)) {
        gui->keyLeft(false);
      } else {
        gui->keyLeft(true);
      }
      break;

    case SDLK_RIGHT:
      if (!(kevent.keysym.mod&KMOD_CTRL)) {
        gui->keyRight(false);
      } else {
        gui->keyRight(true);
      }
      break;

    // Far-long h-movement keys
    case SDLK_HOME:
      gui->keyHome();
      break;

    case SDLK_END:
      gui->keyEnd();
      break;

    // History keys
    case SDLK_UP:
      gui->keyUp();
      break;

    case SDLK_DOWN:
      gui->keyDown();
      break;

    // TAB
    case SDLK_TAB:
      tryAutocompletion();
      break;


  }

}

void Sdl2IO::tryAutocompletion() {

  roguedm::Sentence commandLine = gui->getCommandLine();

  // check for empty line to list all commands
  if(0==gui->commandLength()) {

    for(const auto & commandHandler : commandHandlers) {

      roguedm::SentenceList options =
        commandHandler->autocompleteListOptions(commandLine);

      if(!options.empty()) {
        for(const auto & option : options)
          gui->consoleHistoryPush(option);
      }

    }

  }

  // Process action
  for(const auto & commandHandler : commandHandlers) {

    if(RDM_COMMAND_AC_COMPLETED==commandHandler->autocomplete(commandLine)) {
      gui->setCommandLine(commandLine);
      break;
    }

    roguedm::SentenceList options =
      commandHandler->autocompleteListOptions(commandLine);

    if(!options.empty()) {
      for(const auto & option : options)
        gui->consoleHistoryPush(option);
      break;
    }

  }

  gui->resetHistoryCurrent();

}

void Sdl2IO::processLine() {

  if(0==gui->commandLength())
    return;

  roguedm::Sentence currentCommand = gui->getCommandLine();

  // TODO: Move this responsibility to other place
  if(currentCommand[0].wordClass==RDM_WCLASS_NORMAL) {
    roguedm::Word newPsayCommand;
    newPsayCommand.wordContent = RDM_CMD_PSAY;
    newPsayCommand.wordClass = RDM_WCLASS_COMMAND;
    currentCommand.insert(currentCommand.begin(), newPsayCommand);
  }

  // Process action
  for(const auto & commandHandler : commandHandlers)
    if(RDM_COMMAND_DONE==commandHandler->processCommand(currentCommand)) {

      // Push the command in the historic
      gui->commandHistoryPush(currentCommand);

      // Print the command to the console
      gui->consoleHistoryPush(currentCommand);

      // Reset the history pointer
      gui->resetHistoryCurrent();

      // Reset the command line
      gui->resetLine();
    }

}

} // namespace roguedm_gui
