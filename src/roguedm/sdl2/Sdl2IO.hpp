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

#include <iostream>
#include <map>
#include <memory>

#include <SDL2/SDL.h>

#include "CharmapStamper.hpp"
#include "../CommandHandlerInterface.hpp"
#include "../Config.hpp"
#include "../GameComponentInterface.hpp"

// Max command history lines..
#define RDM_CL_MAX_HISTORY         128

// Command's words.
#define RDM_CMD_QUIT         u8"quit"       // End app.
#define RDM_CMD_PSAY         u8"psay"       // Player say to all.

namespace roguedm_gui {

/** \brief Struct Player for the name and so. */
struct Player {
  std::string name;
  WordClass word;
};

/**
 * \brief The interface management class.
 *
 * Manage the SDL interface with the player. The console graphical output and
 * the texts input. Also handles the vast majority of the console command line
 * features.
 *
 * Pattern Singleton.
 */
class Sdl2IO :
    roguedm::CommandHandlerInterface,
    roguedm::GameComponentInterface
{

  RDM_DECLARE_CLASS_AS_NOCPNOMV(Sdl2IO);

  public:

    Sdl2IO();
    ~Sdl2IO();

    /**
     * Method used when the main app has time to allow a network management
     * step from the game loop.
     */
    void update() override;

    /**
     * Used to ask the command handler a response for a command.
     */
    int processCommand(const roguedm::Sentence&) override;

    /**
     * Used to ask the command handler an autocomplete suggestion.
     */
    int autocomplete(roguedm::Sentence&) const override;

    /**
     * Used to ask the command handler an autocomplete candidate list.
     */
    roguedm::SentenceListReference autocompleteListOptions(
      const roguedm::Sentence&
    ) const override;

    /**
     * Get the error code value (detecting contruction failures).
     * \return Status code to be returned with cstdlib exit() or 0 if ok.
     */
    int getErrorCode() const;

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
    void setDefaultWord(roguedm::Word c);

    /**
     * Tell the outside that the user wants to quit de app.
     * \return 1 if user want to quit, 0 if not
     */
    int mustHalt();

  private:

    /** App configuration singleton reference. */
    roguedm::ConfigSharedPtr config;

    /**
     * Keep the SDL_GetTicks return value to implement a FPS lock.
     */
    int ticks;

    /**
     * Method internally called to manage the SDL event queque.
     */
    void eventsManager();

    /** Updates the screen if window resizes. */
    void initScreenSize();

    /** Define the initial word type table. */
    void initCharmaps();

    /** Reser the command line and its history navigation cursor */
    void resetLine();

    /** Manage a text composition from the SDL events manager */
    void processText(SDL_Event*);

    /** Manage a key senden from the SDL events manager */
    void processKey(SDL_Event*);

    /** Process the command line. */
    void processLine();

    /** Try to find a valid autocompletion for the current command. */
    void tryAutocompletion();

    /** The default word to add when the user autocomplete with an empy line. */
    roguedm::Word defaultWord;

    /** The command hanlders list. */
    std::vector<CommandHandlerInterface*> commandHandlers;

    /** Error code */
    int errorCode;

    /** True when user wants to quit. */
    int appDone;

    /** SDL window */
    SDL_Window *window = nullptr;

    /** SDL main rendering surface */
    SDL_Renderer *renderer = nullptr;

    /** The current command line contents. */
    roguedm::Sentence commandLine;

    /** Current history position when using up-down keys. */
    int historyCurrent;

    /** The current command line contents. */
    roguedm::SentenceList consoleHistory;

    /** Sentences vector for the history. */
    roguedm::SentenceList history;

    /**
     * Sentence currently written when the player starts using the history
     * exploration control
     */
    roguedm::Sentence historyBackup;

    /** Line exploration current displacement in words. */
    int currentWord;

    /** Line exploration current displacement in characters. */
    int wordRShift;

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

    std::unique_ptr<CharmapStamper> defaultStamper;

    int defaultCHeight;

    int defaultCWidth;

};

} // namespace roguedm_gui
