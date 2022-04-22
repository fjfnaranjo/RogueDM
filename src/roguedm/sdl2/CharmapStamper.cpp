// Copyright 2022 Fco. Jose Fdez. Naranjo AKA github.com/fjfnaranjo

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

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "mbtools.hpp"
#include "translations.hpp"
#include "../commands/Word.hpp"
#include "../paths.hpp"
#include "../strings.hpp"

namespace roguedm_gui {

CharmapStamper::CharmapStamper() {
  initSuccess = false;

  config = roguedm::Config::instance();
  txtCHeight = 0;
  txtCWidth = 0;
  txtCpr = 0;
  txtHSep = 0;
  txtWSep = 0;
  txtHStart = 0;
  txtWStart = 0;

  wordKindTexture = {};
}

CharmapStamper::~CharmapStamper() {
  resetCharmapStamper();
}

void CharmapStamper::resetCharmapStamper() {
  if (initSuccess) {
    for (int kind = 0; kind < RDM_WCLASS_TOTAL; kind++)
      SDL_DestroyTexture(wordKindTexture[kind]);
    initSuccess = false;
  }
}

bool CharmapStamper::loadDefaultCharmap(SDL_Renderer *renderer,
                                        std::string defaultCharmap) {
  // Default charmap texture file path
  std::string defaultCharmapPath = std::string(RDM_PATH_HERE)
      + std::string(RDM_PATH_SHARE) + std::string(RDM_PATH_SEP)
      + config->getSettingValue(defaultCharmap, "path",
                                "imgs/codepage-850-9-14.png");

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
  if (!baseTexture)
    return false;

  defineAndColorizeWordTypes(renderer, baseTexture);

  // Free reference surfaces
  SDL_FreeSurface(baseTexture);

  initSuccess = true;
  return true;
}

unsigned int CharmapStamper::decoratorsLength(int idx) const {
  return multibyteLenght(roguedm::wordKinds[idx].lDecorator)
      + multibyteLenght(roguedm::wordKinds[idx].rDecorator);
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

void CharmapStamper::stampInnerChar(std::string c, int t, int x, int y,
                                    SDL_Renderer* renderer, int mx, int my) {
  stampChar(transChar(c), t, x, y, renderer, mx, my);
}

void CharmapStamper::stampLDecoratorChar(int idx, int t, int x, int y,
                                         SDL_Renderer* renderer, int mx,
                                         int my) {
  stampChar(
      transChar(
          multibyteCharacterByIndex(roguedm::wordKinds[t].lDecorator, idx)),
      t, x, y, renderer, mx, my);
}

void CharmapStamper::stampRDecoratorChar(int idx, int t, int x, int y,
                                         SDL_Renderer* renderer, int mx,
                                         int my) {
  stampChar(
      transChar(
          multibyteCharacterByIndex(roguedm::wordKinds[t].rDecorator, idx)),
      t, x, y, renderer, mx, my);
}

void CharmapStamper::stampIp(SDL_Renderer *renderer, int x, int y) {
  mStampChar(95, 0, x / txtCWidth, y / txtCHeight, renderer);
}

void CharmapStamper::drawBox(int x, int y, int w, int h,
                             SDL_Renderer *renderer) {
  mStampChar(201, 0, x, y, renderer);
  mStampChar(187, 0, x + w, y, renderer);
  mStampChar(200, 0, x, y + h, renderer);
  mStampChar(188, 0, x + w, y + h, renderer);
  for (int c = x + 1; c < x + w; c++)
    mStampChar(205, 0, c, y, renderer);
  for (int c = x + 1; c < x + w; c++)
    mStampChar(205, 0, c, y + h, renderer);
  for (int c = y + 1; c < y + h; c++)
    mStampChar(186, 0, x, c, renderer);
  for (int c = y + 1; c < y + h; c++)
    mStampChar(186, 0, x + w, c, renderer);
}

void CharmapStamper::drawHSeparator(int x, int y, int s,
                                    SDL_Renderer *renderer) {
  mStampChar(204, 0, x, y, renderer);
  mStampChar(185, 0, x + s, y, renderer);
  for (int c = x + 1; c < x + s; c++)
    mStampChar(205, 0, c, y, renderer);
}

void CharmapStamper::drawVSeparator(int x, int y, int s,
                                    SDL_Renderer *renderer) {
  mStampChar(203, 0, x, y, renderer);
  mStampChar(202, 0, x, y + s, renderer);
  for (int c = y + 1; c < y + s; c++)
    mStampChar(186, 0, x, c, renderer);
}

void CharmapStamper::drawCross(int x, int y, SDL_Renderer *renderer) {
  mStampChar(206, 0, x, y, renderer);
}

void CharmapStamper::defineAndColorizeWordTypes(SDL_Renderer *renderer,
                                                SDL_Surface *baseSurface) {
  for (int kind = 0; kind < RDM_WCLASS_TOTAL; kind++)
    wordKindTexture.insert(
        wordKindTexture.end(),
        colorizeWordType(renderer, baseSurface,
                         roguedm::wordKinds[kind].fgColor.r,
                         roguedm::wordKinds[kind].fgColor.g,
                         roguedm::wordKinds[kind].fgColor.b));  //,
  // roguedm::wordKinds[kind].bgColor.r,
  // roguedm::wordKinds[kind].bgColor.g,
  // roguedm::wordKinds[kind].bgColor.b
  // ));
}

SDL_Texture* CharmapStamper::colorizeWordType(SDL_Renderer *renderer,
                                              SDL_Surface *srf, int fgr,
                                              int fgg, int fgb) {  //,
  // int bgr,
  // int bgg,
  // int bgb
// ) {
  SDL_Palette *palette = srf->format->palette;
  for (int i = 0; i < srf->format->palette->ncolors; i++) {
    palette->colors[i].r = fgr;
    palette->colors[i].g = fgg;
    palette->colors[i].b = fgb;
  }
  return SDL_CreateTextureFromSurface(renderer, srf);
}

int CharmapStamper::transChar(std::string c) {
  for (auto const & entry : transUtf8)
    if (0 == c.compare(entry.first))
      return entry.second;
  // TODO(fjfnaranjo): Remove magic constants like this one
  return 254;
}

void CharmapStamper::stampChar(int n, int t, int x, int y,
                               SDL_Renderer *renderer, int maxCols,
                               int maxRows) {
  while (n > 255)
    n -= 256;
  while (x > maxCols)
    x -= maxCols + 1;
  while (y > maxRows)
    y -= maxRows + 1;
  int columnNumber = n % txtCpr;
  int rowNumber = n / txtCpr;
  SDL_Rect srcrect;
  srcrect.x = txtWStart + (txtWSep * columnNumber) + (txtCWidth * columnNumber);
  srcrect.y = txtHStart + (txtHSep * rowNumber) + (txtCHeight * rowNumber);
  srcrect.w = txtCWidth;
  srcrect.h = txtCHeight;
  SDL_Rect dstrect;
  dstrect.x = x * txtCWidth;
  dstrect.y = y * txtCHeight;
  dstrect.w = txtCWidth;
  dstrect.h = txtCHeight;
  SDL_RenderCopy(renderer, wordKindTexture[t], &srcrect, &dstrect);
}

void CharmapStamper::mStampChar(int n, int t, int x, int y,
                                SDL_Renderer *renderer) {
  while (n > 255)
    n -= 256;
  int columnNumber = n % txtCpr;
  int rowNumber = n / txtCpr;
  SDL_Rect srcrect;
  srcrect.x = txtWStart + (txtWSep * columnNumber) + (txtCWidth * columnNumber);
  srcrect.y = txtHStart + (txtHSep * rowNumber) + (txtCHeight * rowNumber);
  srcrect.w = txtCWidth;
  srcrect.h = txtCHeight;
  SDL_Rect dstrect;
  dstrect.x = x * txtCWidth;
  dstrect.y = y * txtCHeight;
  dstrect.w = txtCWidth;
  dstrect.h = txtCHeight;
  SDL_RenderCopy(renderer, wordKindTexture[t], &srcrect, &dstrect);
}

}  // namespace roguedm_gui
