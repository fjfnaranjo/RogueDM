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

#include "CharmapStamper.hpp"

#include <SDL2/SDL_image.h>

#include "../strings.hpp"

namespace roguedm_gui {

CharmapStamper::CharmapStamper () {
  config = roguedm::Config::instance();
  txtCHeight = 0;
  txtCWidth = 0;
  txtCpr = 0;
  txtHeight = 0;
  txtWidth = 0;
  txtHSep = 0;
  txtWSep = 0;
  txtHStart = 0;
  txtWStart = 0;
  ipI = nullptr;
}

CharmapStamper::~CharmapStamper() {
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

void CharmapStamper::loadDefaultCharmap(SDL_Renderer *renderer, std::string defaultCharmap) {

  // Default charmap texture file path
  std::string defaultCharmapPath =
    std::string(RDM_PATH_SHARE) +
    std::string(RDM_PATH_SEP) +
    config->getSettingValue(
      defaultCharmap, "path", "imgs/codepage-850-9-14.png"
    );

  // Texture data
  txtCHeight = config->getSettingIntValue(defaultCharmap, "txtCHeight", 14);
  txtCWidth = config->getSettingIntValue(defaultCharmap, "txtCWidth", 9);
  txtCpr = config->getSettingIntValue(defaultCharmap, "txtCpr", 32);
  txtHeight = config->getSettingIntValue(defaultCharmap, "txtHeight", 112);
  txtWidth = config->getSettingIntValue(defaultCharmap, "txtWidth", 288);
  txtHSep = config->getSettingIntValue(defaultCharmap, "txtHSep", 0);
  txtWSep = config->getSettingIntValue(defaultCharmap, "txtWSep", 0);
  txtHStart = config->getSettingIntValue(defaultCharmap, "txtHStart", 0);
  txtWStart = config->getSettingIntValue(defaultCharmap, "txtWStart", 0);

  // Insertion point rects
  SDL_Rect orig, dest;
  orig.x = config->getSettingIntValue(defaultCharmap, "txtIpXStart", 279);
  orig.y = config->getSettingIntValue(defaultCharmap, "txtIpYStart", 28);
  orig.w = txtCWidth;
  orig.h = txtCHeight;
  dest.x = 0;
  dest.y = 0;

  // Load base texture
  SDL_Surface *baseTexture;
  baseTexture = IMG_Load(defaultCharmapPath.c_str());

  // Insertion point surface creation
  SDL_Surface *ipI_surface;
  ipI_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,orig.w,orig.h,32,0,0,0,0);

  // Insertion point surface blitting
  SDL_BlitSurface(baseTexture,&orig,ipI_surface,&dest);
  SDL_SetColorKey(ipI_surface,true,SDL_MapRGB(ipI_surface->format,0,0,0));
  ipI = SDL_CreateTextureFromSurface(renderer, ipI_surface);

  colorizeTexture(renderer, baseTexture);

  // Free reference surfaces
  SDL_FreeSurface(baseTexture);
  SDL_FreeSurface(ipI_surface);

  initTransTable();

}

int CharmapStamper::decoratorsLength(int idx) const {
  return
    multibyteLenght(wordTypes[idx].lDecorator) +
    multibyteLenght(wordTypes[idx].rDecorator);
}

int CharmapStamper::lDecoratorsLength(int idx) const {
  return multibyteLenght(wordTypes[idx].lDecorator);
}

int CharmapStamper::rDecoratorsLength(int idx) const {
  return multibyteLenght(wordTypes[idx].rDecorator);
}

int CharmapStamper::getCHeight() const {
  return txtCHeight;
}
int CharmapStamper::getCWidth() const {
  return txtCWidth;
}

void CharmapStamper::stampInnerChar(
  std::string c,
  int t,
  int x,
  int y,
  SDL_Renderer* renderer,
  int mx,
  int my
) {
  stampChar(transChar(c), t, x, y, renderer, mx, my);
}

void CharmapStamper::stampLDecoratorChar(
  int idx,
  int t,
  int x,
  int y,
  SDL_Renderer* renderer,
  int mx,
  int my
) {

  stampChar(transChar(multibyteCharacterByIndex(wordTypes[t].lDecorator, idx)), t, x, y, renderer, mx, my);
}

void CharmapStamper::stampRDecoratorChar(
  int idx,
  int t,
  int x,
  int y,
  SDL_Renderer* renderer,
  int mx,
  int my
) {
  stampChar(transChar(multibyteCharacterByIndex(wordTypes[t].rDecorator, idx)), t, x, y, renderer, mx, my);
}

void CharmapStamper::stampIp(SDL_Renderer *renderer, int x, int y) {
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  dest.w = txtCWidth;
  dest.h = txtCHeight;
  SDL_RenderCopy(renderer,ipI,NULL,&dest);
}

void CharmapStamper::drawBox(
  int t,
  int x,
  int y,
  int w,
  int h,
  SDL_Renderer *renderer
) {

  int c;

  switch(t) {

    case 1:
      mStampChar(218,0,x,y,renderer);
      mStampChar(191,0,x+w,y,renderer);
      mStampChar(192,0,x,y+h,renderer);
      mStampChar(217,0,x+w,y+h,renderer);
      for(c=x+1;c<x+w;c++) mStampChar(197,0,c,y,renderer);
      for(c=x+1;c<x+w;c++) mStampChar(197,0,c,y+h,renderer);
      for(c=y+1;c<y+h;c++) mStampChar(197,0,x,c,renderer);
      for(c=y+1;c<y+h;c++) mStampChar(197,0,x+w,c,renderer);
      break;
    case 2:
      mStampChar(201,0,x,y,renderer);
      mStampChar(187,0,x+w,y,renderer);
      mStampChar(200,0,x,y+h,renderer);
      mStampChar(188,0,x+w,y+h,renderer);
      for(c=x+1;c<x+w;c++) mStampChar(205,0,c,y,renderer);
      for(c=x+1;c<x+w;c++) mStampChar(205,0,c,y+h,renderer);
      for(c=y+1;c<y+h;c++) mStampChar(186,0,x,c,renderer);
      for(c=y+1;c<y+h;c++) mStampChar(186,0,x+w,c,renderer);
      break;
    case 3:
      mStampChar(201,0,x,y,renderer);
      mStampChar(187,0,x+w,y,renderer);
      mStampChar(200,0,x,y+h,renderer);
      mStampChar(188,0,x+w,y+h,renderer);
      for(c=x+1;c<x+w;c++) mStampChar(206,0,c,y,renderer);
      for(c=x+1;c<x+w;c++) mStampChar(206,0,c,y+h,renderer);
      for(c=y+1;c<y+h;c++) mStampChar(206,0,x,c,renderer);
      for(c=y+1;c<y+h;c++) mStampChar(206,0,x+w,c,renderer);
      break;
    default:
      mStampChar(218,0,x,y,renderer);
      mStampChar(191,0,x+w,y,renderer);
      mStampChar(192,0,x,y+h,renderer);
      mStampChar(217,0,x+w,y+h,renderer);
      for(c=x+1;c<x+w;c++) mStampChar(196,0,c,y,renderer);
      for(c=x+1;c<x+w;c++) mStampChar(196,0,c,y+h,renderer);
      for(c=y+1;c<y+h;c++) mStampChar(179,0,x,c,renderer);
      for(c=y+1;c<y+h;c++) mStampChar(179,0,x+w,c,renderer);
      break;

  }

}

void CharmapStamper::drawHSeparator(
  int t,
  int x,
  int y,
  int s,
  SDL_Renderer *renderer
) {

  int c;

  switch(t) {

    case 1:
      mStampChar(195,0,x,y,renderer);
      mStampChar(180,0,x+s,y,renderer);
      for(c=x+1;c<x+s;c++) mStampChar(197,0,c,y,renderer);
      break;
    case 2:
      mStampChar(204,0,x,y,renderer);
      mStampChar(185,0,x+s,y,renderer);
      for(c=x+1;c<x+s;c++) mStampChar(205,0,c,y,renderer);
      break;
    case 3:
      mStampChar(204,0,x,y,renderer);
      mStampChar(185,0,x+s,y,renderer);
      for(c=x+1;c<x+s;c++) mStampChar(206,0,c,y,renderer);
      break;
    default:
      mStampChar(195,0,x,y,renderer);
      mStampChar(180,0,x+s,y,renderer);
      for(c=x+1;c<x+s;c++) mStampChar(196,0,c,y,renderer);
      break;

  }

}

void CharmapStamper::drawVSeparator(
  int t,
  int x,
  int y,
  int s,
  SDL_Renderer *renderer
) {

  int c;

  switch(t) {

    case 1:
      mStampChar(194,0,x,y,renderer);
      mStampChar(193,0,x,y+s,renderer);
      for(c=y+1;c<y+s;c++) mStampChar(197,0,x,c,renderer);
      break;
    case 2:
      mStampChar(203,0,x,y,renderer);
      mStampChar(202,0,x,y+s,renderer);
      for(c=y+1;c<y+s;c++) mStampChar(186,0,x,c,renderer);
      break;
    case 3:
      mStampChar(203,0,x,y,renderer);
      mStampChar(202,0,x,y+s,renderer);
      for(c=y+1;c<y+s;c++) mStampChar(206,0,x,c,renderer);
      break;
    default:
      mStampChar(194,0,x,y,renderer);
      mStampChar(193,0,x,y+s,renderer);
      for(c=y+1;c<y+s;c++) mStampChar(179,0,x,c,renderer);
      break;

  }

}

void CharmapStamper::drawCross(
  int t,
  int x,
  int y,
  SDL_Renderer *renderer
) {
  mStampChar((t==0||t==1)?197:206,0,x,y,renderer);
}

void CharmapStamper::colorizeTexture(SDL_Renderer *renderer, SDL_Surface *baseTexture) {

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

}

void CharmapStamper::initTransTable() {

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

void CharmapStamper::colorizeWordType(
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

int CharmapStamper::transChar(std::string c) {
  for (auto const & entry : transUtf8)
    if (0==c.compare(entry.first))
      return entry.second;
    // TODO: Remove magic constant
    return 254;
}

void CharmapStamper::stampChar(
  int cNumber,
  int wType,
  int dx,
  int dy,
  SDL_Renderer *renderer,
  int maxCols,
  int maxRows
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

void CharmapStamper::mStampChar(
  int cNumber,
  int wType,
  int dx,
  int dy,
  SDL_Renderer *renderer
) {

  while(cNumber>255) cNumber-=256;
  //while(dx>maxCols) dx-=maxCols+1;
  //while(dy>maxRows) dy-=maxRows+1;
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

} // namespace roguedm_gui
