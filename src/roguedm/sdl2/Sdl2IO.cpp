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
#include <cwchar>
#include <locale>
#include <vector>

#include <SDL2/SDL_image.h>

#include "../macros.hpp"
#include "../gettext.hpp"
#include "../paths.hpp"
#include "../strings.hpp"

namespace roguedm_gui {

// Repaint window
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
  drawBox        (2,0,0,maxCols,maxRows);
  dialogMain.x = 1;
  dialogMain.y = 1;
  dialogMain.w = (int)(maxCols-(maxCols/4));
  dialogMain.h = (int)(maxRows-(maxRows/3));
  dialogText.x = 1;
  dialogText.y = dialogMain.h+3;
  dialogText.w = dialogMain.w;
  dialogText.h = maxRows-dialogMain.h-4;
  drawVSeparator(2,dialogText.w+2,0,maxRows);
  drawHSeparator(2,0,dialogText.y-1,dialogText.w+2);
  dialogCell.x = dialogText.w+3;
  dialogCell.y = 1;
  dialogCell.w = maxCols-dialogMain.w-4;
  dialogCell.h = (int)(maxRows-((maxRows/8)*2));
  drawHSeparator(2,dialogCell.x-1,dialogCell.y+dialogCell.h+1,dialogCell.w+2);
  dialogCreature.x = dialogCell.x;
  dialogCreature.y = dialogCell.y+dialogCell.h+2;
  dialogCreature.w = dialogCell.w;
  dialogCreature.h = dialogCell.h*2;
  drawHSeparator(
    2,dialogCreature.x-1,dialogCreature.y+dialogCreature.h+1,dialogCreature.w+2
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
      multibyteLenght(wordTypes[commandLine[c].wordClass].lDecorator) +
      multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator);

    if(cLineLenght>dialogText.w) {

      cLineLenght = 1 +
        multibyteLenght(commandLine[c].wordContent) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].lDecorator) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator);

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
        multibyteLenght(wordTypes[commandLine[c].wordClass].lDecorator) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator);
      if(cLineLenght>dialogText.w) {
        --cSkip;
        cLineLenght = 0;
      }
    }

    if(cSkip==0) {

      int nextLineLength = cLineLenght + 1 +
        multibyteLenght(commandLine[c].wordContent) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].lDecorator) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator);
      if(nextLineLength>dialogText.w) {
        cLineLenght = 0;
        --cStart;
      }

      int wordL = multibyteLenght(commandLine[c].wordContent) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].lDecorator) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator);

      int xpos=dialogText.x+cLineLenght;

      if (wordL>0) {
        for(
          std::size_t it=0;
          it < multibyteLenght(wordTypes[commandLine[c].wordClass].lDecorator);
          ++it, ++xpos
        )
          stampChar(
            transChar(multibyteCharacterByIndex(wordTypes[commandLine[c].wordClass].lDecorator, it)),
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart
          );

        for(
          std::size_t it=0;
          it < multibyteLenght(commandLine[c].wordContent);
          ++it, ++xpos
        )
          stampChar(
            transChar(multibyteCharacterByIndex(commandLine[c].wordContent, it)),
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart
          );

        for(
          std::size_t it=0;
          it < multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator);
          ++it, ++xpos
        )
          stampChar(
            transChar(multibyteCharacterByIndex(wordTypes[commandLine[c].wordClass].rDecorator, it)),
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart
          );

      }

      dest.y = (dialogText.y+(dialogText.h)-cStart)*txtCHeight;
      dest.w = txtCWidth;
      dest.h = txtCHeight;
      if(c==currentWord) {
        if(wordRShift==0) {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+wordL-
                  multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator)
              )*txtCWidth
            );
          SDL_RenderCopy(renderer,ipI,NULL,&dest);
        } else if (wordRShift==(int)multibyteLenght(commandLine[c].wordContent)) {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+
                  multibyteLenght(wordTypes[commandLine[c].wordClass].lDecorator)
              )*txtCWidth
            );
          SDL_RenderCopy(renderer,ipI,NULL,&dest);
        } else {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+wordL-wordRShift-
                  multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator)
              )*txtCWidth
            );
          SDL_RenderCopy(renderer,ipI,NULL,&dest);
        }
      }

      cLineLenght += 1 +
        multibyteLenght(commandLine[c].wordContent) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].lDecorator) +
        multibyteLenght(wordTypes[commandLine[c].wordClass].rDecorator);

    }

  }

  // End of command line .......

  // TODO: Paint the command history if fits

  SDL_RenderPresent(renderer);

}

