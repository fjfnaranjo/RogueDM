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

  resetCommand();
  historyBackup = roguedm::Sentence();

  emptyWord.content = RDM_WEMPTY;
  emptyWord.kind = RDM_WCLASS_NORMAL;

}

CommandComposer::~CommandComposer () {}

roguedm::Command CommandComposer::getCommand() {
  if(hasCommand())
    return sentence2Command(sentence);
  else {

    // Auto insert a psay command on command-less lines
    if(commandLength()>0) {
      roguedm::Command newPsayCommand;
      newPsayCommand.name = RDM_CMD_PSAY;
      newPsayCommand.params = sentence;
      return newPsayCommand;
    }

    return roguedm::Command();

  }
}

roguedm::Sentence CommandComposer::getRawSentence() {
  return sentence;
}

void CommandComposer::setCommand(const roguedm::Command& command) {
  resetCommand();
  sentence = command2Sentence(command);
}

int CommandComposer::commandLength() {
  int limit = sentence.size();
  int cLineLenght = 1;
  for(int c = 0; c < limit; c++)
    cLineLenght += 1 +
      multibyteLenght(sentence[c].content) +
      multibyteLenght(roguedm::wordKinds[sentence[c].kind].lDecorator) +
      multibyteLenght(roguedm::wordKinds[sentence[c].kind].rDecorator);
  return cLineLenght - 1;
}

bool CommandComposer::hasCommand() {
  if(0!=commandLength() && sentence[0].kind == RDM_WCLASS_COMMAND)
    return true;
  return false;
}

void CommandComposer::keyBackspace() {

  if(
    wordRShift==(int)multibyteLenght(sentence[currentWord].content)
  ) {
    if(currentWord>0) {
      if(
        1==currentWord
        && multibyteLenght(sentence[currentWord].content)!=0
      )
        sentence[currentWord-1].kind = RDM_WCLASS_NORMAL;
      wordRShift=multibyteLenght(sentence[currentWord].content);
      sentence[currentWord-1].content =
        sentence[currentWord-1].content +
        sentence[currentWord].content;
      sentence.erase(sentence.begin()+currentWord);
      currentWord--;
      }
  } else {
    if(0==currentWord)
      sentence[0].kind = RDM_WCLASS_NORMAL;
    sentence[currentWord].content =
      multibyteSubstr(
        sentence[currentWord].content,
        0,
        multibyteLenght(sentence[currentWord].content)-wordRShift-1
      ) +
      multibyteSubstr(
        sentence[currentWord].content,
        multibyteLenght(sentence[currentWord].content)-wordRShift,
        wordRShift
      );
  }
  historyCurrent = 0;

}

void CommandComposer::keyDelete() {

  if(wordRShift==0) {
    if((currentWord+1)<(int)sentence.size()) {
      if(0==currentWord && multibyteLenght(sentence[1].content)!=0)
        sentence[0].kind = RDM_WCLASS_NORMAL;
      wordRShift=multibyteLenght(sentence[currentWord+1].content);
      sentence[currentWord].content =
        sentence[currentWord].content +
        sentence[currentWord+1].content;
      sentence.erase(sentence.begin()+currentWord+1);
    }
  } else {
    if(0==currentWord)
      sentence[0].kind = RDM_WCLASS_NORMAL;
    sentence[currentWord].content =
      multibyteSubstr(
        sentence[currentWord].content,
        0,
        multibyteLenght(sentence[currentWord].content)-wordRShift
      ) +
      multibyteSubstr(
        sentence[currentWord].content,
        multibyteLenght(sentence[currentWord].content)-
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
      wordRShift<(int)multibyteLenght(sentence[currentWord].content)
    )
      wordRShift++;
    else if (currentWord>0) {
      currentWord--;
      wordRShift=0;
    }
    historyCurrent = 0;
  } else {
    if(
      wordRShift<(int)multibyteLenght(sentence[currentWord].content)
    ) {
      wordRShift = multibyteLenght(sentence[currentWord].content);
    } else if (currentWord>0) {
      currentWord--;
      wordRShift=0;
    }
    historyCurrent = 0;
  }

}

