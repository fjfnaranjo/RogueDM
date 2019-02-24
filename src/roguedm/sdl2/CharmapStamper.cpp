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

#include <SDL_image.h>

#include "mbtools.hpp"
#include "../commands/Word.hpp"
#include "../strings.hpp"

namespace roguedm_gui {

CharmapStamper::CharmapStamper () {

  initSuccess = false;

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
  resetCharmapStamper();
}

void CharmapStamper::resetCharmapStamper() {
  if(initSuccess) {
    for(int kind = 0; kind < RDM_WCLASS_TOTAL; kind++)
      SDL_DestroyTexture(wordKindTexture[kind]);
    initSuccess = false;
  }
}

bool CharmapStamper::loadDefaultCharmap(
  SDL_Renderer *renderer,
  std::string defaultCharmap
) {

  // Default charmap texture file path
  std::string defaultCharmapPath =
    std::string(RDM_PATH_HERE) +
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

  initSuccess = true;
  return true;

}

unsigned int CharmapStamper::decoratorsLength(int idx) const {
  return
    multibyteLenght(roguedm::wordKinds[idx].lDecorator) +
    multibyteLenght(roguedm::wordKinds[idx].rDecorator);
}

unsigned int CharmapStamper::lDecoratorsLength(int idx) const {
  return multibyteLenght(roguedm::wordKinds[idx].lDecorator);
}

unsigned int CharmapStamper::rDecoratorsLength(int idx) const {
  return multibyteLenght(roguedm::wordKinds[idx].rDecorator);
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
    transChar(multibyteCharacterByIndex(roguedm::wordKinds[t].lDecorator, idx)),
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
    transChar(multibyteCharacterByIndex(roguedm::wordKinds[t].rDecorator, idx)),
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
  for(int kind = 0; kind < RDM_WCLASS_TOTAL; kind++)
    wordKindTexture.insert(wordKindTexture.end(), colorizeWordType(
      renderer,
      baseSurface,
      roguedm::wordKinds[kind].fgColor.r,
      roguedm::wordKinds[kind].fgColor.g,
      roguedm::wordKinds[kind].fgColor.b/*,
      roguedm::wordKinds[kind].bgColor.r,
      roguedm::wordKinds[kind].bgColor.g,
      roguedm::wordKinds[kind].bgColor.b*/
    ));
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
  int fgb/*,
  int bgr,
  int bgg,
  int bgb*/
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
  // TODO: Remove magic constants like this one
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
  SDL_RenderCopy(renderer,wordKindTexture[t],&srcrect,&dstrect);
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
  SDL_RenderCopy(renderer,wordKindTexture[t],&srcrect,&dstrect);
}

} // namespace roguedm_gui
