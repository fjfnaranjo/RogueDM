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

/**
 * \file IOLocal.cpp
 * \brief File containing the IOLocal class definitions.
 */

#include "IOLocal.hpp"

namespace roguedm {

// Singleton instance manager.
IOLocal* IOLocal::instance(int instanceMode) {

  static IOLocal *instance;

  if(instance) {
    switch(instanceMode) {
      case RDM_IOLOCAL_MODE_RESET:
        delete instance;
        instance = new IOLocal();
        break;
      case RDM_IOLOCAL_MODE_DELETE:
        delete instance;
      case RDM_IOLOCAL_MODE_CREATE:
        break;
      default:
        return 0;
        break;
    }
  } else {
    switch(instanceMode) {
      case RDM_IOLOCAL_MODE_CREATE:
        instance = new IOLocal();
        break;
      case RDM_IOLOCAL_MODE_DELETE:
      case RDM_IOLOCAL_MODE_RESET:
      default:
        return 0;
        break;
    }
  }

  return instance;

}

// Repaint window
void IOLocal::update() {

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
      commandLine[c].wordContent.length() +
      wordTypes[commandLine[c].wordClass].lDecorator.length() +
      wordTypes[commandLine[c].wordClass].rDecorator.length();

    if(cLineLenght>dialogText.w) {

      cLineLenght = 1 +
        commandLine[c].wordContent.length() +
        wordTypes[commandLine[c].wordClass].lDecorator.length() +
        wordTypes[commandLine[c].wordClass].rDecorator.length();

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
        commandLine[c].wordContent.length() +
        wordTypes[commandLine[c].wordClass].lDecorator.length() +
        wordTypes[commandLine[c].wordClass].rDecorator.length();
      if(cLineLenght>dialogText.w) {
        --cSkip;
        cLineLenght = 0;
      }
    }

    if(cSkip==0) {

      int nextLineLength = cLineLenght + 1 +
        commandLine[c].wordContent.length() +
        wordTypes[commandLine[c].wordClass].lDecorator.length() +
        wordTypes[commandLine[c].wordClass].rDecorator.length();
      if(nextLineLength>dialogText.w) {
        cLineLenght = 0;
        --cStart;
      }

      int wordL = commandLine[c].wordContent.length() +
        wordTypes[commandLine[c].wordClass].lDecorator.length() +
        wordTypes[commandLine[c].wordClass].rDecorator.length();

      int xpos=dialogText.x+cLineLenght;

      if (wordL>0) {
        for(
          int it=0;
          it < (int)wordTypes[commandLine[c].wordClass].lDecorator.length();
          ++it, ++xpos
        )
          stampChar(
            transChar(wordTypes[commandLine[c].wordClass].lDecorator[it]),
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart
          );

        for(
          int it=0;
          it < (int)commandLine[c].wordContent.length();
          ++it, ++xpos
        )
          stampChar(
            transChar(commandLine[c].wordContent[it]),
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart
          );

        for(
          int it=0;
          it < (int)wordTypes[commandLine[c].wordClass].rDecorator.length();
          ++it, ++xpos
        )
          stampChar(
            transChar(wordTypes[commandLine[c].wordClass].rDecorator[it]),
            commandLine[c].wordClass,
            xpos,
            dialogText.y+(dialogText.h)-cStart
          );

      }

      dest.y = (dialogText.y+(dialogText.h)-cStart)*txtCHeight;
      if(c==currentWord) {
        if(wordRShift==0) {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+wordL-
                  wordTypes[commandLine[c].wordClass].rDecorator.length()
              )*txtCWidth
            )-(txtCWidth/2);
          SDL_RenderCopy(renderer,ipI,NULL,&dest);
        } else if (wordRShift==(int)commandLine[c].wordContent.length()) {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+
                  wordTypes[commandLine[c].wordClass].lDecorator.length()
              )*txtCWidth
            )-(txtCWidth/2);
          SDL_RenderCopy(renderer,ipI,NULL,&dest);
        } else {
          dest.x =
            (
              (
                dialogText.x+cLineLenght+wordL-wordRShift-
                  wordTypes[commandLine[c].wordClass].rDecorator.length()
              )*txtCWidth
            )-(txtCWidth/2);
          SDL_RenderCopy(renderer,ipI,NULL,&dest);
        }
      }

      cLineLenght += 1 +
        commandLine[c].wordContent.length() +
        wordTypes[commandLine[c].wordClass].lDecorator.length() +
        wordTypes[commandLine[c].wordClass].rDecorator.length();

    }

  }

  // End of command line .......

  // TODO: Paint the command history if fits

  SDL_RenderPresent(renderer);

}