void CommandComposer::keyRight(bool fullWord) {

  // TODO: Fix fullWord implementation.
  if (!fullWord) {
    if(wordRShift>0)
      wordRShift--;
    else if (currentWord<((int)sentence.size()-1)) {
      currentWord++;
      wordRShift = multibyteLenght(sentence[currentWord].content);
    }
    historyCurrent = 0;
  } else {
    if(wordRShift>0) {
      wordRShift = 0;
    } else if ((currentWord+1)<(int)sentence.size()) {
      currentWord++;
      wordRShift = multibyteLenght(sentence[currentWord].content);
    }
    historyCurrent = 0;
  }

}

void CommandComposer::keyHome() {

  currentWord = 0;
  wordRShift = multibyteLenght(sentence[currentWord].content);
  historyCurrent = 0;

}

void CommandComposer::keyEnd() {

  currentWord = sentence.size()-1;
  wordRShift = 0;
  historyCurrent = 0;

}

void CommandComposer::keyUp() {

  if(!history.empty()) {
    if(0==historyCurrent) {
      historyBackup = sentence;
      historyCurrent = history.size();
      sentence = history[historyCurrent-1];
      currentWord = sentence.size()-1;
      wordRShift = 0;
    } else if (historyCurrent>1) {
      historyCurrent--;
      sentence = history[historyCurrent-1];
      currentWord = sentence.size()-1;
      wordRShift = 0;
    }
  }

}

void CommandComposer::keyDown() {

  if(!history.empty()) {
    if(historyCurrent) {
      if(historyCurrent<(int)history.size()) {
        historyCurrent++;
        sentence = history[historyCurrent-1];
        currentWord = sentence.size()-1;
        wordRShift = 0;
      } else {
        historyCurrent=0;
        resetCommand();
        sentence = historyBackup;
        currentWord = sentence.size()-1;
        wordRShift = 0;
      }
    }
  }

}

void CommandComposer::keySpace() {

  if(wordRShift==0) {
    sentence.insert(
      sentence.begin()+currentWord+1,
      emptyWord
    );
    currentWord++;
  } else {
    sentence.insert(
      sentence.begin()+currentWord+1,
      emptyWord
    );
    sentence[currentWord+1].content = multibyteSubstr(
      sentence[currentWord].content,
      multibyteLenght(sentence[currentWord].content)-wordRShift,
      wordRShift
    );
    sentence[currentWord].content = multibyteSubstr(
      sentence[currentWord].content,
      0,
      multibyteLenght(sentence[currentWord].content)-wordRShift
    );
    sentence[currentWord].kind = RDM_WCLASS_NORMAL;
    currentWord++;
    wordRShift = multibyteLenght(sentence[currentWord].content);

  }
  historyCurrent = 0;

}

void CommandComposer::keyCharacter(std::string character) {

  if(0==currentWord)
    sentence[currentWord].kind = RDM_WCLASS_NORMAL;
  if(wordRShift==0) {
    sentence[currentWord].content += character;
  } else {
    std::string leftPart = multibyteSubstr(
      sentence[currentWord].content,
      0,
      multibyteLenght(sentence[currentWord].content)-wordRShift
    );
    std::string rightPart = multibyteSubstr(
      sentence[currentWord].content,
      multibyteLenght(sentence[currentWord].content)-wordRShift,
      wordRShift
    );
    sentence[currentWord].content = leftPart;
    sentence[currentWord].content += character;
    sentence[currentWord].content += rightPart;
  }
  historyCurrent = 0;

}

void CommandComposer::commandHistoryPush(roguedm::Command command) {

  if(0!=historyCurrent)
    resetCommand();

  history.push_back(command2Sentence(command));

  // Limit the command historic size
  if(history.size()>RDM_CL_MAX_HISTORY)
    history.erase(history.begin());

}

void CommandComposer::resetCommand() {

  historyCurrent = 0;
  sentence.clear();
  sentence.push_back(emptyWord);
  currentWord = 0;
  wordRShift = 0;

}