int Sdl2IO::mustHalt() {
  return appDone;
}

int Sdl2IO::processCommand(const roguedm::Sentence& a) {
  if(a[0].wordContent==RDM_CMD_QUIT && a[0].wordClass==RDM_WCLASS_COMMAND) {
    appDone = 1;
    return RDM_COMMAND_DONE;
  }
  return RDM_COMMAND_UNKNOWN;
}

const int Sdl2IO::autocomplete(roguedm::Sentence& a) {

  // quit command completion
  if(a[0].wordContent==RDM_CMD_QUIT && a[0].wordClass==RDM_WCLASS_NORMAL) {
    a[0].wordClass=RDM_WCLASS_COMMAND;
    return RDM_COMMAND_AC_COMPLETED;
  }

  return RDM_COMMAND_AC_NEXT;

}
const roguedm::SentenceListReference Sdl2IO::autocompleteListOptions(
  const roguedm::Sentence& a
) {

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

Sdl2IO::Sdl2IO() {

  errorCode = 0;
  appDone = 0;

  // SDL init checks
  if(0!=SDL_Init(SDL_INIT_VIDEO)) {
    errorCode = 1;
    SDL_LogError(
      SDL_LOG_CATEGORY_APPLICATION,
      _(RDM_STR_SDL_ERROR),
      SDL_GetError()
    );
    return;
  }

  int createStatus = SDL_CreateWindowAndRenderer(
    800, 500,
    SDL_WINDOW_RESIZABLE,
    &window,
    &renderer
  );

  if(createStatus || NULL==window || NULL==renderer) {
    errorCode = 2;
    SDL_LogError(
      SDL_LOG_CATEGORY_APPLICATION,
      _(RDM_STR_SDL_ERROR),
      SDL_GetError()
    );
    SDL_Quit();
    return;
  }

  // Clear color
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  // Words, comand line and history init
  initWordTypes();
  initTransTable();
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
  initScreenSize();
  SDL_ShowCursor(true);
  SDL_StartTextInput();

}

Sdl2IO::~Sdl2IO() {

  if(0!=errorCode)
    return;

  // Clear word type textures
  SDL_DestroyTexture(ipI);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NORMAL].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_COMMAND].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ALLIED].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_NEUTRAL].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ENEMY].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_PLACE].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT_MAGIC].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT_SET].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT_UNIQ].charsTexture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT_EPIC].charsTexture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

}

