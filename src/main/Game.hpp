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
 * \file Game.hpp
 * \brief File containing the Game class declarations.
 */

#pragma once

#include "../CommandHandlerInterface.hpp"
#include "../GameComponentInterface.hpp"

namespace roguedm_main {

/**
 * \brief Class for all game rules related tasks.
 *
 * This class manages the game rules and anything related to it.
 * \see roguedm::StateInterface
 * \see roguedm::StateInterface.execute()
 */
class Game :
  public roguedm::CommandHandlerInterface,
         roguedm::GameComponentInterface
{
  public:

    /**
     * Default constructor (project guidelines requires always a constructor,
     * even if it will be empty).
     */
    Game();

    /**
     * Default destructor (project guidelines requires always a destructor,
     * even if it will be empty).
     */
    virtual ~Game();

    /**
     * Used by IOLocal to ask the command handler a response for a command.
     * \see roguedm::CommandHandlerInterface.processCommand()
     */
    int processCommand(const roguedm::Sentence&);

    /**
     * Used by IOLocal to ask the command handler a autocomplete suggestion.
     * \see roguedm::CommandHandlerInterface.autocomplete()
     */
    const int autocomplete(roguedm::Sentence&);

    /**
     * Used by IOLocal to ask the command handler a autocomplete list.
     * \see roguedm::CommandHandlerInterface.autocompleteListOptions()
     */
    const roguedm::SentenceList* autocompleteListOptions(
      const roguedm::Sentence&
    );

    /**
     * Method used when the main app has time to allow a network management
     * step from the game loop.
     */
    void update();

  private:

    /** Copy operator (private because is disabled by default). */
    Game(const Game&);

    /** Assing operator (private because is disabled by default). */
    void operator=(const Game&);

};

} // namespace roguedm
