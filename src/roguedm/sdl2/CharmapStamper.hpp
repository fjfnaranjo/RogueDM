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

#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>

#include "mbtools.hpp"
#include "../Config.hpp"
#include "../commands.hpp"

namespace roguedm_gui {

/** \brief Struct wordclass to contain word color and decorators. */
struct WordClass {
  SDL_Color color;
  SDL_Color clearColor;
  std::string lDecorator;
  std::string rDecorator;
  SDL_Texture *charsTexture;
};

class CharmapStamper {

  public:

    CharmapStamper();
    ~CharmapStamper();
    void loadDefaultCharmap(SDL_Renderer*, std::string);
    int decoratorsLength(int) const;
    int lDecoratorsLength(int) const;
    int rDecoratorsLength(int) const;
    int getCHeight() const;
    int getCWidth() const;

    void stampInnerChar(
      std::string,
      int,
      int,
      int,
      SDL_Renderer*,
      int,
      int
    );

    void stampLDecoratorChar(
      int,
      int,
      int,
      int,
      SDL_Renderer*,
      int,
      int
    );

    void stampRDecoratorChar(
      int,
      int,
      int,
      int,
      SDL_Renderer*,
      int,
      int
    );

    void stampIp(
      SDL_Renderer*,
      int,
      int
    );

    /**
     * Draw a box using the ASCII 850 pipe characters.
     * \param t Box type.
     * \param x Box starting x coordinate.
     * \param y Box starting y coordinate.
     * \param w Box widht.
     * \param h Box height.
     */
    void drawBox(
      int t,
      int x,
      int y,
      int w,
      int h,
      SDL_Renderer*
    );

    /**
     * Draw a horizontal separator line using the ASCII 850 pipe characters.
     * \param t Separator type.
     * \param x Separator starting x coordinate.
     * \param y Separator starting y coordinate.
     * \param s Separator length.
     */
    void drawHSeparator(
      int t,
      int x,
      int y,
      int s,
      SDL_Renderer*
    );

    /**
     * Draw a vertical separator line using the ASCII 850 pipe characters.
     * \param t Separator type.
     * \param x Separator starting x coordinate.
     * \param y Separator starting y coordinate.
     * \param s Separator length.
     */
    void drawVSeparator(
      int t,
      int x,
      int y,
      int s,
      SDL_Renderer*
    );

    /**
     * Write a 850 ASCII cross character.
     * \param t Word type.
     * \param x Cross x coordinate.
     * \param y Cross y coordinate.
     */
    void drawCross(
      int t,
      int x,
      int y,
      SDL_Renderer*
    );

  private:

    /** App configuration singleton reference. */
    roguedm::ConfigSharedPtr config;

    void colorizeTexture(SDL_Renderer*, SDL_Surface*);

    /**
     * Define the table used to translate a UTF-8 input character to a ASCII 850
     * page character set.
     */
    void initTransTable();

    /**
     * Used by method initWordTypes to set the words color textures.
     * \param wordTypeSurface Reference to the surface to be colored.
     * \param rf Foreground red color component.
     * \param gf Foreground green color component.
     * \param bf Foreground blue color component.
     * \param br Background red color component.
     * \param bg Background green color component.
     * \param bb Background blue color component.
     * \see initWordTypes()
     */
    void colorizeWordType(
      SDL_Texture* wordTypeSurface,
      int rf,
      int gf,
      int bf,
      int br,
      int bg,
      int bb
    );

    /**
     * Translate a multibyte character to its ASCII 850 position.
     * \param c The multibyte character.
     * \return The ASCII 850 position.
     */
    int transChar(std::string);

    /**
     * Write a 850 ASCII char in the interface.
     * \param n ASCII position.
     * \param t Word type.
     * \param x Char x coordinate.
     * \param y Char y coordinate.
     */
    void stampChar(
      int,
      int,
      int,
      int,
      SDL_Renderer*,
      int,
      int
    );

    void mStampChar(
      int,
      int,
      int,
      int,
      SDL_Renderer*
    );

    /** Table used to translate a UTF-8 input character to a ASCII 850 */
    std::map<std::string,int> transUtf8;

    /** Characters texture chars-by-row. */
    int txtCpr;

    /** Full character texture height. */
    int txtHeight;

    /** Full character texture width. */
    int txtWidth;

    /** Single character texture height. */
    int txtCHeight;

    /** Single character texture width. */
    int txtCWidth;

    /** Texture vertical character separation. */
    int txtHSep;

    /** Texture horizontal character separation. */
    int txtWSep;

    /** Texture vertical first character position. */
    int txtHStart;

    /** Texture horizontal first character position. */
    int txtWStart;

    /** Table holding all the word types */
    WordClass wordTypes[RDM_WCLASS_TOTAL];

    /** Insertion point graphic surface. */
    SDL_Texture *ipI;

};

} // namespace roguedm_gui