int Sdl2IO::getErrorCode() {
  return errorCode;
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

void Sdl2IO::initWordTypes() {

  // Load base texture
  SDL_Surface *baseTexture;
  baseTexture = IMG_Load(RDM_PATH_SHARE "/imgs/codepage-850-9-14.png");

  // Insertion point rects
  SDL_Rect orig, dest;
  orig.x=279; orig.y=28; orig.w=9; orig.h=14;
  dest.x=0;dest.y=0;

  // Insertion point surface creation
  SDL_Surface *ipI_surface;
  ipI_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,orig.w,orig.h,32,0,0,0,0);

  // Insertion point surface blitting
  SDL_BlitSurface(baseTexture,&orig,ipI_surface,&dest);
  SDL_SetColorKey(ipI_surface,true,SDL_MapRGB(ipI_surface->format,0,0,0));
  ipI = SDL_CreateTextureFromSurface(renderer, ipI_surface);

  // Texture data
  txtCpr=32;
  txtHeight=112;
  txtWidth=288;
  txtCHeight=14;
  txtCWidth=9;
  txtHSep=0;
  txtWSep=0;
  txtHStart=0;
  txtWStart=0;

  // Define word type colors
  wordTypes[RDM_WCLASS_NORMAL].color.r = 200;
  wordTypes[RDM_WCLASS_NORMAL].color.g = 200;
  wordTypes[RDM_WCLASS_NORMAL].color.b = 200;
  wordTypes[RDM_WCLASS_NORMAL].clearColor.r = 0;
  wordTypes[RDM_WCLASS_NORMAL].clearColor.g = 0;
  wordTypes[RDM_WCLASS_NORMAL].clearColor.b = 0;
  wordTypes[RDM_WCLASS_NORMAL].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NORMAL].rDecorator = u8"";
  wordTypes[RDM_WCLASS_NORMAL].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_NORMAL].charsTexture,
    wordTypes[RDM_WCLASS_NORMAL].color.r,
    wordTypes[RDM_WCLASS_NORMAL].color.g,
    wordTypes[RDM_WCLASS_NORMAL].color.b,
    wordTypes[RDM_WCLASS_NORMAL].clearColor.r,
    wordTypes[RDM_WCLASS_NORMAL].clearColor.g,
    wordTypes[RDM_WCLASS_NORMAL].clearColor.b
  );
  wordTypes[RDM_WCLASS_COMMAND].color.r = 255;
  wordTypes[RDM_WCLASS_COMMAND].color.g = 255;
  wordTypes[RDM_WCLASS_COMMAND].color.b = 255;
  wordTypes[RDM_WCLASS_COMMAND].clearColor.r = 0;
  wordTypes[RDM_WCLASS_COMMAND].clearColor.g = 0;
  wordTypes[RDM_WCLASS_COMMAND].clearColor.b = 0;
  wordTypes[RDM_WCLASS_COMMAND].lDecorator = u8"\\";
  wordTypes[RDM_WCLASS_COMMAND].rDecorator = u8"";
  wordTypes[RDM_WCLASS_COMMAND].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_COMMAND].charsTexture,
    wordTypes[RDM_WCLASS_COMMAND].color.r,
    wordTypes[RDM_WCLASS_COMMAND].color.g,
    wordTypes[RDM_WCLASS_COMMAND].color.b,
    wordTypes[RDM_WCLASS_COMMAND].clearColor.r,
    wordTypes[RDM_WCLASS_COMMAND].clearColor.g,
    wordTypes[RDM_WCLASS_COMMAND].clearColor.b
  );
  wordTypes[RDM_WCLASS_NPC_ALLIED].color.r = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].color.g = 128;
  wordTypes[RDM_WCLASS_NPC_ALLIED].color.b = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].clearColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].clearColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].clearColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].lDecorator = u8"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED].rDecorator = u8"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_NPC_ALLIED].charsTexture,
    wordTypes[RDM_WCLASS_NPC_ALLIED].color.r,
    wordTypes[RDM_WCLASS_NPC_ALLIED].color.g,
    wordTypes[RDM_WCLASS_NPC_ALLIED].color.b,
    wordTypes[RDM_WCLASS_NPC_ALLIED].clearColor.r,
    wordTypes[RDM_WCLASS_NPC_ALLIED].clearColor.g,
    wordTypes[RDM_WCLASS_NPC_ALLIED].clearColor.b
  );
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].color.r = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].color.g = 255;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].color.b = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].clearColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].clearColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].clearColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].lDecorator = u8"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].rDecorator = u8"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].charsTexture,
    wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].color.r,
    wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].color.g,
    wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].color.b,
    wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].clearColor.r,
    wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].clearColor.g,
    wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].clearColor.b
  );
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].color.r = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].color.g = 64;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].color.b = 255;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].clearColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].clearColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].clearColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].rDecorator = u8"?";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_NPC_NEUTRAL].charsTexture,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL].color.r,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL].color.g,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL].color.b,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL].clearColor.r,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL].clearColor.g,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL].clearColor.b
  );
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].color.r = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].color.g = 192;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].color.b = 255;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].clearColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].clearColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].clearColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].rDecorator = u8"?";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].charsTexture,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].color.r,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].color.g,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].color.b,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].clearColor.r,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].clearColor.g,
    wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].clearColor.b
  );
  wordTypes[RDM_WCLASS_NPC_ENEMY].color.r = 255;
  wordTypes[RDM_WCLASS_NPC_ENEMY].color.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].color.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].clearColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].clearColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].clearColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_ENEMY].rDecorator = u8"!!";
  wordTypes[RDM_WCLASS_NPC_ENEMY].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_NPC_ENEMY].charsTexture,
    wordTypes[RDM_WCLASS_NPC_ENEMY].color.r,
    wordTypes[RDM_WCLASS_NPC_ENEMY].color.g,
    wordTypes[RDM_WCLASS_NPC_ENEMY].color.b,
    wordTypes[RDM_WCLASS_NPC_ENEMY].clearColor.r,
    wordTypes[RDM_WCLASS_NPC_ENEMY].clearColor.g,
    wordTypes[RDM_WCLASS_NPC_ENEMY].clearColor.b
  );
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].color.r = 128;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].color.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].color.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].clearColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].clearColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].clearColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].rDecorator = u8"!";
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].charsTexture,
    wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].color.r,
    wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].color.g,
    wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].color.b,
    wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].clearColor.r,
    wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].clearColor.g,
    wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].clearColor.b
  );
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].color.r = 255;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].color.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].color.b = 192;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].clearColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].clearColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].clearColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].rDecorator = u8"!!!";
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].charsTexture,
    wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].color.r,
    wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].color.g,
    wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].color.b,
    wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].clearColor.r,
    wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].clearColor.g,
    wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].clearColor.b
  );
  wordTypes[RDM_WCLASS_PLACE].color.r = 255;
  wordTypes[RDM_WCLASS_PLACE].color.g = 128;
  wordTypes[RDM_WCLASS_PLACE].color.b = 0;
  wordTypes[RDM_WCLASS_PLACE].clearColor.r = 0;
  wordTypes[RDM_WCLASS_PLACE].clearColor.g = 0;
  wordTypes[RDM_WCLASS_PLACE].clearColor.b = 0;
  wordTypes[RDM_WCLASS_PLACE].lDecorator = u8"[";
  wordTypes[RDM_WCLASS_PLACE].rDecorator = u8"]";
  wordTypes[RDM_WCLASS_PLACE].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_PLACE].charsTexture,
    wordTypes[RDM_WCLASS_PLACE].color.r,
    wordTypes[RDM_WCLASS_PLACE].color.g,
    wordTypes[RDM_WCLASS_PLACE].color.b,
    wordTypes[RDM_WCLASS_PLACE].clearColor.r,
    wordTypes[RDM_WCLASS_PLACE].clearColor.g,
    wordTypes[RDM_WCLASS_PLACE].clearColor.b
  );
  wordTypes[RDM_WCLASS_OBJECT].color.r = 224;
  wordTypes[RDM_WCLASS_OBJECT].color.g = 224;
  wordTypes[RDM_WCLASS_OBJECT].color.b = 0;
  wordTypes[RDM_WCLASS_OBJECT].clearColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT].clearColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT].clearColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT].rDecorator = u8"}";
  wordTypes[RDM_WCLASS_OBJECT].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_OBJECT].charsTexture,
    wordTypes[RDM_WCLASS_OBJECT].color.r,
    wordTypes[RDM_WCLASS_OBJECT].color.g,
    wordTypes[RDM_WCLASS_OBJECT].color.b,
    wordTypes[RDM_WCLASS_OBJECT].clearColor.r,
    wordTypes[RDM_WCLASS_OBJECT].clearColor.g,
    wordTypes[RDM_WCLASS_OBJECT].clearColor.b
  );
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].color.r = 255;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].color.g = 255;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].color.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].clearColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].clearColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].clearColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].rDecorator = u8"}*";
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_OBJECT_MAGIC].charsTexture,
    wordTypes[RDM_WCLASS_OBJECT_MAGIC].color.r,
    wordTypes[RDM_WCLASS_OBJECT_MAGIC].color.g,
    wordTypes[RDM_WCLASS_OBJECT_MAGIC].color.b,
    wordTypes[RDM_WCLASS_OBJECT_MAGIC].clearColor.r,
    wordTypes[RDM_WCLASS_OBJECT_MAGIC].clearColor.g,
    wordTypes[RDM_WCLASS_OBJECT_MAGIC].clearColor.b
  );
  wordTypes[RDM_WCLASS_OBJECT_SET].color.r = 192;
  wordTypes[RDM_WCLASS_OBJECT_SET].color.g = 255;
  wordTypes[RDM_WCLASS_OBJECT_SET].color.b = 64;
  wordTypes[RDM_WCLASS_OBJECT_SET].clearColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT_SET].clearColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT_SET].clearColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_SET].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT_SET].rDecorator = u8"}s";
  wordTypes[RDM_WCLASS_OBJECT_SET].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_OBJECT_SET].charsTexture,
    wordTypes[RDM_WCLASS_OBJECT_SET].color.r,
    wordTypes[RDM_WCLASS_OBJECT_SET].color.g,
    wordTypes[RDM_WCLASS_OBJECT_SET].color.b,
    wordTypes[RDM_WCLASS_OBJECT_SET].clearColor.r,
    wordTypes[RDM_WCLASS_OBJECT_SET].clearColor.g,
    wordTypes[RDM_WCLASS_OBJECT_SET].clearColor.b
  );
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].color.r = 192;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].color.g = 192;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].color.b = 64;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].clearColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].clearColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].clearColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].rDecorator = u8"}**";
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_OBJECT_UNIQ].charsTexture,
    wordTypes[RDM_WCLASS_OBJECT_UNIQ].color.r,
    wordTypes[RDM_WCLASS_OBJECT_UNIQ].color.g,
    wordTypes[RDM_WCLASS_OBJECT_UNIQ].color.b,
    wordTypes[RDM_WCLASS_OBJECT_UNIQ].clearColor.r,
    wordTypes[RDM_WCLASS_OBJECT_UNIQ].clearColor.g,
    wordTypes[RDM_WCLASS_OBJECT_UNIQ].clearColor.b
  );
  wordTypes[RDM_WCLASS_OBJECT_EPIC].color.r = 255;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].color.g = 255;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].color.b = 192;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].clearColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].clearColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].clearColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT_EPIC].rDecorator = u8"}***";
  wordTypes[RDM_WCLASS_OBJECT_EPIC].charsTexture = SDL_CreateTextureFromSurface(renderer, baseTexture);

  colorizeWordType(
    wordTypes[RDM_WCLASS_OBJECT_EPIC].charsTexture,
    wordTypes[RDM_WCLASS_OBJECT_EPIC].color.r,
    wordTypes[RDM_WCLASS_OBJECT_EPIC].color.g,
    wordTypes[RDM_WCLASS_OBJECT_EPIC].color.b,
    wordTypes[RDM_WCLASS_OBJECT_EPIC].clearColor.r,
    wordTypes[RDM_WCLASS_OBJECT_EPIC].clearColor.g,
    wordTypes[RDM_WCLASS_OBJECT_EPIC].clearColor.b
  );

  // Free reference surfaces
  SDL_FreeSurface(baseTexture);
  SDL_FreeSurface(ipI_surface);

}