int IOLocal::mustHalt() {
  return appDone;
}

int IOLocal::processCommand(const Sentence& a) {
  if(a[0].wordContent==RDM_CMD_QUIT && a[0].wordClass==RDM_WCLASS_COMMAND) {
    appDone = 1;
    return 1;
  }
  return 0;
}

const int IOLocal::autocomplete(Sentence& a) {

  // quit command completion
  if(a[0].wordContent==RDM_CMD_QUIT && a[0].wordClass==RDM_WCLASS_NORMAL) {
    a[0].wordClass=RDM_WCLASS_COMMAND;
    return 1;
  }

  return 0;

}
const std::vector<Sentence>* IOLocal::autocompleteListOptions(const Sentence& a)
{

  Word psayCmd;
  psayCmd.wordContent = RDM_CMD_PSAY;
  psayCmd.wordClass = RDM_WCLASS_COMMAND;

  Word quitCmd;
  quitCmd.wordContent = RDM_CMD_QUIT;
  quitCmd.wordClass = RDM_WCLASS_COMMAND;

  if(a.size()==1 && a[0].wordContent.length()==0) {

    std::vector<Sentence> *l = new std::vector<Sentence>;

    Sentence *o = new Sentence;

    o->push_back(psayCmd);
    l->push_back(*o);

    o->push_back(quitCmd);
    l->push_back(*o);

    delete o;

    return l;

  }

  return new std::vector<Sentence>;

}

IOLocal::IOLocal() {

  errorCode = 0;
  appDone = 0;

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
  Word newDefaultWord;
  newDefaultWord.wordContent = RDM_CMD_PSAY;
  newDefaultWord.wordClass = RDM_WCLASS_COMMAND;
  setDefaultWord(newDefaultWord);

  // Local command handler
  registerCommandHandler(this);

  // More SDL Init
  initScreenSize();
  SDL_ShowCursor(true);

}

IOLocal::~IOLocal() {

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

}

int IOLocal::getErrorCode() {
  return errorCode;
}

void IOLocal::resetLine() {
  Word emptyWord;
  emptyWord.wordContent = L"";
  emptyWord.wordClass = RDM_WCLASS_NORMAL;
  commandLine.clear();
  commandLine.push_back(emptyWord);
  currentWord = 0;
  wordRShift = 0;
}

