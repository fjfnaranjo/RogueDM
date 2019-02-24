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

#include <SDL.h>

#include "CharmapStamper.hpp"
#include "../macros.hpp"
#include "../CommandHandlerInterface.hpp"

// Max command history lines..
#define RDM_CL_MAX_HISTORY         128

// Command's words.
#define RDM_CMD_QUIT         u8"quit"       // End app.
#define RDM_CMD_PSAY         u8"psay"       // Player say to all.

namespace roguedm_gui {

class Gui {

  RDM_DECLARE_CLASS_AS_NOCPNOMV(Gui)

  public:

    Gui();
    ~Gui();
    void update(SDL_Renderer*);
    bool initGui(SDL_Renderer*);
    roguedm::Sentence getCommandLine();
    void setCommandLine(const roguedm::Sentence&);
    int commandLength();
    bool hasCommand();
    void addReplaceCommand();
    void commandHistoryPush(roguedm::Sentence);
    void consoleHistoryPush(roguedm::Sentence);
    void keyBackspace();
    void keyDelete();
    void keyLeft(bool);
    void keyRight(bool);
    void keyHome();
    void keyEnd();
    void keyUp();
    void keyDown();
    void keySpace();
    void keyCharacter(std::string character);
    void resetHistoryCurrent();

    /** Reser the command line and its history navigation cursor */
    void resetLine();

    /** Updates the screen if window resizes. */
    void resetScreenSize(SDL_Window*);

  private:

    /** Stores if SDL resources have been initialized successfully. */
    bool initSuccess;

    /** Define the initial word type table. */
    bool initCharmaps(SDL_Renderer*);

    /** App configuration singleton reference. */
    roguedm::ConfigSharedPtr config;

    /** Character width of the default charmap. */
    int defaultCWidth;

    /** Character height of the default charmap. */
    int defaultCHeight;

    /** Screen size in columns. */
    int maxCols;

    /** Screen size in rows. */
    int maxRows;

    /** An empty command to signify the absence of command. */
    roguedm::Word emptyWord;

    /** Line exploration current displacement in words. */
    int currentWord;

    /** Line exploration current displacement in characters. */
    int wordRShift;

    /** The current command line contents. */
    roguedm::Sentence commandLine;

    /** The current command line contents. */
    roguedm::SentenceList consoleHistory;

    /** Sentences vector for the history. */
    roguedm::SentenceList history;

    /** Current history position when using up-down keys. */
    int historyCurrent;

    /**
     * Sentence currently written when the player starts using the history
     * exploration control
     */
    roguedm::Sentence historyBackup;

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

    /** Unique pointer to the default charmap drawing class. */
    std::unique_ptr<CharmapStamper> defaultStamper;

};

} // namespace roguedm_gui