void Sdl2IO::initTransTable() {

  transUtf8[u8" "] = 32;
  transUtf8[u8"!"] = 33;
  transUtf8[u8"\""]= 34;
  transUtf8[u8"#"] = 35;
  transUtf8[u8"$"] = 36;
  transUtf8[u8"%"] = 37;
  transUtf8[u8"&"] = 38;
  transUtf8[u8"'"] = 39;
  transUtf8[u8"("] = 40;
  transUtf8[u8")"] = 41;
  transUtf8[u8"*"] = 42;
  transUtf8[u8"+"] = 43;
  transUtf8[u8","] = 44;
  transUtf8[u8"-"] = 45;
  transUtf8[u8"."] = 46;
  transUtf8[u8"/"] = 47;

  transUtf8[u8"0"] = 48;
  transUtf8[u8"1"] = 49;
  transUtf8[u8"2"] = 50;
  transUtf8[u8"3"] = 51;
  transUtf8[u8"4"] = 52;
  transUtf8[u8"5"] = 53;
  transUtf8[u8"6"] = 54;
  transUtf8[u8"7"] = 55;
  transUtf8[u8"8"] = 56;
  transUtf8[u8"9"] = 57;
  transUtf8[u8":"] = 58;
  transUtf8[u8";"] = 59;
  transUtf8[u8"<"] = 60;
  transUtf8[u8"="] = 61;
  transUtf8[u8">"] = 62;
  transUtf8[u8"?"] = 63;

  transUtf8[u8"@"] = 64;
  transUtf8[u8"A"] = 65;
  transUtf8[u8"B"] = 66;
  transUtf8[u8"C"] = 67;
  transUtf8[u8"D"] = 68;
  transUtf8[u8"E"] = 69;
  transUtf8[u8"F"] = 70;
  transUtf8[u8"G"] = 71;
  transUtf8[u8"H"] = 72;
  transUtf8[u8"I"] = 73;
  transUtf8[u8"J"] = 74;
  transUtf8[u8"K"] = 75;
  transUtf8[u8"L"] = 76;
  transUtf8[u8"M"] = 77;
  transUtf8[u8"N"] = 78;
  transUtf8[u8"O"] = 79;

  transUtf8[u8"P"] = 80;
  transUtf8[u8"Q"] = 81;
  transUtf8[u8"R"] = 82;
  transUtf8[u8"S"] = 83;
  transUtf8[u8"T"] = 84;
  transUtf8[u8"U"] = 85;
  transUtf8[u8"V"] = 86;
  transUtf8[u8"W"] = 87;
  transUtf8[u8"X"] = 88;
  transUtf8[u8"Y"] = 89;
  transUtf8[u8"Z"] = 90;
  transUtf8[u8"["] = 91;
  transUtf8[u8"\\"]= 92;
  transUtf8[u8"]"] = 93;
  transUtf8[u8"^"] = 94;
  transUtf8[u8"_"] = 95;

  transUtf8[u8"`"] = 96;
  transUtf8[u8"a"] = 97;
  transUtf8[u8"b"] = 98;
  transUtf8[u8"c"] = 99;
  transUtf8[u8"d"] = 100;
  transUtf8[u8"e"] = 101;
  transUtf8[u8"f"] = 102;
  transUtf8[u8"g"] = 103;
  transUtf8[u8"h"] = 104;
  transUtf8[u8"i"] = 105;
  transUtf8[u8"j"] = 106;
  transUtf8[u8"k"] = 107;
  transUtf8[u8"l"] = 108;
  transUtf8[u8"m"] = 109;
  transUtf8[u8"n"] = 110;
  transUtf8[u8"o"] = 111;

  transUtf8[u8"p"] = 112;
  transUtf8[u8"q"] = 113;
  transUtf8[u8"r"] = 114;
  transUtf8[u8"s"] = 115;
  transUtf8[u8"t"] = 116;
  transUtf8[u8"u"] = 117;
  transUtf8[u8"v"] = 118;
  transUtf8[u8"w"] = 119;
  transUtf8[u8"x"] = 120;
  transUtf8[u8"y"] = 121;
  transUtf8[u8"z"] = 122;
  transUtf8[u8"{"] = 123;
  transUtf8[u8"|"] = 124;
  transUtf8[u8"}"] = 125;
  transUtf8[u8"~"] = 126;

  transUtf8[u8"\u00c7"] = 128;
  transUtf8[u8"\u00fc"] = 129;
  transUtf8[u8"\u00e9"] = 130;
  transUtf8[u8"\u00e7"] = 135;
  transUtf8[u8"\u00c9"] = 144;
  transUtf8[u8"\u00e1"] = 160;
  transUtf8[u8"\u00ed"] = 161;
  transUtf8[u8"\u00f3"] = 162;
  transUtf8[u8"\u00fa"] = 163;
  transUtf8[u8"\u00f1"] = 164;
  transUtf8[u8"\u00d1"] = 165;
  transUtf8[u8"\u00aa"] = 166;
  transUtf8[u8"\u00ba"] = 167;
  transUtf8[u8"\u00bf"] = 168;
  transUtf8[u8"\u00a1"] = 173;
  transUtf8[u8"\u00c1"] = 181;
  transUtf8[u8"\u20ac"] = 213;
  transUtf8[u8"\u00cd"] = 214;
  transUtf8[u8"\u00d3"] = 224;
  transUtf8[u8"\u00da"] = 233;

}