void CommandComposer::paintCommandLine(
  SDL_Renderer *renderer, std::shared_ptr<CharmapStamper> stamper,
  int maxCols, int maxRows, int defaultCWidth, int defaultCHeight
) {

  SDL_Rect dialogMain;
  SDL_Rect dialogCell;
  SDL_Rect dialogCreature;
  SDL_Rect dialogText;

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
  int limit = sentence.size();
  int cStart=0;
  int cSkip=0;
  int cLineLenght=0;
  SDL_Rect dest;

  for(int c = 0; c < limit; c++) {

    cLineLenght += 1 +
      multibyteLenght(sentence[c].content) +
      stamper->decoratorsLength(sentence[c].kind);

    if(cLineLenght>dialogText.w) {

      cLineLenght = 1 +
        multibyteLenght(sentence[c].content) +
        stamper->decoratorsLength(sentence[c].kind);

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
        multibyteLenght(sentence[c].content) +
        stamper->decoratorsLength(sentence[c].kind);
      if(cLineLenght>dialogText.w) {
        --cSkip;
        cLineLenght = 0;
      }
    }

    if(cSkip==0) {

      int nextLineLength = cLineLenght + 1 +
        multibyteLenght(sentence[c].content) +
        stamper->decoratorsLength(sentence[c].kind);
      if(nextLineLength>dialogText.w) {
        cLineLenght = 0;
        --cStart;
      }

      int wordL = multibyteLenght(sentence[c].content) +
          stamper->decoratorsLength(sentence[c].kind);

      int xpos=dialogText.x+cLineLenght;

      if (wordL>0) {
        for(
          std::size_t it=0;
            it < stamper->lDecoratorsLength(sentence[c].kind);
          ++it, ++xpos
        )
          stamper->stampLDecoratorChar(
            it,
            sentence[c].kind,
            xpos,
            dialogText.y+(dialogText.h)-cStart,
            renderer,
            maxCols,
            maxRows
          );

        for(
          std::size_t it=0;
          it < multibyteLenght(sentence[c].content);
          ++it, ++xpos
        )
          stamper->stampInnerChar(
            multibyteCharacterByIndex(sentence[c].content, it),
            sentence[c].kind,
            xpos,
            dialogText.y+(dialogText.h)-cStart,
            renderer,
            maxCols,
            maxRows
          );

        for(
          std::size_t it=0;
            it < stamper->rDecoratorsLength(sentence[c].kind);
          ++it, ++xpos
        )
          stamper->stampRDecoratorChar(
            it,
            sentence[c].kind,
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
                stamper->rDecoratorsLength(sentence[c].kind)
              )*defaultCWidth
            );
          stamper->stampIp(renderer, dest.x, dest.y);
        } else if (
          wordRShift==(int)multibyteLenght(sentence[c].content)
        ) {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+
                stamper->lDecoratorsLength(sentence[c].kind)
              )*defaultCWidth
            );
          stamper->stampIp(renderer, dest.x, dest.y);
        } else {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+wordL-wordRShift-
                stamper->rDecoratorsLength(sentence[c].kind)
              )*defaultCWidth
            );
          stamper->stampIp(renderer, dest.x, dest.y);
        }
      }

      cLineLenght += 1 +
        multibyteLenght(sentence[c].content) +
        stamper->decoratorsLength(sentence[c].kind);

    }

  }

  // End of command line .......
}

roguedm::Sentence CommandComposer::command2Sentence(roguedm::Command command) {
  roguedm::Word newCommand;
  newCommand.content = command.name;
  newCommand.kind = RDM_WCLASS_COMMAND;
  roguedm::Sentence newSentence;
  newSentence.insert(newSentence.begin(), newCommand);
  for(auto const &word: command.params)
    newSentence.insert(newSentence.end(), word);
  return newSentence;
}

roguedm::Command CommandComposer::sentence2Command(
  roguedm::Sentence a_sentence
) {
  if(
    a_sentence.size()>0
    && sentence[0].kind == RDM_WCLASS_COMMAND
    && sentence[0].content.size()>0
  ) {
    roguedm::Command newCommand;
    newCommand.name = a_sentence[0].content;
    if(sentence.size()>1) {
      auto sentenceCopy = a_sentence;
      sentenceCopy.erase(sentenceCopy.begin());
      newCommand.params = sentenceCopy;
    } else
      newCommand.params = roguedm::Sentence();
    return newCommand;
  } else
    return roguedm::Command();
}

} // namespace roguedm_gui
