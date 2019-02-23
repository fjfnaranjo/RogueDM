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

#include "Gui.hpp"

#include "mbtools.hpp"

namespace roguedm_gui {

Gui::Gui() {

  initSuccess = false;

  config = roguedm::Config::instance();

  defaultCWidth = 0;
  defaultCHeight = 0;
  maxCols = 0;
  maxRows = 0;

  currentWord = 0;
  wordRShift = 0;
  historyCurrent = 0;
  historyBackup = commandLine;

  resetLine();

  // Empty word
  emptyWord.wordContent = RDM_WEMPTY;
  emptyWord.wordClass = RDM_WCLASS_NORMAL;

}

Gui::~Gui() {
  if(initSuccess)
    defaultStamper.reset();
}

void Gui::update(SDL_Renderer *renderer) {

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

}

bool Gui::initGui(SDL_Renderer *renderer) {

  // Words, command line and history init
  if(!initCharmaps(renderer))
    return false;

  // Clear color
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  SDL_ShowCursor(true);

  return true;

}

roguedm::SentenceReference Gui::getCommandLine() {
  return std::make_shared<roguedm::Sentence>(commandLine);
}

int Gui::commandLength() {
  int limit = commandLine.size();
  int cLineLenght = 1;
  for(int c = 0; c < limit; c++)
    cLineLenght += 1 +
      multibyteLenght(commandLine[c].wordContent) +
      defaultStamper->decoratorsLength(commandLine[c].wordClass);
  return cLineLenght - 1;
}

bool Gui::hasCommand() {
  if(0!=commandLength() && commandLine[0].wordClass == RDM_WCLASS_COMMAND)
    return true;
  return false;
}

void Gui::commandHistoryPush(roguedm::Sentence command) {
  history.push_back(command);

  // Limit the command historic size
  if(history.size()>RDM_CL_MAX_HISTORY)
    history.erase(history.begin());

}

void Gui::consoleHistoryPush(roguedm::Sentence command) {
  consoleHistory.push_back(command);
}

void Gui::keyBackspace() {

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

}

void Gui::keyDelete() {

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

}

void Gui::keyLeft(bool fullWord) {

  if (!fullWord) {
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

}

void Gui::keyRight(bool fullWord) {

  if (!fullWord) {
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

}

void Gui::keyHome() {

  currentWord = 0;
  wordRShift = multibyteLenght(commandLine[currentWord].wordContent);
  historyCurrent = 0;

}

void Gui::keyEnd() {

  currentWord = commandLine.size()-1;
  wordRShift = 0;
  historyCurrent = 0;

}

void Gui::keyUp() {

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

}

void Gui::keyDown() {

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

}

void Gui::keySpace() {

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

}

void Gui::keyCharacter(std::string character) {

  if(0==currentWord)
    commandLine[currentWord].wordClass = RDM_WCLASS_NORMAL;
  if(wordRShift==0) {
    commandLine[currentWord].wordContent += character;
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
    commandLine[currentWord].wordContent += character;
    commandLine[currentWord].wordContent += rightPart;
  }
  historyCurrent = 0;

}

void Gui::resetHistoryCurrent() {
  historyCurrent = 0;
}

void Gui::resetLine() {
  commandLine.clear();
  commandLine.push_back(emptyWord);
  currentWord = 0;
  wordRShift = 0;
}

void Gui::resetScreenSize(SDL_Window *window) {
  int ww, wh;
  SDL_GetWindowSize(window, &ww, &wh);
  maxCols=(int)floor(ww/defaultCWidth)-1;
  maxRows=(int)floor(wh/defaultCHeight)-1;
}

bool Gui::initCharmaps(SDL_Renderer *renderer) {

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

} // namespace roguedm_gui
