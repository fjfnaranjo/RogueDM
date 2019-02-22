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

  appDone = 0;

  ticks = 0;

  defaultCWidth = 0;
  defaultCHeight = 0;
  maxCols = 0;
  maxRows = 0;

  currentWord = 0;
  wordRShift = 0;
  historyCurrent = 0;

}

Sdl2IO::~Sdl2IO() {
  if(initSuccess) {
    defaultStamper.reset();
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

  // Clear color
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  // Words, command line and history init
  if(!initCharmaps()) {
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

  resetLine();
  historyCurrent = 0;
  historyBackup = commandLine;

  // Default word
  roguedm::Word newDefaultWord;
  newDefaultWord.wordContent = RDM_CMD_PSAY;
  newDefaultWord.wordClass = RDM_WCLASS_COMMAND;
  setDefaultWord(newDefaultWord);

  // Local command handler
  registerCommandHandler(this);

  // More SDL Init
  ticks = SDL_GetTicks();
  resetScreenSize();
  SDL_ShowCursor(true);
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

  // Panels
  // TODO: Move box drawing to the stamper class or a helper function
  defaultStamper->drawBox        (0,0,maxCols,maxRows,renderer);
  dialogMain.x = 1;
  dialogMain.y = 1;
  dialogMain.w = (int)(maxCols-(maxCols/4));
  dialogMain.h = (int)(maxRows-(maxRows/3));
  dialogText.x = 1;
  dialogText.y = dialogMain.h+3;
  dialogText.w = dialogMain.w;
  dialogText.h = maxRows-dialogMain.h-4;
  defaultStamper->drawVSeparator(dialogText.w+2,0,maxRows,renderer);
  defaultStamper->drawHSeparator(0,dialogText.y-1,dialogText.w+2,renderer);
  dialogCell.x = dialogText.w+3;
  dialogCell.y = 1;
  dialogCell.w = maxCols-dialogMain.w-4;
  dialogCell.h = (int)(maxRows-((maxRows/8)*2));
  defaultStamper->drawHSeparator(
    dialogCell.x-1,dialogCell.y+dialogCell.h+1,dialogCell.w+2,renderer
  );
  dialogCreature.x = dialogCell.x;
  dialogCreature.y = dialogCell.y+dialogCell.h+2;
  dialogCreature.w = dialogCell.w;
  dialogCreature.h = dialogCell.h*2;
  defaultStamper->drawHSeparator(
    dialogCreature.x-1,dialogCreature.y+dialogCreature.h+1,
    dialogCreature.w+2,renderer
  );
  dialogPlayers.x = dialogCell.x;
  dialogPlayers.y = dialogCreature.y+dialogCreature.h+2;
  dialogPlayers.w = dialogCell.w;
  dialogPlayers.h = maxRows-dialogCell.h-dialogCreature.h-6;

  // Current command line
  int limit = commandLine.size();
  int cStart=0;
  int cSkip=0;
  int cLineLenght=0;
  SDL_Rect dest;

  for(int c = 0; c < limit; c++) {

    cLineLenght += 1 +
      multibyteLenght(commandLine[c].wordContent) +
      defaultStamper->decoratorsLength(commandLine[c].wordClass);

    if(cLineLenght>dialogText.w) {

      cLineLenght = 1 +
        multibyteLenght(commandLine[c].wordContent) +
        defaultStamper->decoratorsLength(commandLine[c].wordClass);

      ++cStart;

    }

  }

  if((int)(cStart)>dialogText.h) {

    cSkip = cStart-dialogText.h;
    cStart = dialogText.h;

  }

  cLineLenght = 0;

  for(int c = 0; c < limit; c++) {

    if(cSkip>0) {
      cLineLenght += 1 +
        multibyteLenght(commandLine[c].wordContent) +
        defaultStamper->decoratorsLength(commandLine[c].wordClass);
      if(cLineLenght>dialogText.w) {
        --cSkip;
        cLineLenght = 0;
      }
    }

    if(cSkip==0) {

      int nextLineLength = cLineLenght + 1 +
        multibyteLenght(commandLine[c].wordContent) +
        defaultStamper->decoratorsLength(commandLine[c].wordClass);
      if(nextLineLength>dialogText.w) {
        cLineLenght = 0;
        --cStart;
      }

      int wordL = multibyteLenght(commandLine[c].wordContent) +
          defaultStamper->decoratorsLength(commandLine[c].wordClass);

      int xpos=dialogText.x+cLineLenght;

      if (wordL>0) {
        for(
          std::size_t it=0;
            it < defaultStamper->lDecoratorsLength(commandLine[c].wordClass);
          ++it, ++xpos
        )
          defaultStamper->stampLDecoratorChar(
            it,
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart,
            renderer,
            maxCols,
            maxRows
          );

        for(
          std::size_t it=0;
          it < multibyteLenght(commandLine[c].wordContent);
          ++it, ++xpos
        )
          defaultStamper->stampInnerChar(
            multibyteCharacterByIndex(commandLine[c].wordContent, it),
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart,
            renderer,
            maxCols,
            maxRows
          );

        for(
          std::size_t it=0;
            it < defaultStamper->rDecoratorsLength(commandLine[c].wordClass);
          ++it, ++xpos
        )
          defaultStamper->stampRDecoratorChar(
            it,
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart,
            renderer,
            maxCols,
            maxRows
          );

      }

      dest.y = (dialogText.y+(dialogText.h)-cStart)*defaultCHeight;
      dest.w = defaultCWidth;
      dest.h = defaultCHeight;
      if(c==currentWord) {
        if(wordRShift==0) {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+wordL-
                defaultStamper->rDecoratorsLength(commandLine[c].wordClass)
              )*defaultCWidth
            );
          defaultStamper->stampIp(renderer, dest.x, dest.y);
        } else if (
          wordRShift==(int)multibyteLenght(commandLine[c].wordContent)
        ) {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+
                defaultStamper->lDecoratorsLength(commandLine[c].wordClass)
              )*defaultCWidth
            );
          defaultStamper->stampIp(renderer, dest.x, dest.y);
        } else {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+wordL-wordRShift-
                defaultStamper->rDecoratorsLength(commandLine[c].wordClass)
              )*defaultCWidth
            );
          defaultStamper->stampIp(renderer, dest.x, dest.y);
        }
      }

      cLineLenght += 1 +
        multibyteLenght(commandLine[c].wordContent) +
        defaultStamper->decoratorsLength(commandLine[c].wordClass);

    }

  }

  // End of command line .......

  // TODO: Paint the command history if fits

  SDL_RenderPresent(renderer);

}

