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

#include "mbtools.hpp"
#include "../strings.hpp"

namespace roguedm_gui {

CharmapStamper::CharmapStamper () {
  config = roguedm::Config::instance();
  txtCHeight = 0;
  txtCWidth = 0;
  txtCpr = 0;
  txtHSep = 0;
  txtWSep = 0;
  txtHStart = 0;
  txtWStart = 0;
}

CharmapStamper::~CharmapStamper() {
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NORMAL].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_COMMAND].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ALLIED].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_NEUTRAL].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ENEMY].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_PLACE].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT_MAGIC].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT_SET].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT_UNIQ].texture);
  SDL_DestroyTexture(wordTypes[RDM_WCLASS_OBJECT_EPIC].texture);
}

bool CharmapStamper::loadDefaultCharmap(
  SDL_Renderer *renderer,
  std::string defaultCharmap
) {

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
  txtHSep = config->getSettingIntValue(defaultCharmap, "txtHSep", 0);
  txtWSep = config->getSettingIntValue(defaultCharmap, "txtWSep", 0);
  txtHStart = config->getSettingIntValue(defaultCharmap, "txtHStart", 0);
  txtWStart = config->getSettingIntValue(defaultCharmap, "txtWStart", 0);

  // Load base texture
  SDL_Surface *baseTexture;
  baseTexture = IMG_Load(defaultCharmapPath.c_str());
  if(!baseTexture)
    return false;

  defineAndColorizeWordTypes(renderer, baseTexture);

  // Free reference surfaces
  SDL_FreeSurface(baseTexture);

  initTransTable();

  return true;

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
  stampChar(
    transChar(multibyteCharacterByIndex(wordTypes[t].lDecorator, idx)),
    t, x, y, renderer, mx, my
  );
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
  stampChar(
    transChar(multibyteCharacterByIndex(wordTypes[t].rDecorator, idx)),
    t, x, y, renderer, mx, my
  );
}

void CharmapStamper::stampIp(SDL_Renderer *renderer, int x, int y) {
  mStampChar(95, 0, x/txtCWidth, y/txtCHeight, renderer);
}

void CharmapStamper::drawBox(
  int x,
  int y,
  int w,
  int h,
  SDL_Renderer *renderer
) {
  mStampChar(201,0,x,y,renderer);
  mStampChar(187,0,x+w,y,renderer);
  mStampChar(200,0,x,y+h,renderer);
  mStampChar(188,0,x+w,y+h,renderer);
  for(int c=x+1;c<x+w;c++) mStampChar(205,0,c,y,renderer);
  for(int c=x+1;c<x+w;c++) mStampChar(205,0,c,y+h,renderer);
  for(int c=y+1;c<y+h;c++) mStampChar(186,0,x,c,renderer);
  for(int c=y+1;c<y+h;c++) mStampChar(186,0,x+w,c,renderer);
}

void CharmapStamper::drawHSeparator(
  int x,
  int y,
  int s,
  SDL_Renderer *renderer
) {
  mStampChar(204,0,x,y,renderer);
  mStampChar(185,0,x+s,y,renderer);
  for(int c=x+1;c<x+s;c++) mStampChar(205,0,c,y,renderer);
}

void CharmapStamper::drawVSeparator(
  int x,
  int y,
  int s,
  SDL_Renderer *renderer
) {
  mStampChar(203,0,x,y,renderer);
  mStampChar(202,0,x,y+s,renderer);
  for(int c=y+1;c<y+s;c++) mStampChar(186,0,x,c,renderer);
}

void CharmapStamper::drawCross(
  int x,
  int y,
  SDL_Renderer *renderer
) {
  mStampChar(206,0,x,y,renderer);
}