void Sdl2IO::colorizeWordType(
  SDL_Texture* srf,
  int fgr,
  int fgg,
  int fgb,
  int bgr,
  int bgg,
  int bgb)
{
  // TODO: SDL2Migration Add support for colorization
  return;
  int i;
  SDL_Color colors[256];
  for(i=0;i<256;i++){
    colors[i].r=0;
    colors[i].g=0;
    colors[i].b=0;
  }
  colors[0].r=fgr;
  colors[0].g=fgg;
  colors[0].b=fgb;
  colors[1].r=bgr;
  colors[1].g=bgg;
  colors[1].b=bgb;
  // SDL_SetPalette(srf, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);

}

// Recalculate window coords and zones after term resize
void Sdl2IO::initScreenSize() {
  int ww, wh;
  SDL_GetWindowSize(window, &ww, &wh);
  maxCols=floor(ww/txtCWidth)-1;
  maxRows=floor(wh/txtCHeight)-1;
}

void Sdl2IO::drawBox(
  int t,
  int x,
  int y,
  int w,
  int h
) {

  int c;

  switch(t) {

    case 1:
      stampChar(218,0,x,y);
      stampChar(191,0,x+w,y);
      stampChar(192,0,x,y+h);
      stampChar(217,0,x+w,y+h);
      for(c=x+1;c<x+w;c++) stampChar(197,0,c,y);
      for(c=x+1;c<x+w;c++) stampChar(197,0,c,y+h);
      for(c=y+1;c<y+h;c++) stampChar(197,0,x,c);
      for(c=y+1;c<y+h;c++) stampChar(197,0,x+w,c);
      break;
    case 2:
      stampChar(201,0,x,y);
      stampChar(187,0,x+w,y);
      stampChar(200,0,x,y+h);
      stampChar(188,0,x+w,y+h);
      for(c=x+1;c<x+w;c++) stampChar(205,0,c,y);
      for(c=x+1;c<x+w;c++) stampChar(205,0,c,y+h);
      for(c=y+1;c<y+h;c++) stampChar(186,0,x,c);
      for(c=y+1;c<y+h;c++) stampChar(186,0,x+w,c);
      break;
    case 3:
      stampChar(201,0,x,y);
      stampChar(187,0,x+w,y);
      stampChar(200,0,x,y+h);
      stampChar(188,0,x+w,y+h);
      for(c=x+1;c<x+w;c++) stampChar(206,0,c,y);
      for(c=x+1;c<x+w;c++) stampChar(206,0,c,y+h);
      for(c=y+1;c<y+h;c++) stampChar(206,0,x,c);
      for(c=y+1;c<y+h;c++) stampChar(206,0,x+w,c);
      break;
    default:
      stampChar(218,0,x,y);
      stampChar(191,0,x+w,y);
      stampChar(192,0,x,y+h);
      stampChar(217,0,x+w,y+h);
      for(c=x+1;c<x+w;c++) stampChar(196,0,c,y);
      for(c=x+1;c<x+w;c++) stampChar(196,0,c,y+h);
      for(c=y+1;c<y+h;c++) stampChar(179,0,x,c);
      for(c=y+1;c<y+h;c++) stampChar(179,0,x+w,c);
      break;

  }

}

