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

#include "CharmapStamper.hpp"
#include "../macros.hpp"
#include "../commands/CommandHandlerInterface.hpp"

// Max command history lines..
#define RDM_CL_MAX_HISTORY         128

namespace roguedm_gui {

/** \brief Helps with the editing and management of commands in the GUI. */
class CommandComposer {

RDM_DECLARE_CLASS_AS_NOCPNOMV(CommandComposer)

 public:

  CommandComposer();
  ~CommandComposer();

  /** \ref roguedm::Command getter. */
  roguedm::Command getCommand();

  /** \ref roguedm::Command full sentence getter. */
  roguedm::Sentence getRawSentence();

  /** \ref roguedm::Command full sentence setter. */
  void setCommand(const roguedm::Command&);

  /** Get the current command full length. */
  int commandLength();

  /** Check if there is already a command in the command line. */
  bool hasCommand();

  /** Process pressing of key backspace. */
  void keyBackspace();

  /** Process pressing of key delete. */
  void keyDelete();

  /**
   * Process pressing of key left.
   * \param fullWord Move in increments of full words.
   */
  void keyLeft(bool fullWord);

  /**
   * Process pressing of key right.
   * \param fullWord Move in increments of full words.
   */
  void keyRight(bool fullWord);

  /** Process pressing of key home. */
  void keyHome();

  /** Process pressing of key end. */
  void keyEnd();

  /** Process pressing of key up. */
  void keyUp();

  /** Process pressing of key down. */
  void keyDown();

  /** Process pressing of key space. */
  void keySpace();

  /** Process pressing of any normal character key (not space). */
  void keyCharacter(std::string character);

  /** Add a new command to the command history. */
  void commandHistoryPush(roguedm::Command);

  /** Reset the command line and its history navigation cursor */
  void resetCommand();

  /** Paints the command line. */
  void paintCommandLine(SDL_Renderer*, std::shared_ptr<CharmapStamper>, int,
                        int, int, int);

 private:

  /** Serialize a command as a sentence. */
  roguedm::Sentence command2Sentence(roguedm::Command);

  /** Serialize a sentence as a command. */
  roguedm::Command sentence2Command(roguedm::Sentence);

  /** An empty command to signify the absence of command. */
  roguedm::Word emptyWord;

  /** Line exploration current displacement in words. */
  int currentWord;

  /** Line exploration current displacement in characters. */
  int wordRShift;

  /**
   * The current command sentence contents.
   *
   * For ease of manipulation this contains both the \ref roguedm::Command
   * name and the parameters in a single \ref roguedm::Sentence object.
   */
  roguedm::Sentence sentence;

  /** Sentences vector for the history. */
  roguedm::SentenceList history;

  /** Current history position when using up-down keys. */
  int historyCurrent;

  /**
   * Sentence currently written when the player starts using the history
   * exploration control
   */
  roguedm::Sentence historyBackup;

};

}  // namespace roguedm_gui