void CharmapStamper::defineAndColorizeWordTypes(
  SDL_Renderer *renderer, SDL_Surface *baseSurface
) {

  wordTypes[RDM_WCLASS_NORMAL].fgColor.r = 200;
  wordTypes[RDM_WCLASS_NORMAL].fgColor.g = 200;
  wordTypes[RDM_WCLASS_NORMAL].fgColor.b = 200;
  wordTypes[RDM_WCLASS_NORMAL].bgColor.r = 0;
  wordTypes[RDM_WCLASS_NORMAL].bgColor.g = 0;
  wordTypes[RDM_WCLASS_NORMAL].bgColor.b = 0;
  wordTypes[RDM_WCLASS_NORMAL].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NORMAL].rDecorator = u8"";
  wordTypes[RDM_WCLASS_NORMAL].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_NORMAL].fgColor.r,
      wordTypes[RDM_WCLASS_NORMAL].fgColor.g,
      wordTypes[RDM_WCLASS_NORMAL].fgColor.b,
      wordTypes[RDM_WCLASS_NORMAL].bgColor.r,
      wordTypes[RDM_WCLASS_NORMAL].bgColor.g,
      wordTypes[RDM_WCLASS_NORMAL].bgColor.b
  );

  wordTypes[RDM_WCLASS_COMMAND].fgColor.r = 255;
  wordTypes[RDM_WCLASS_COMMAND].fgColor.g = 255;
  wordTypes[RDM_WCLASS_COMMAND].fgColor.b = 255;
  wordTypes[RDM_WCLASS_COMMAND].bgColor.r = 0;
  wordTypes[RDM_WCLASS_COMMAND].bgColor.g = 0;
  wordTypes[RDM_WCLASS_COMMAND].bgColor.b = 0;
  wordTypes[RDM_WCLASS_COMMAND].lDecorator = u8"\\";
  wordTypes[RDM_WCLASS_COMMAND].rDecorator = u8"";
  wordTypes[RDM_WCLASS_COMMAND].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_COMMAND].fgColor.r,
      wordTypes[RDM_WCLASS_COMMAND].fgColor.g,
      wordTypes[RDM_WCLASS_COMMAND].fgColor.b,
      wordTypes[RDM_WCLASS_COMMAND].bgColor.r,
      wordTypes[RDM_WCLASS_COMMAND].bgColor.g,
      wordTypes[RDM_WCLASS_COMMAND].bgColor.b
  );

  wordTypes[RDM_WCLASS_NPC_ALLIED].fgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].fgColor.g = 128;
  wordTypes[RDM_WCLASS_NPC_ALLIED].fgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].bgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].bgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].bgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED].lDecorator = u8"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED].rDecorator = u8"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_NPC_ALLIED].fgColor.r,
      wordTypes[RDM_WCLASS_NPC_ALLIED].fgColor.g,
      wordTypes[RDM_WCLASS_NPC_ALLIED].fgColor.b,
      wordTypes[RDM_WCLASS_NPC_ALLIED].bgColor.r,
      wordTypes[RDM_WCLASS_NPC_ALLIED].bgColor.g,
      wordTypes[RDM_WCLASS_NPC_ALLIED].bgColor.b
  );

  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].fgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].fgColor.g = 255;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].fgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].bgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].bgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].bgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].lDecorator = u8"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].rDecorator = u8"'";
  wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].fgColor.r,
      wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].fgColor.g,
      wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].fgColor.b,
      wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].bgColor.r,
      wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].bgColor.g,
      wordTypes[RDM_WCLASS_NPC_ALLIED_CMB].bgColor.b
  );

  wordTypes[RDM_WCLASS_NPC_NEUTRAL].fgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].fgColor.g = 64;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].fgColor.b = 255;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].bgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].bgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].bgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].rDecorator = u8"?";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL].fgColor.r,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL].fgColor.g,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL].fgColor.b,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL].bgColor.r,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL].bgColor.g,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL].bgColor.b
  );

  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].fgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].fgColor.g = 192;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].fgColor.b = 255;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].bgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].bgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].bgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].rDecorator = u8"?";
  wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].fgColor.r,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].fgColor.g,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].fgColor.b,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].bgColor.r,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].bgColor.g,
      wordTypes[RDM_WCLASS_NPC_NEUTRAL_CMB].bgColor.b
  );

  wordTypes[RDM_WCLASS_NPC_ENEMY].fgColor.r = 255;
  wordTypes[RDM_WCLASS_NPC_ENEMY].fgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].fgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].bgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].bgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].bgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_ENEMY].rDecorator = u8"!!";
  wordTypes[RDM_WCLASS_NPC_ENEMY].texture =   colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_NPC_ENEMY].fgColor.r,
      wordTypes[RDM_WCLASS_NPC_ENEMY].fgColor.g,
      wordTypes[RDM_WCLASS_NPC_ENEMY].fgColor.b,
      wordTypes[RDM_WCLASS_NPC_ENEMY].bgColor.r,
      wordTypes[RDM_WCLASS_NPC_ENEMY].bgColor.g,
      wordTypes[RDM_WCLASS_NPC_ENEMY].bgColor.b
  );

  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].fgColor.r = 128;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].fgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].fgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].bgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].bgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].bgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].rDecorator = u8"!";
  wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].fgColor.r,
      wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].fgColor.g,
      wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].fgColor.b,
      wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].bgColor.r,
      wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].bgColor.g,
      wordTypes[RDM_WCLASS_NPC_ENEMY_LIGHT].bgColor.b
  );

  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].fgColor.r = 255;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].fgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].fgColor.b = 192;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].bgColor.r = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].bgColor.g = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].bgColor.b = 0;
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].lDecorator = u8"";
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].rDecorator = u8"!!!";
  wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].fgColor.r,
      wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].fgColor.g,
      wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].fgColor.b,
      wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].bgColor.r,
      wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].bgColor.g,
      wordTypes[RDM_WCLASS_NPC_ENEMY_HARD].bgColor.b
  );

  wordTypes[RDM_WCLASS_PLACE].fgColor.r = 255;
  wordTypes[RDM_WCLASS_PLACE].fgColor.g = 128;
  wordTypes[RDM_WCLASS_PLACE].fgColor.b = 0;
  wordTypes[RDM_WCLASS_PLACE].bgColor.r = 0;
  wordTypes[RDM_WCLASS_PLACE].bgColor.g = 0;
  wordTypes[RDM_WCLASS_PLACE].bgColor.b = 0;
  wordTypes[RDM_WCLASS_PLACE].lDecorator = u8"[";
  wordTypes[RDM_WCLASS_PLACE].rDecorator = u8"]";
  wordTypes[RDM_WCLASS_PLACE].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_PLACE].fgColor.r,
      wordTypes[RDM_WCLASS_PLACE].fgColor.g,
      wordTypes[RDM_WCLASS_PLACE].fgColor.b,
      wordTypes[RDM_WCLASS_PLACE].bgColor.r,
      wordTypes[RDM_WCLASS_PLACE].bgColor.g,
      wordTypes[RDM_WCLASS_PLACE].bgColor.b
  );

  wordTypes[RDM_WCLASS_OBJECT].fgColor.r = 224;
  wordTypes[RDM_WCLASS_OBJECT].fgColor.g = 224;
  wordTypes[RDM_WCLASS_OBJECT].fgColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT].bgColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT].bgColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT].bgColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT].rDecorator = u8"}";
  wordTypes[RDM_WCLASS_OBJECT].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_OBJECT].fgColor.r,
      wordTypes[RDM_WCLASS_OBJECT].fgColor.g,
      wordTypes[RDM_WCLASS_OBJECT].fgColor.b,
      wordTypes[RDM_WCLASS_OBJECT].bgColor.r,
      wordTypes[RDM_WCLASS_OBJECT].bgColor.g,
      wordTypes[RDM_WCLASS_OBJECT].bgColor.b
  );

  wordTypes[RDM_WCLASS_OBJECT_MAGIC].fgColor.r = 255;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].fgColor.g = 255;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].fgColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].bgColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].bgColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].bgColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].rDecorator = u8"}*";
  wordTypes[RDM_WCLASS_OBJECT_MAGIC].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_OBJECT_MAGIC].fgColor.r,
      wordTypes[RDM_WCLASS_OBJECT_MAGIC].fgColor.g,
      wordTypes[RDM_WCLASS_OBJECT_MAGIC].fgColor.b,
      wordTypes[RDM_WCLASS_OBJECT_MAGIC].bgColor.r,
      wordTypes[RDM_WCLASS_OBJECT_MAGIC].bgColor.g,
      wordTypes[RDM_WCLASS_OBJECT_MAGIC].bgColor.b
  );

  wordTypes[RDM_WCLASS_OBJECT_SET].fgColor.r = 192;
  wordTypes[RDM_WCLASS_OBJECT_SET].fgColor.g = 255;
  wordTypes[RDM_WCLASS_OBJECT_SET].fgColor.b = 64;
  wordTypes[RDM_WCLASS_OBJECT_SET].bgColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT_SET].bgColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT_SET].bgColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_SET].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT_SET].rDecorator = u8"}s";
  wordTypes[RDM_WCLASS_OBJECT_SET].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_OBJECT_SET].fgColor.r,
      wordTypes[RDM_WCLASS_OBJECT_SET].fgColor.g,
      wordTypes[RDM_WCLASS_OBJECT_SET].fgColor.b,
      wordTypes[RDM_WCLASS_OBJECT_SET].bgColor.r,
      wordTypes[RDM_WCLASS_OBJECT_SET].bgColor.g,
      wordTypes[RDM_WCLASS_OBJECT_SET].bgColor.b
  );

  wordTypes[RDM_WCLASS_OBJECT_UNIQ].fgColor.r = 192;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].fgColor.g = 192;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].fgColor.b = 64;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].bgColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].bgColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].bgColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].rDecorator = u8"}**";
  wordTypes[RDM_WCLASS_OBJECT_UNIQ].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_OBJECT_UNIQ].fgColor.r,
      wordTypes[RDM_WCLASS_OBJECT_UNIQ].fgColor.g,
      wordTypes[RDM_WCLASS_OBJECT_UNIQ].fgColor.b,
      wordTypes[RDM_WCLASS_OBJECT_UNIQ].bgColor.r,
      wordTypes[RDM_WCLASS_OBJECT_UNIQ].bgColor.g,
      wordTypes[RDM_WCLASS_OBJECT_UNIQ].bgColor.b
  );

  wordTypes[RDM_WCLASS_OBJECT_EPIC].fgColor.r = 255;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].fgColor.g = 255;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].fgColor.b = 192;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].bgColor.r = 0;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].bgColor.g = 0;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].bgColor.b = 0;
  wordTypes[RDM_WCLASS_OBJECT_EPIC].lDecorator = u8"{";
  wordTypes[RDM_WCLASS_OBJECT_EPIC].rDecorator = u8"}***";
  wordTypes[RDM_WCLASS_OBJECT_EPIC].texture = colorizeWordType(
      renderer,
      baseSurface,
      wordTypes[RDM_WCLASS_OBJECT_EPIC].fgColor.r,
      wordTypes[RDM_WCLASS_OBJECT_EPIC].fgColor.g,
      wordTypes[RDM_WCLASS_OBJECT_EPIC].fgColor.b,
      wordTypes[RDM_WCLASS_OBJECT_EPIC].bgColor.r,
      wordTypes[RDM_WCLASS_OBJECT_EPIC].bgColor.g,
      wordTypes[RDM_WCLASS_OBJECT_EPIC].bgColor.b
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

SDL_Texture* CharmapStamper::colorizeWordType(
  SDL_Renderer *renderer,
  SDL_Surface *srf,
  int fgr,
  int fgg,
  int fgb,
  int bgr,
  int bgg,
  int bgb
) {
  SDL_Palette *palette = srf->format->palette;
  for(int i=0;i < srf->format->palette->ncolors;i++) {
    palette->colors[i].r=fgr;
    palette->colors[i].g=fgg;
    palette->colors[i].b=fgb;
  }
  return SDL_CreateTextureFromSurface(renderer, srf);
}

int CharmapStamper::transChar(std::string c) {
  for (auto const & entry : transUtf8)
    if (0==c.compare(entry.first))
      return entry.second;
    // TODO: Remove magic constant
    return 254;
}

void CharmapStamper::stampChar(
  int n,
  int t,
  int x,
  int y,
  SDL_Renderer *renderer,
  int maxCols,
  int maxRows
) {
  while(n>255) n-=256;
  while(x>maxCols) x-=maxCols+1;
  while(y>maxRows) y-=maxRows+1;
  int columnNumber = n % txtCpr;
  int rowNumber = n / txtCpr;
  SDL_Rect srcrect;
  srcrect.x=txtWStart+(txtWSep*columnNumber)+(txtCWidth*columnNumber);
  srcrect.y=txtHStart+(txtHSep*rowNumber)+(txtCHeight*rowNumber);
  srcrect.w=txtCWidth;
  srcrect.h=txtCHeight;
  SDL_Rect dstrect;
  dstrect.x=x*txtCWidth;
  dstrect.y=y*txtCHeight;
  dstrect.w=txtCWidth;
  dstrect.h=txtCHeight;
  SDL_RenderCopy(renderer,wordTypes[t].texture,&srcrect,&dstrect);
}

void CharmapStamper::mStampChar(
  int n,
  int t,
  int x,
  int y,
  SDL_Renderer *renderer
) {
  while(n>255) n-=256;
  int columnNumber = n % txtCpr;
  int rowNumber = n / txtCpr;
  SDL_Rect srcrect;
  srcrect.x=txtWStart+(txtWSep*columnNumber)+(txtCWidth*columnNumber);
  srcrect.y=txtHStart+(txtHSep*rowNumber)+(txtCHeight*rowNumber);
  srcrect.w=txtCWidth;
  srcrect.h=txtCHeight;
  SDL_Rect dstrect;
  dstrect.x=x*txtCWidth;
  dstrect.y=y*txtCHeight;
  dstrect.w=txtCWidth;
  dstrect.h=txtCHeight;
  SDL_RenderCopy(renderer,wordTypes[t].texture,&srcrect,&dstrect);
}

} // namespace roguedm_gui