int Sdl2IO::processCommand(const roguedm::Sentence& a) {
  if(a[0].wordContent==RDM_CMD_QUIT && a[0].wordClass==RDM_WCLASS_COMMAND) {
    appDone = 1;
    return RDM_COMMAND_DONE;
  }
  return RDM_COMMAND_UNKNOWN;
}

int Sdl2IO::autocomplete(roguedm::Sentence& a) const {

  // quit command completion
  if(a[0].wordContent==RDM_CMD_QUIT && a[0].wordClass==RDM_WCLASS_NORMAL) {
    a[0].wordClass=RDM_WCLASS_COMMAND;
    return RDM_COMMAND_AC_COMPLETED;
  }

  return RDM_COMMAND_AC_NEXT;

}

roguedm::SentenceListReference Sdl2IO::autocompleteListOptions(
  const roguedm::Sentence& a
) const {

  roguedm::Word psayCmd;
  psayCmd.wordContent = RDM_CMD_PSAY;
  psayCmd.wordClass = RDM_WCLASS_COMMAND;

  roguedm::Word quitCmd;
  quitCmd.wordContent = RDM_CMD_QUIT;
  quitCmd.wordClass = RDM_WCLASS_COMMAND;

  if(a.size()==1 && multibyteLenght(a[0].wordContent)==0) {

    roguedm::SentenceListReference l =
        std::make_shared<roguedm::SentenceList>();

    roguedm::SentenceReference o =
        std::make_shared<roguedm::Sentence>();

    o->push_back(psayCmd);
    l->push_back(*o);

    o->push_back(quitCmd);
    l->push_back(*o);

    return l;

  }

  return std::make_shared<roguedm::SentenceList>();

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
            resetScreenSize();
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

void Sdl2IO::resetScreenSize() {
  int ww, wh;
  SDL_GetWindowSize(window, &ww, &wh);
  maxCols=(int)floor(ww/defaultCWidth)-1;
  maxRows=(int)floor(wh/defaultCHeight)-1;
}

bool Sdl2IO::initCharmaps() {

  // Default charmap texture
  std::string defaultCharmap =
    RDM_CFG_CHARMAP_PREFIX + config->getSettingValue(
      "general", "charmaps", "default"
    );

  defaultStamper = std::make_unique<CharmapStamper>();
  if(!defaultStamper->loadDefaultCharmap(renderer, defaultCharmap))
    return false;

  defaultCHeight = defaultStamper->getCHeight();
  defaultCWidth = defaultStamper->getCWidth();

  return true;

}

void Sdl2IO::resetLine() {
  roguedm::Word emptyWord;
  emptyWord.wordContent = u8"";
  emptyWord.wordClass = RDM_WCLASS_NORMAL;
  commandLine.clear();
  commandLine.push_back(emptyWord);
  currentWord = 0;
  wordRShift = 0;
}

void Sdl2IO::processText(SDL_Event* event) {

  roguedm::Word emptyWord;
  emptyWord.wordContent = RDM_WEMPTY;
  emptyWord.wordClass = RDM_WCLASS_NORMAL;

  std::string text = event->text.text;

  if(text == u8"") {

    // Space
    if(wordRShift==0) {
      commandLine.insert(
        commandLine.begin()+currentWord+1,
        emptyWord
      );
      currentWord++;
    } else {
      commandLine.insert(
        commandLine.begin()+currentWord+1,
        emptyWord
      );
      commandLine[currentWord+1].wordContent = multibyteSubstr(
        commandLine[currentWord].wordContent,
        multibyteLenght(commandLine[currentWord].wordContent)-wordRShift,
        wordRShift
      );
      commandLine[currentWord].wordContent = multibyteSubstr(
        commandLine[currentWord].wordContent,
        0,
        multibyteLenght(commandLine[currentWord].wordContent)-wordRShift
      );
      commandLine[currentWord].wordClass = RDM_WCLASS_NORMAL;
      currentWord++;
      wordRShift = multibyteLenght(commandLine[currentWord].wordContent);

    }
    historyCurrent = 0;

  } else {

    // TODO: SDL2New Implement SDL2 text input support
    if(0==currentWord)
      commandLine[currentWord].wordClass = RDM_WCLASS_NORMAL;
    if(wordRShift==0) {
      commandLine[currentWord].wordContent += text;
    } else {
      std::string leftPart = multibyteSubstr(
        commandLine[currentWord].wordContent,
        0,
        multibyteLenght(commandLine[currentWord].wordContent)-wordRShift
      );
      std::string rightPart = multibyteSubstr(
        commandLine[currentWord].wordContent,
        multibyteLenght(commandLine[currentWord].wordContent)-wordRShift,
        wordRShift
      );
      commandLine[currentWord].wordContent = leftPart;
      commandLine[currentWord].wordContent += text;
      commandLine[currentWord].wordContent += rightPart;
    }
    historyCurrent = 0;

  }

}

void Sdl2IO::processKey(SDL_Event* event) {

  roguedm::Word emptyWord;
  emptyWord.wordContent = RDM_WEMPTY;
  emptyWord.wordClass = RDM_WCLASS_NORMAL;

  SDL_KeyboardEvent kevent = event->key;

  switch(kevent.keysym.sym) {

    // Enter
    case SDLK_RETURN:
      processLine();
      break;

    // Backspace
    case SDLK_BACKSPACE:
      if(
        wordRShift==(int)multibyteLenght(commandLine[currentWord].wordContent)
      ) {
        if(currentWord>0) {
          if(
            1==currentWord
            && multibyteLenght(commandLine[currentWord].wordContent)!=0
          )
            commandLine[currentWord-1].wordClass = RDM_WCLASS_NORMAL;
          wordRShift=multibyteLenght(commandLine[currentWord].wordContent);
          commandLine[currentWord-1].wordContent =
            commandLine[currentWord-1].wordContent +
            commandLine[currentWord].wordContent;
          commandLine.erase(commandLine.begin()+currentWord);
          currentWord--;
          }
      } else {
        if(0==currentWord)
          commandLine[0].wordClass = RDM_WCLASS_NORMAL;
        commandLine[currentWord].wordContent =
          multibyteSubstr(
            commandLine[currentWord].wordContent,
            0,
            multibyteLenght(commandLine[currentWord].wordContent)-wordRShift-1
          ) +
          multibyteSubstr(
            commandLine[currentWord].wordContent,
            multibyteLenght(commandLine[currentWord].wordContent)-wordRShift,
            wordRShift
          );
      }
      historyCurrent = 0;
      break;

    // Delete
    case SDLK_DELETE:
      if(wordRShift==0) {
        if((currentWord+1)<(int)commandLine.size()) {
          if(0==currentWord && multibyteLenght(commandLine[1].wordContent)!=0)
            commandLine[0].wordClass = RDM_WCLASS_NORMAL;
          wordRShift=multibyteLenght(commandLine[currentWord+1].wordContent);
          commandLine[currentWord].wordContent =
            commandLine[currentWord].wordContent +
            commandLine[currentWord+1].wordContent;
          commandLine.erase(commandLine.begin()+currentWord+1);
        }
      } else {
        if(0==currentWord)
          commandLine[0].wordClass = RDM_WCLASS_NORMAL;
        commandLine[currentWord].wordContent =
          multibyteSubstr(
            commandLine[currentWord].wordContent,
            0,
            multibyteLenght(commandLine[currentWord].wordContent)-wordRShift
          ) +
          multibyteSubstr(
            commandLine[currentWord].wordContent,
            multibyteLenght(commandLine[currentWord].wordContent)-
              wordRShift+1,
            wordRShift-1
          );
          wordRShift--;
      }
      historyCurrent = 0;
      break;

    // H-movement keys
    case SDLK_LEFT:
      if (!(kevent.keysym.mod&KMOD_CTRL)) {
        if(
          wordRShift<(int)multibyteLenght(commandLine[currentWord].wordContent)
        )
          wordRShift++;
        else if (currentWord>0) {
          currentWord--;
          wordRShift=0;
        }
        historyCurrent = 0;
      } else {
        if(
          wordRShift<(int)multibyteLenght(commandLine[currentWord].wordContent)
        ) {
          wordRShift = multibyteLenght(commandLine[currentWord].wordContent);
        } else if (currentWord>0) {
          currentWord--;
          wordRShift=0;
        }
        historyCurrent = 0;
      }
      break;

    case SDLK_RIGHT:
      if (!(kevent.keysym.mod&KMOD_CTRL)) {
        if(wordRShift>0)
          wordRShift--;
        else if (currentWord<((int)commandLine.size()-1)) {
          currentWord++;
          wordRShift = multibyteLenght(commandLine[currentWord].wordContent);
        }
        historyCurrent = 0;
      } else {
        if(wordRShift>0) {
          wordRShift = 0;
        } else if ((currentWord+1)<(int)commandLine.size()) {
          currentWord++;
          wordRShift = multibyteLenght(commandLine[currentWord].wordContent);
        }
        historyCurrent = 0;
      }
      break;

    // Far-long h-movement keys
    case SDLK_HOME:
      currentWord = 0;
      wordRShift = multibyteLenght(commandLine[currentWord].wordContent);
      historyCurrent = 0;
      break;

    case SDLK_END:
      currentWord = commandLine.size()-1;
      wordRShift = 0;
      historyCurrent = 0;
      break;

    // History keys
    case SDLK_UP:
      if(!history.empty()) {
        if(0==historyCurrent) {
          historyBackup = commandLine;
          historyCurrent = history.size();
          commandLine = history[historyCurrent-1];
          currentWord = commandLine.size()-1;
          wordRShift = 0;
        } else if (historyCurrent>1) {
          historyCurrent--;
          commandLine = history[historyCurrent-1];
          currentWord = commandLine.size()-1;
          wordRShift = 0;
        }
      }
      break;

    case SDLK_DOWN:
      if(!history.empty()) {
        if(historyCurrent) {
          if(historyCurrent<(int)history.size()) {
            historyCurrent++;
            commandLine = history[historyCurrent-1];
            currentWord = commandLine.size()-1;
            wordRShift = 0;
          } else {
            historyCurrent=0;
            resetLine();
            commandLine = historyBackup;
            currentWord = commandLine.size()-1;
            wordRShift = 0;
          }
        }
      }
      break;

    // TAB
    case SDLK_TAB:
      tryAutocompletion();
      historyCurrent = 0;
      break;


  }

}

void Sdl2IO::processLine() {

  // Ignore empty lines
  if(!commandLine.empty()) {

    // If first word is empty, inserts the say command
    if(multibyteLenght(commandLine[0].wordContent)==0) {
      commandLine.erase(commandLine.begin());
      commandLine.insert(commandLine.begin(),defaultWord);
    }

    // If first word is not a command, inserts the say command
    if(commandLine[0].wordClass!=RDM_WCLASS_COMMAND)
      commandLine.insert(commandLine.begin(),defaultWord);

    // Process action
    for(const auto & commandHandler : commandHandlers)
      if(RDM_COMMAND_DONE==commandHandler->processCommand(commandLine))
        break;

    // Push the command in the historic
    history.push_back(commandLine);

    // Print the command to the console
    consoleHistory.push_back(commandLine);

    // Limit the command historic size
    if(history.size()>RDM_CL_MAX_HISTORY)
      history.erase(history.begin());

    // Reset the history pointer
    historyCurrent = 0;

    // Reset the command line
    resetLine();

  }

}

void Sdl2IO::tryAutocompletion() {

  // empty word to insert when expanding commands
  roguedm::Word emptyWord;
  emptyWord.wordContent = RDM_WEMPTY;
  emptyWord.wordClass = RDM_WCLASS_NORMAL;

  roguedm::SentenceListReference options;

  // check for empty line to list all commands
  if(commandLine.size()==1 && multibyteLenght(commandLine[0].wordContent)==0) {

    for(const auto & commandHandler : commandHandlers) {

      options = commandHandler->autocompleteListOptions(commandLine);

      if(!options->empty()) {
        for(const auto & option : *options)
          consoleHistory.push_back(option);
      }

    }

  }

  // Process action
  for(const auto & commandHandler : commandHandlers) {

    if(RDM_COMMAND_AC_COMPLETED==commandHandler->autocomplete(commandLine))
      break;

    options = commandHandler->autocompleteListOptions(commandLine);

    if(!options->empty()) {
      for(const auto & option : *options)
        consoleHistory.push_back(option);
      break;
    }

  }

}

void Sdl2IO::setDefaultWord(roguedm::Word c) {
  defaultWord = c;
}

} // namespace roguedm_gui