void IOLocal::initWordTypes() {

  // Load base texture
  SDL_Surface *baseTexture;
  baseTexture = IMG_Load("env/share/imgs/page850bw.gif");

  // Insertion point rects
  SDL_Rect orig, dest;
  orig.x=84; orig.y=98; orig.w=7; orig.h=14;
  dest.x=0;dest.y=0;

  // Insertion point surface creation
  SDL_Surface *ipI_surface;
  ipI_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,orig.w,orig.h,32,0,0,0,0);

  // Insertion point surface blitting
  SDL_BlitSurface(baseTexture,&orig,ipI_surface,&dest);
  SDL_SetColorKey(ipI_surface,true,SDL_MapRGB(ipI_surface->format,0,0,0));
  ipI = SDL_CreateTextureFromSurface(renderer, ipI_surface);

  // Texture data
  txtCpr=16;
  txtHeight=112;
  txtWidth=224;
  txtCHeight=14;
  txtCWidth=7;
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
  wordTypes[RDM_WCLASS_NORMAL].lDecorator = L"";
  wordTypes[RDM_WCLASS_NORMAL].rDecorator = L"";
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
  wordTypes[RDM_WCLASS_COMMAND].lDecorator = L"\\";
  wordTypes[RDM_WCLASS_COMMAND].rDecorator = L"";
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
  wordTypes[RDM_WCLASS_NPC_ALLIED].lDecorator = L"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED].rDecorator = L"'";
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
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].lDecorator = L"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].rDecorator = L"'";
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
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].lDecorator = L"";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].rDecorator = L"?";
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
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].lDecorator = L"";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].rDecorator = L"?";
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
  wordTypes[RDM_WCLASS_NPC_ENEMY].lDecorator = L"";
  wordTypes[RDM_WCLASS_NPC_ENEMY].rDecorator = L"!!";
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
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].lDecorator = L"";
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].rDecorator = L"!";
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
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].lDecorator = L"";
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].rDecorator = L"!!!";
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
  wordTypes[RDM_WCLASS_PLACE].lDecorator = L"[";
  wordTypes[RDM_WCLASS_PLACE].rDecorator = L"]";
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
  wordTypes[RDM_WCLASS_OBJECT].lDecorator = L"{";
  wordTypes[RDM_WCLASS_OBJECT].rDecorator = L"}";
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
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].lDecorator = L"{";
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].rDecorator = L"}*";
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
  wordTypes[RDM_WCLASS_OBJECT_SET].lDecorator = L"{";
  wordTypes[RDM_WCLASS_OBJECT_SET].rDecorator = L"}s";
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
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].lDecorator = L"{";
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].rDecorator = L"}**";
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
  wordTypes[RDM_WCLASS_OBJECT_EPIC].lDecorator = L"{";
  wordTypes[RDM_WCLASS_OBJECT_EPIC].rDecorator = L"}***";
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

