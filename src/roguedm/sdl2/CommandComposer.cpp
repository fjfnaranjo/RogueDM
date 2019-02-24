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

#include "CommandComposer.hpp"

#include "mbtools.hpp"

namespace roguedm_gui {

CommandComposer::CommandComposer () {

  currentWord = 0;
  wordRShift = 0;

  // Empty word
  emptyWord.content = RDM_WEMPTY;
  emptyWord.kind = RDM_WCLASS_NORMAL;

  historyCurrent = 0;
  historyBackup = roguedm::Sentence();

  resetLine();

}

CommandComposer::~CommandComposer () {}

roguedm::Sentence CommandComposer::getCommandLine() {
  return commandLine;
}

void CommandComposer::setCommandLine(const roguedm::Sentence& newCommandLine) {
  commandLine = newCommandLine;
}

int CommandComposer::commandLength() {
  int limit = commandLine.size();
  int cLineLenght = 1;
  for(int c = 0; c < limit; c++)
    cLineLenght += 1 +
      multibyteLenght(commandLine[c].content) +
      multibyteLenght(roguedm::wordKinds[commandLine[c].kind].lDecorator) +
      multibyteLenght(roguedm::wordKinds[commandLine[c].kind].rDecorator);
  return cLineLenght - 1;
}

bool CommandComposer::hasCommand() {
  if(0!=commandLength() && commandLine[0].kind == RDM_WCLASS_COMMAND)
    return true;
  return false;
}

void CommandComposer::resetLine() {
  commandLine.clear();
  commandLine.push_back(emptyWord);
  currentWord = 0;
  wordRShift = 0;
}

void CommandComposer::keyBackspace() {

  if(
    wordRShift==(int)multibyteLenght(commandLine[currentWord].content)
  ) {
    if(currentWord>0) {
      if(
        1==currentWord
        && multibyteLenght(commandLine[currentWord].content)!=0
      )
        commandLine[currentWord-1].kind = RDM_WCLASS_NORMAL;
      wordRShift=multibyteLenght(commandLine[currentWord].content);
      commandLine[currentWord-1].content =
        commandLine[currentWord-1].content +
        commandLine[currentWord].content;
      commandLine.erase(commandLine.begin()+currentWord);
      currentWord--;
      }
  } else {
    if(0==currentWord)
      commandLine[0].kind = RDM_WCLASS_NORMAL;
    commandLine[currentWord].content =
      multibyteSubstr(
        commandLine[currentWord].content,
        0,
        multibyteLenght(commandLine[currentWord].content)-wordRShift-1
      ) +
      multibyteSubstr(
        commandLine[currentWord].content,
        multibyteLenght(commandLine[currentWord].content)-wordRShift,
        wordRShift
      );
  }
  historyCurrent = 0;

}

void CommandComposer::keyDelete() {

  if(wordRShift==0) {
    if((currentWord+1)<(int)commandLine.size()) {
      if(0==currentWord && multibyteLenght(commandLine[1].content)!=0)
        commandLine[0].kind = RDM_WCLASS_NORMAL;
      wordRShift=multibyteLenght(commandLine[currentWord+1].content);
      commandLine[currentWord].content =
        commandLine[currentWord].content +
        commandLine[currentWord+1].content;
      commandLine.erase(commandLine.begin()+currentWord+1);
    }
  } else {
    if(0==currentWord)
      commandLine[0].kind = RDM_WCLASS_NORMAL;
    commandLine[currentWord].content =
      multibyteSubstr(
        commandLine[currentWord].content,
        0,
        multibyteLenght(commandLine[currentWord].content)-wordRShift
      ) +
      multibyteSubstr(
        commandLine[currentWord].content,
        multibyteLenght(commandLine[currentWord].content)-
          wordRShift+1,
        wordRShift-1
      );
      wordRShift--;
  }
  historyCurrent = 0;

}

void CommandComposer::keyLeft(bool fullWord) {

  // TODO: Fix fullWord implementation.

  if (!fullWord) {
    if(
      wordRShift<(int)multibyteLenght(commandLine[currentWord].content)
    )
      wordRShift++;
    else if (currentWord>0) {
      currentWord--;
      wordRShift=0;
    }
    historyCurrent = 0;
  } else {
    if(
      wordRShift<(int)multibyteLenght(commandLine[currentWord].content)
    ) {
      wordRShift = multibyteLenght(commandLine[currentWord].content);
    } else if (currentWord>0) {
      currentWord--;
      wordRShift=0;
    }
    historyCurrent = 0;
  }

}

void CommandComposer::keyRight(bool fullWord) {

  if (!fullWord) {
    if(wordRShift>0)
      wordRShift--;
    else if (currentWord<((int)commandLine.size()-1)) {
      currentWord++;
      wordRShift = multibyteLenght(commandLine[currentWord].content);
    }
    historyCurrent = 0;
  } else {
    if(wordRShift>0) {
      wordRShift = 0;
    } else if ((currentWord+1)<(int)commandLine.size()) {
      currentWord++;
      wordRShift = multibyteLenght(commandLine[currentWord].content);
    }
    historyCurrent = 0;
  }

}

void CommandComposer::keyHome() {

  currentWord = 0;
  wordRShift = multibyteLenght(commandLine[currentWord].content);
  historyCurrent = 0;

}

void CommandComposer::keyEnd() {

  currentWord = commandLine.size()-1;
  wordRShift = 0;
  historyCurrent = 0;

}

void CommandComposer::keyUp() {

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

void CommandComposer::keyDown() {

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

void CommandComposer::keySpace() {

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
    commandLine[currentWord+1].content = multibyteSubstr(
      commandLine[currentWord].content,
      multibyteLenght(commandLine[currentWord].content)-wordRShift,
      wordRShift
    );
    commandLine[currentWord].content = multibyteSubstr(
      commandLine[currentWord].content,
      0,
      multibyteLenght(commandLine[currentWord].content)-wordRShift
    );
    commandLine[currentWord].kind = RDM_WCLASS_NORMAL;
    currentWord++;
    wordRShift = multibyteLenght(commandLine[currentWord].content);

  }
  historyCurrent = 0;

}

void CommandComposer::keyCharacter(std::string character) {

  if(0==currentWord)
    commandLine[currentWord].kind = RDM_WCLASS_NORMAL;
  if(wordRShift==0) {
    commandLine[currentWord].content += character;
  } else {
    std::string leftPart = multibyteSubstr(
      commandLine[currentWord].content,
      0,
      multibyteLenght(commandLine[currentWord].content)-wordRShift
    );
    std::string rightPart = multibyteSubstr(
      commandLine[currentWord].content,
      multibyteLenght(commandLine[currentWord].content)-wordRShift,
      wordRShift
    );
    commandLine[currentWord].content = leftPart;
    commandLine[currentWord].content += character;
    commandLine[currentWord].content += rightPart;
  }
  historyCurrent = 0;

}

void CommandComposer::commandHistoryPush(roguedm::Sentence command) {
  history.push_back(command);

  // Limit the command historic size
  if(history.size()>RDM_CL_MAX_HISTORY)
    history.erase(history.begin());

}

void CommandComposer::resetHistoryCurrent() {
  historyCurrent = 0;
}

void CommandComposer::paintCommandLine(
  SDL_Renderer *renderer, std::shared_ptr<CharmapStamper> stamper,
  SDL_Rect dialogMain, SDL_Rect dialogCell, SDL_Rect dialogCreature,
  SDL_Rect dialogText,
  int maxCols, int maxRows, int defaultCWidth, int defaultCHeight
) {

  // Panels
  // TODO: Move box drawing to other class or a helper function
  stamper->drawBox        (0,0,maxCols,maxRows,renderer);
  dialogMain.x = 1;
  dialogMain.y = 1;
  dialogMain.w = (int)(maxCols-(maxCols/4));
  dialogMain.h = (int)(maxRows-(maxRows/3));
  dialogText.x = 1;
  dialogText.y = dialogMain.h+3;
  dialogText.w = dialogMain.w;
  dialogText.h = maxRows-dialogMain.h-4;
  stamper->drawVSeparator(dialogText.w+2,0,maxRows,renderer);
  stamper->drawHSeparator(0,dialogText.y-1,dialogText.w+2,renderer);
  dialogCell.x = dialogText.w+3;
  dialogCell.y = 1;
  dialogCell.w = maxCols-dialogMain.w-4;
  dialogCell.h = (int)(maxRows-((maxRows/8)*2));
  stamper->drawHSeparator(
    dialogCell.x-1,dialogCell.y+dialogCell.h+1,dialogCell.w+2,renderer
  );
  dialogCreature.x = dialogCell.x;
  dialogCreature.y = dialogCell.y+dialogCell.h+2;
  dialogCreature.w = dialogCell.w;
  dialogCreature.h = dialogCell.h*2;
  stamper->drawHSeparator(
    dialogCreature.x-1,dialogCreature.y+dialogCreature.h+1,
    dialogCreature.w+2,renderer
  );

  // Current command line
  int limit = commandLine.size();
  int cStart=0;
  int cSkip=0;
  int cLineLenght=0;
  SDL_Rect dest;

  for(int c = 0; c < limit; c++) {

    cLineLenght += 1 +
      multibyteLenght(commandLine[c].content) +
      stamper->decoratorsLength(commandLine[c].kind);

    if(cLineLenght>dialogText.w) {

      cLineLenght = 1 +
        multibyteLenght(commandLine[c].content) +
        stamper->decoratorsLength(commandLine[c].kind);

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
        multibyteLenght(commandLine[c].content) +
        stamper->decoratorsLength(commandLine[c].kind);
      if(cLineLenght>dialogText.w) {
        --cSkip;
        cLineLenght = 0;
      }
    }

    if(cSkip==0) {

      int nextLineLength = cLineLenght + 1 +
        multibyteLenght(commandLine[c].content) +
        stamper->decoratorsLength(commandLine[c].kind);
      if(nextLineLength>dialogText.w) {
        cLineLenght = 0;
        --cStart;
      }

      int wordL = multibyteLenght(commandLine[c].content) +
          stamper->decoratorsLength(commandLine[c].kind);

      int xpos=dialogText.x+cLineLenght;

      if (wordL>0) {
        for(
          std::size_t it=0;
            it < stamper->lDecoratorsLength(commandLine[c].kind);
          ++it, ++xpos
        )
          stamper->stampLDecoratorChar(
            it,
            commandLine[c].kind,
            xpos,
            dialogText.y+(dialogText.h)-cStart,
            renderer,
            maxCols,
            maxRows
          );

        for(
          std::size_t it=0;
          it < multibyteLenght(commandLine[c].content);
          ++it, ++xpos
        )
          stamper->stampInnerChar(
            multibyteCharacterByIndex(commandLine[c].content, it),
            commandLine[c].kind,
            xpos,
            dialogText.y+(dialogText.h)-cStart,
            renderer,
            maxCols,
            maxRows
          );

        for(
          std::size_t it=0;
            it < stamper->rDecoratorsLength(commandLine[c].kind);
          ++it, ++xpos
        )
          stamper->stampRDecoratorChar(
            it,
            commandLine[c].kind,
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
                stamper->rDecoratorsLength(commandLine[c].kind)
              )*defaultCWidth
            );
          stamper->stampIp(renderer, dest.x, dest.y);
        } else if (
          wordRShift==(int)multibyteLenght(commandLine[c].content)
        ) {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+
                stamper->lDecoratorsLength(commandLine[c].kind)
              )*defaultCWidth
            );
          stamper->stampIp(renderer, dest.x, dest.y);
        } else {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+wordL-wordRShift-
                stamper->rDecoratorsLength(commandLine[c].kind)
              )*defaultCWidth
            );
          stamper->stampIp(renderer, dest.x, dest.y);
        }
      }

      cLineLenght += 1 +
        multibyteLenght(commandLine[c].content) +
        stamper->decoratorsLength(commandLine[c].kind);

    }

  }

  // End of command line .......
}

} // namespace roguedm_gui
