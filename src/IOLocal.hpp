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
 * \file IOLocal.hpp
 * \brief File containing the IOLocal class declarations.
 */

#pragma once

#include <iostream>
#include <map>

#include <SDL2/SDL.h>

#include "CommandHandlerInterface.hpp"
#include "GameComponentInterface.hpp"

// Max command history lines..
#define RDM_CL_MAX_HISTORY         128

// Command's words.
#define RDM_CMD_QUIT         L"quit"       // End app.
#define RDM_CMD_PSAY         L"psay"       // Player say to all.

namespace roguedm {

/** \brief Struct wordclass to contain word color and decorators. */
struct WordClass {
  SDL_Color color;
  SDL_Color clearColor;
  std::wstring lDecorator;
  std::wstring rDecorator;
  SDL_Texture *charsTexture;
};

/** \brief Struct Player for the name and so. */
struct Player {
  std::wstring name;
  WordClass word;
};

/**
 * \brief The interface management class.
 *
 * Manage the SDL interface with the player. The console graphical output and
 * the texts input. Also handles the vast mayority of the console command line
 * features.
 *
 * Pattern Singleton.
 */
class IOLocal : CommandHandlerInterface, GameComponentInterface
{

  RDM_DECLARE_CLASS_AS_SINGLETON(IOLocal)

  public:

    /**
     * Method used when the main app has time to allow a network management
     * step from the game loop.
     * \see GameComponentInterface.update()
     */
    void update();

    /**
     * Used by IOLocal to ask the command handler a response for a command.
     * \see CommandHandlerInterface.processCommand()
     */
    int processCommand(const Sentence&);

    /**
     * Used by IOLocal to ask the command handler a autocomplete suggestion.
     * \see CommandHandlerInterface.autocomplete()
     */
    const int autocomplete(Sentence&);

    /**
     * Used by IOLocal to ask the command handler a autocomplete list.
     * \see CommandHandlerInterface.autocompleteListOptions()
     */
    const std::vector<Sentence>* autocompleteListOptions(const Sentence&);

    /**
     * Get the error code value (detecting contruction failures).
     * \return Status code to be returned with cstdlib exit() or 0 if ok.
     */
    int getErrorCode();

    /**
     * Add a new command handler to the internal list, after erasing any
     * previous ocurrence of that handler.
     * \param c The command handler.
     */
    void registerCommandHandler(CommandHandlerInterface *c);

    /**
     * Remove all references to the command handler from the internal list.
     * \param c The command handler.
     */
    void unregisterCommandHandler(CommandHandlerInterface *c);

    /**
     * Set a new default word.
     * \param c The new word.
     */
    void setDefaultWord(Word c);

    /**
     * Tell the outside that the user wants to quit de app.
     * \return 1 if user want to quit, 0 if not
     */
    int mustHalt();

  private:

    /**
     * Method internally called to manage the SDL event queque.
     */
    void eventsManager();

    /** Updates the screen if window resizes. */
    void initScreenSize();

    /** Define the initial word type table. */
    void initWordTypes();

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
     * Define the table used to translate a UTF-8 input character to a ASCII 850
     * page character set.
     */
    void initTransTable();

    /** Reser the command line and its history navigation cursor */
    void resetLine();

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
      int h
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
      int s
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
      int s
    );

    /**
     * Translate a wchar to the ASCII 850 corresponding code.
     * \param c The wide character.
     * \return The ASCII position.
     */
    int transChar(wchar_t c);

    /**
     * Write a 850 ASCII char in the interface.
     * \param n ASCII position.
     * \param t Word type.
     * \param x Char x coordinate.
     * \param y Char y coordinate.
     */
    void stampChar(
      int n,
      int t,
      int x,
      int y
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
      int y
    );

    /** Manage a key senden from the SDL events manager */
    void processKey(SDL_Event*);

    /** Process the command line. */
    void processLine();

    /** Try to find a valid autocompletion for the current command. */
    void tryAutocompletion();

    /** The default word to add when the user autocomplete with an empy line. */
    Word defaultWord;

    /** The command hanlders list. */
    std::vector<CommandHandlerInterface*> commandHandlers;

    /** Error code */
    int errorCode;

    /** True when user wants to quit. */
    int appDone;

    /** Table holding all the word types */
    WordClass wordTypes[RDM_WCLASS_TOTAL];

    /** Table used to translate a UTF-8 input character to a ASCII 850 */
    std::map<wchar_t,int> transUtf8;

    /** SDL window */
    SDL_Window *window = nullptr;

    /** SDL main rendering surface */
    SDL_Renderer *renderer = nullptr;

    /** The current command line contents. */
    Sentence commandLine;

    /** Current history position when using up-down keys. */
    int historyCurrent;

    /** The current command line contents. */
    std::vector<Sentence> consoleHistory;

    /** Sentences vector for the history. */
    std::vector<Sentence> history;

    /**
     * Sentence currently written when the player starts using the history
     * exploration control
     */
    Sentence historyBackup;

    /** Line exploration current displacement in words. */
    int currentWord;

    /** Line exploration current displacement in characters. */
    int wordRShift;

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

    /** Insertion point graphic surface. */
    SDL_Texture *ipI;

    /** Screen size in rows. */
    int maxRows;

    /** Screen size in colums. */
    int maxCols;

    /** Coordinates for the main window inside the terminal. */
    SDL_Rect dialogMain;

    /** Coordinates for the cell info window inside the terminal. */
    SDL_Rect dialogCell;

    /** Coordinates for the creature info window inside the terminal. */
    SDL_Rect dialogCreature;

    /** Coordinates for the players info window inside the terminal. */
    SDL_Rect dialogPlayers;

    /** Coordinates for the text window inside the terminal. */
    SDL_Rect dialogText;

};

} // namespace roguedm