void Sdl2IO::drawHSeparator(
  int t,
  int x,
  int y,
  int s
) {

  int c;

  switch(t) {

    case 1:
      stampChar(195,0,x,y);
      stampChar(180,0,x+s,y);
      for(c=x+1;c<x+s;c++) stampChar(197,0,c,y);
      break;
    case 2:
      stampChar(204,0,x,y);
      stampChar(185,0,x+s,y);
      for(c=x+1;c<x+s;c++) stampChar(205,0,c,y);
      break;
    case 3:
      stampChar(204,0,x,y);
      stampChar(185,0,x+s,y);
      for(c=x+1;c<x+s;c++) stampChar(206,0,c,y);
      break;
    default:
      stampChar(195,0,x,y);
      stampChar(180,0,x+s,y);
      for(c=x+1;c<x+s;c++) stampChar(196,0,c,y);
      break;

  }

}

void Sdl2IO::drawVSeparator(
  int t,
  int x,
  int y,
  int s
) {

  int c;

  switch(t) {

    case 1:
      stampChar(194,0,x,y);
      stampChar(193,0,x,y+s);
      for(c=y+1;c<y+s;c++) stampChar(197,0,x,c);
      break;
    case 2:
      stampChar(203,0,x,y);
      stampChar(202,0,x,y+s);
      for(c=y+1;c<y+s;c++) stampChar(186,0,x,c);
      break;
    case 3:
      stampChar(203,0,x,y);
      stampChar(202,0,x,y+s);
      for(c=y+1;c<y+s;c++) stampChar(206,0,x,c);
      break;
    default:
      stampChar(194,0,x,y);
      stampChar(193,0,x,y+s);
      for(c=y+1;c<y+s;c++) stampChar(179,0,x,c);
      break;

  }

}

