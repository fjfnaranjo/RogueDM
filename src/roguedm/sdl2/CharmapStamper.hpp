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

#include "../Config.hpp"
#include "../commands.hpp"

namespace roguedm_gui {

/**
 * Struct to define word-type colors and enclosing decorations.
 *
 * * fgColor: Color for the foreground.
 * * bgColor: Color for the background.
 * * lDecorator: Left-side of the enclosing decoration.
 * * rDecorator: Right-side of the enclosing decoration.
 * * texture: The actual texture with the colorization applied.
 */
struct WordClass {
  SDL_Color fgColor;
  SDL_Color bgColor;
  std::string lDecorator;
  std::string rDecorator;
  SDL_Texture *texture;
};

/**
 * This class is responsible of storing a particular character map (charmap)
 * image and use it to write characters to the screen.
 */
class CharmapStamper {

  public:

    CharmapStamper();
    ~CharmapStamper();

    /** Loads a charmap image using the config section values for it. */
    bool loadDefaultCharmap(SDL_Renderer*, std::string);

    /**
     * Returns the character size for all enclosing decoration characters.
     */
    int decoratorsLength(int) const;

    /**
     * Returns the character size for the left enclosing decoration
     * characters.
     */
    int lDecoratorsLength(int) const;

    /**
     * Returns the character size for the right enclosing decoration
     * characters.
     */
    int rDecoratorsLength(int) const;

    /** Returns the character height for the charmap. */
    int getCHeight() const;

    /** Returns the character width for the charmap. */
    int getCWidth() const;

    /**
     * Proxy for the \ref stampChar method used for writing a single character.
     */
    void stampInnerChar(
      std::string,
      int,
      int,
      int,
      SDL_Renderer*,
      int,
      int
    );

    /**
     * Proxy for the \ref stampChar method used for writing a particular
     * character from the left enclosing decoration characters.
     */
    void stampLDecoratorChar(
      int,
      int,
      int,
      int,
      SDL_Renderer*,
      int,
      int
    );

    /**
     * Proxy for the \ref stampChar method used for writing a particular
     * character from the right enclosing decoration characters.
     */
    void stampRDecoratorChar(
      int,
      int,
      int,
      int,
      SDL_Renderer*,
      int,
      int
    );

    /** Stamps the insertion point character in a particular coordinate. */
    void stampIp(
      SDL_Renderer*,
      int,
      int
    );

    /**
     * Draws a box using the ASCII 850 double pipe characters.
     * \param x Box starting x coordinate.
     * \param y Box starting y coordinate.
     * \param w Box width.
     * \param h Box height.
     */
    void drawBox(
      int x,
      int y,
      int w,
      int h,
      SDL_Renderer*
    );

    /**
     * Draws a horizontal separator line using the ASCII 850 double pipe
     * characters.
     * \param x Separator starting x coordinate.
     * \param y Separator starting y coordinate.
     * \param s Separator length.
     */
    void drawHSeparator(
      int x,
      int y,
      int s,
      SDL_Renderer*
    );

    /**
     * Draws a vertical separator line using the ASCII 850 double pipe
     * characters.
     * \param x Separator starting x coordinate.
     * \param y Separator starting y coordinate.
     * \param s Separator length.
     */
    void drawVSeparator(
      int x,
      int y,
      int s,
      SDL_Renderer*
    );

    /**
     * Draws a ASCII 850 double cross character.
     * \param x Cross x coordinate.
     * \param y Cross y coordinate.
     */
    void drawCross(
      int x,
      int y,
      SDL_Renderer*
    );

  private:

    /** Stores if SDL resources have been initialized successfully. */
    bool initSuccess;

    /** App configuration singleton shared pointer. */
    roguedm::ConfigSharedPtr config;

    /** Define the static word types and colorize them. */
    void defineAndColorizeWordTypes(SDL_Renderer*, SDL_Surface*);

    /**
     * Define the table used to translate a UTF-8 input character to a ASCII 850
     * page character set.
     */
    void initTransTable();

    /** Used to color the word types textures. */
    SDL_Texture* colorizeWordType(
      SDL_Renderer* renderer,
      SDL_Surface* srf,
      int rf,
      int gf,
      int bf,
      int br,
      int bg,
      int bb
    );

    /**
     * Translate a UTF-8 to its ASCII 850 position.
     * \param c The multibyte character.
     * \return The ASCII 850 position.
     */
    int transChar(std::string);

    /**
     * Draws a ASCII 850 char.
     * \param n ASCII 850 position.
     * \param t Word type.
     * \param x Char x coordinate.
     * \param y Char y coordinate.
     * \param renderer The current SDL render context.
     * \param maxCols Upper limit for columns.
     * \param maxRows Upper limit for rows.
     */
    void stampChar(
      int n,
      int t,
      int x,
      int y,
      SDL_Renderer* renderer,
      int maxCols,
      int maxRows
    );

    /**
     * Draws a ASCII 850 char (simple).
     * \param n ASCII 850 position.
     * \param t Word type.
     * \param x Char x coordinate.
     * \param y Char y coordinate.
     * \param renderer The current SDL render context.
     */
    void mStampChar(
      int n,
      int t,
      int x,
      int y,
      SDL_Renderer* renderer
    );

    /** Table used to translate a UTF-8 input character to a ASCII 850 */
    std::map<std::string,int> transUtf8;

    /** Characters texture chars-by-row. */
    int txtCpr;

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

};

} // namespace roguedm_gui