void IOLocal::initTransTable() {

  transUtf8[L' '] = 32;
  transUtf8[L'!'] = 33;
  transUtf8[L'\"']= 34;
  transUtf8[L'#'] = 35;
  transUtf8[L'$'] = 36;
  transUtf8[L'%'] = 37;
  transUtf8[L'&'] = 38;
  transUtf8[L'\''] = 39;
  transUtf8[L'('] = 40;
  transUtf8[L')'] = 41;
  transUtf8[L'*'] = 42;
  transUtf8[L'+'] = 43;
  transUtf8[L','] = 44;
  transUtf8[L'-'] = 45;
  transUtf8[L'.'] = 46;
  transUtf8[L'/'] = 47;

  transUtf8[L'0'] = 48;
  transUtf8[L'1'] = 49;
  transUtf8[L'2'] = 50;
  transUtf8[L'3'] = 51;
  transUtf8[L'4'] = 52;
  transUtf8[L'5'] = 53;
  transUtf8[L'6'] = 54;
  transUtf8[L'7'] = 55;
  transUtf8[L'8'] = 56;
  transUtf8[L'9'] = 57;
  transUtf8[L':'] = 58;
  transUtf8[L';'] = 59;
  transUtf8[L'<'] = 60;
  transUtf8[L'='] = 61;
  transUtf8[L'>'] = 62;
  transUtf8[L'?'] = 63;

  transUtf8[L'@'] = 64;
  transUtf8[L'A'] = 65;
  transUtf8[L'B'] = 66;
  transUtf8[L'C'] = 67;
  transUtf8[L'D'] = 68;
  transUtf8[L'E'] = 69;
  transUtf8[L'F'] = 70;
  transUtf8[L'G'] = 71;
  transUtf8[L'H'] = 72;
  transUtf8[L'I'] = 73;
  transUtf8[L'J'] = 74;
  transUtf8[L'K'] = 75;
  transUtf8[L'L'] = 76;
  transUtf8[L'M'] = 77;
  transUtf8[L'N'] = 78;
  transUtf8[L'O'] = 79;

  transUtf8[L'P'] = 80;
  transUtf8[L'Q'] = 81;
  transUtf8[L'R'] = 82;
  transUtf8[L'S'] = 83;
  transUtf8[L'T'] = 84;
  transUtf8[L'U'] = 85;
  transUtf8[L'V'] = 86;
  transUtf8[L'W'] = 87;
  transUtf8[L'X'] = 88;
  transUtf8[L'Y'] = 89;
  transUtf8[L'Z'] = 90;
  transUtf8[L'['] = 91;
  transUtf8[L'\\']= 92;
  transUtf8[L']'] = 93;
  transUtf8[L'^'] = 94;
  transUtf8[L'_'] = 95;

  transUtf8[L'`'] = 96;
  transUtf8[L'a'] = 97;
  transUtf8[L'b'] = 98;
  transUtf8[L'c'] = 99;
  transUtf8[L'd'] = 100;
  transUtf8[L'e'] = 101;
  transUtf8[L'f'] = 102;
  transUtf8[L'g'] = 103;
  transUtf8[L'h'] = 104;
  transUtf8[L'i'] = 105;
  transUtf8[L'j'] = 106;
  transUtf8[L'k'] = 107;
  transUtf8[L'l'] = 108;
  transUtf8[L'm'] = 109;
  transUtf8[L'n'] = 110;
  transUtf8[L'o'] = 111;

  transUtf8[L'p'] = 112;
  transUtf8[L'q'] = 113;
  transUtf8[L'r'] = 114;
  transUtf8[L's'] = 115;
  transUtf8[L't'] = 116;
  transUtf8[L'u'] = 117;
  transUtf8[L'v'] = 118;
  transUtf8[L'w'] = 119;
  transUtf8[L'x'] = 120;
  transUtf8[L'y'] = 121;
  transUtf8[L'z'] = 122;
  transUtf8[L'{'] = 123;
  transUtf8[L'|'] = 124;
  transUtf8[L'}'] = 125;
  transUtf8[L'~'] = 126;

  transUtf8[L'Ç'] = 128;
  transUtf8[L'ü'] = 129;
  transUtf8[L'é'] = 130;
  transUtf8[L'ç'] = 135;
  transUtf8[L'É'] = 144;
  transUtf8[L'á'] = 160;
  transUtf8[L'í'] = 161;
  transUtf8[L'ó'] = 162;
  transUtf8[L'ú'] = 163;
  transUtf8[L'ñ'] = 164;
  transUtf8[L'Ñ'] = 165;
  transUtf8[L'ª'] = 166;
  transUtf8[L'º'] = 167;
  transUtf8[L'¿'] = 168;
  transUtf8[L'¡'] = 173;
  transUtf8[L'Á'] = 181;
  transUtf8[L'€'] = 213;
  transUtf8[L'Í'] = 214;
  transUtf8[L'Ó'] = 224;
  transUtf8[L'Ú'] = 233;

}