void Sdl2IO::drawCross(
  int t,
  int x,
  int y
) {
  stampChar((t==0||t==1)?197:206,0,x,y);
}

int Sdl2IO::transChar(std::string c) {
  for (auto const & entry : transUtf8)
    if (0==c.compare(entry.first))
      return entry.second;
    // TODO: Remove magic constant
    return 254;
}

void Sdl2IO::stampChar(
  int cNumber,
  int wType,
  int dx,
  int dy
) {

  while(cNumber>255) cNumber-=256;
  while(dx>maxCols) dx-=maxCols+1;
  while(dy>maxRows) dy-=maxRows+1;
  int columnNumber = cNumber % txtCpr;
  int rowNumber = cNumber / txtCpr;
  SDL_Rect srcrect;
  srcrect.x=txtWStart+(txtWSep*columnNumber)+(txtCWidth*columnNumber);
  srcrect.y=txtHStart+(txtHSep*rowNumber)+(txtCHeight*rowNumber);
  srcrect.w=txtCWidth;
  srcrect.h=txtCHeight;
  SDL_Rect dstrect;
  dstrect.x=dx*txtCWidth;
  dstrect.y=dy*txtCHeight;
  dstrect.w=txtCWidth;
  dstrect.h=txtCHeight;
  SDL_RenderCopy(renderer,wordTypes[wType].charsTexture,&srcrect,&dstrect);

}
// Process the user keyboard input
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
            initScreenSize();
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
      if(wordRShift==(int)multibyteLenght(commandLine[currentWord].wordContent)) {
        if(currentWord>0) {
          if(1==currentWord && multibyteLenght(commandLine[currentWord].wordContent)!=0)
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
        if(wordRShift<(int)multibyteLenght(commandLine[currentWord].wordContent))
          wordRShift++;
        else if (currentWord>0) {
          currentWord--;
          wordRShift=0;
        }
        historyCurrent = 0;
      } else {
        if(wordRShift<(int)multibyteLenght(commandLine[currentWord].wordContent)) {
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

void Sdl2IO::setDefaultWord(roguedm::Word c) {
  defaultWord = c;
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

std::size_t Sdl2IO::multibyteLenght(const std::string &string) {
  std::size_t characterCount = 0;
  int currentShift = 0;
  int bytesReaded = 0;
  do {
    currentShift += bytesReaded;
    bytesReaded = mbrtowc(NULL, &string[currentShift], MB_CUR_MAX, NULL);
    if(bytesReaded>0) characterCount++;
  } while (bytesReaded>0);
  return (bytesReaded<0) ? bytesReaded : characterCount;
}

std::string Sdl2IO::multibyteCharacterByIndex(const std::string &string, const std::size_t position) {
  std::string lastCharacterString(MB_CUR_MAX, '\0');
  wchar_t lastCharacter;
  std::size_t characterCount = 0;
  std::size_t characterSize = 0;
  int currentShift = 0;
  int bytesReaded = 0;
  do {
    currentShift += bytesReaded;
    bytesReaded = mbrtowc(&lastCharacter, &string[currentShift], MB_CUR_MAX, NULL);
    if(bytesReaded>0) characterCount++;
    if((characterCount-1)==position) {
      characterSize = wcrtomb(&lastCharacterString[0], lastCharacter, NULL);
      lastCharacterString.resize(characterSize);
      return lastCharacterString;
    }
  } while (bytesReaded>0);
  return u8"";
}

std::string Sdl2IO::multibyteSubstr(const std::string &string, const std::size_t start, const std::size_t size) {
  if(size==0)
    return u8"";
  std::string newString;
  int startByte = 0;
  std::size_t characterCount = 0;
  int currentShift = 0;
  int bytesReaded = 0;
  do {
    currentShift += bytesReaded;
    bytesReaded = mbrtowc(NULL, &string[currentShift], MB_CUR_MAX, NULL);
    if(bytesReaded>0) characterCount++;
    if((characterCount-1)==start && bytesReaded>0)
      startByte = currentShift;
    if((characterCount-1)==start+size)
      return string.substr(startByte, currentShift-startByte);
    if(bytesReaded==0 && startByte>0)
      return string.substr(startByte, currentShift-startByte);
    if(bytesReaded==0)
      return u8"";
  } while (bytesReaded>0);
  return u8"";
}

} // namespace roguedm_gui