void IOLocal::colorizeWordType(
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
void IOLocal::initScreenSize() {
  // TODO: Add support for window resizing
  int ww, wh;
  SDL_GetWindowSize(window, &ww, &wh);
  maxCols=floor(ww/txtCWidth)-1;
  maxRows=floor(wh/txtCHeight)-1;
  update();
}

void IOLocal::drawBox(
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

void IOLocal::drawHSeparator(
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

void IOLocal::drawVSeparator(
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

void IOLocal::drawCross(
  int t,
  int x,
  int y
) {
  stampChar((t==0||t==1)?197:206,0,x,y);
}

int IOLocal::transChar(wchar_t c) {

  std::map<wchar_t,int>::iterator tItt;
  tItt = transUtf8.find(c);
  if (tItt!=transUtf8.end())
    return transUtf8[c];
  else
    return 254;

}

void IOLocal::stampChar(
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
void IOLocal::eventsManager() {

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
      case SDL_KEYDOWN:
        processKey(&event);
        break;
    }
  }

}

void IOLocal::processKey(SDL_Event* event) {

  // TODO: Resolve substitution/replace mode

  Word emptyWord;
  emptyWord.wordContent = RDM_WEMPTY;
  emptyWord.wordClass = RDM_WCLASS_NORMAL;

  SDL_KeyboardEvent kevent = event->key;

  switch(kevent.keysym.sym) {
    // Space
    case SDLK_SPACE:
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
        commandLine[currentWord+1].wordContent =
          commandLine[currentWord].wordContent.substr(
            commandLine[currentWord].wordContent.length()-
              wordRShift,
            wordRShift
          );
        commandLine[currentWord].wordContent =
          commandLine[currentWord].wordContent.substr(
            0,
            commandLine[currentWord].wordContent.length()-wordRShift
          );
        commandLine[currentWord].wordClass = RDM_WCLASS_NORMAL;
        currentWord++;
        wordRShift = commandLine[currentWord].wordContent.length();

      }
      historyCurrent = 0;
      break;

    // Enter
    case SDLK_RETURN:
      processLine();
      break;

    // Backspace
    case SDLK_BACKSPACE:
      if(wordRShift==(int)commandLine[currentWord].wordContent.length()) {
        if(currentWord>0) {
          if(1==currentWord && commandLine[currentWord].wordContent.length()!=0)
            commandLine[currentWord-1].wordClass = RDM_WCLASS_NORMAL;
          wordRShift=commandLine[currentWord].wordContent.length();
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
          commandLine[currentWord].wordContent.substr(
            0,
            commandLine[currentWord].wordContent.length()-
              wordRShift-1
          ) +
          commandLine[currentWord].wordContent.substr(
            commandLine[currentWord].wordContent.length()-
              wordRShift,
            wordRShift
          );
      }
      historyCurrent = 0;
      break;

    // Delete
    case SDLK_DELETE:
      if(wordRShift==0) {
        if((currentWord+1)<(int)commandLine.size()) {
          if(0==currentWord && commandLine[1].wordContent.length()!=0)
            commandLine[0].wordClass = RDM_WCLASS_NORMAL;
          wordRShift=commandLine[currentWord+1].wordContent.length();
          commandLine[currentWord].wordContent =
            commandLine[currentWord].wordContent +
            commandLine[currentWord+1].wordContent;
          commandLine.erase(commandLine.begin()+currentWord+1);
        }
      } else {
        if(0==currentWord)
          commandLine[0].wordClass = RDM_WCLASS_NORMAL;
        commandLine[currentWord].wordContent =
          commandLine[currentWord].wordContent.substr(
            0,
            commandLine[currentWord].wordContent.length()-wordRShift
          ) +
          commandLine[currentWord].wordContent.substr(
            commandLine[currentWord].wordContent.length()-
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
        if(wordRShift<(int)commandLine[currentWord].wordContent.size())
          wordRShift++;
        else if (currentWord>0) {
          currentWord--;
          wordRShift=0;
        }
        historyCurrent = 0;
      } else {
        if(wordRShift<(int)commandLine[currentWord].wordContent.size()) {
          wordRShift = commandLine[currentWord].wordContent.size();
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
          wordRShift = commandLine[currentWord].wordContent.size();
        }
        historyCurrent = 0;
      } else {
        if(wordRShift>0) {
          wordRShift = 0;
        } else if ((currentWord+1)<(int)commandLine.size()) {
          currentWord++;
          wordRShift = commandLine[currentWord].wordContent.size();
        }
        historyCurrent = 0;
      }
      break;

    // Far-long h-movement keys
    case SDLK_HOME:
      currentWord = 0;
      wordRShift = commandLine[currentWord].wordContent.size();
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

    // Disable modificators
    case SDLK_RSHIFT:
    case SDLK_LSHIFT:
    case SDLK_LALT:
    case SDLK_RALT:
    case SDLK_LCTRL:
    case SDLK_RCTRL:
    case SDLK_MODE:
    case SDLK_MENU:
    case SDLK_KP_ENTER:
    case SDLK_ESCAPE:
      break;

    // Other wide characteres
    default:
      // TODO: SDL2New Implement SDL2 text input support
      char* key_char = (char*)SDL_GetKeyName(kevent.keysym.sym);
      key_char[0] = tolower(key_char[0]);
      wchar_t key_wchar;
      mbstowcs(&key_wchar, key_char , 1);
      if(0==currentWord)
        commandLine[currentWord].wordClass = RDM_WCLASS_NORMAL;
      if(wordRShift==0) {
        commandLine[currentWord].wordContent += key_wchar;
      } else {
        std::wstring leftPart = commandLine[currentWord].wordContent.substr(
          0,
          commandLine[currentWord].wordContent.length()-wordRShift
        );
        std::wstring rightPart = commandLine[currentWord].wordContent.substr(
            commandLine[currentWord].wordContent.length()-wordRShift,
            wordRShift
        );
        commandLine[currentWord].wordContent = leftPart;
        commandLine[currentWord].wordContent += key_wchar;
        commandLine[currentWord].wordContent += rightPart;
      }
      historyCurrent = 0;
      break;

  }

}

void IOLocal::tryAutocompletion() {

  // empty word to insert when expanding commands
  Word emptyWord;
  emptyWord.wordContent = RDM_WEMPTY;
  emptyWord.wordClass = RDM_WCLASS_NORMAL;

  const std::vector<Sentence> *options;

  // check for empty line to list all commands
  if(commandLine.size()==1 && commandLine[0].wordContent.length()==0) {

    for(int count = 0, max = commandHandlers.size(); count<max ; count++) {

      options = commandHandlers[count]->autocompleteListOptions(commandLine);

      if(!options->empty()) {
        for(int counter = 0, max = options->size() ; counter<max ; counter++)
          consoleHistory.push_back(options->at(counter));
        delete options;
      }

    }

  }

  // Process action
  for(int count = 0, max = commandHandlers.size(); count<max ; count++) {

    if(1==commandHandlers[count]->autocomplete(commandLine))
      break;

    options = commandHandlers[count]->autocompleteListOptions(commandLine);

    if(!options->empty()) {
      for(int counter = 0, max = options->size() ; counter<max ; counter++)
        consoleHistory.push_back(options->at(counter));
      delete options;
      break;
    }

  }

}

void IOLocal::processLine() {

  // Ignoramos líneas vacias
  if(!commandLine.empty()) {

    // If first word is empty, inserts the say command
    if(commandLine[0].wordContent.length()==0) {
      commandLine.erase(commandLine.begin());
      commandLine.insert(commandLine.begin(),defaultWord);
    }

    // If first word is not a command, inserts the say command
    if(commandLine[0].wordClass!=RDM_WCLASS_COMMAND)
      commandLine.insert(commandLine.begin(),defaultWord);

    // Process action
    for(int count = 0, max = commandHandlers.size(); count<max ; count++)
      if(1==commandHandlers[count]->processCommand(commandLine))
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

void IOLocal::setDefaultWord(Word c) {
  defaultWord = c;
}

void IOLocal::registerCommandHandler(CommandHandlerInterface *c) {
  unregisterCommandHandler(c);
  commandHandlers.push_back(c);
}

void IOLocal::unregisterCommandHandler(CommandHandlerInterface *c) {
  for(int counter=0,max=commandHandlers.size();counter<max;counter++)
    if(commandHandlers[counter]==c)
      commandHandlers.erase(commandHandlers.begin()+counter);
}

} // namespace roguedm
