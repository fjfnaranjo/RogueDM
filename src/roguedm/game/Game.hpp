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

#include "../CommandHandlerInterface.hpp"
#include "../GameComponentInterface.hpp"

namespace roguedm_game {

/**
 * \brief Class for all game rules related tasks.
 *
 * This class manages the game rules and anything related to it.
 */
class Game :
  public roguedm::CommandHandlerInterface,
         roguedm::GameComponentInterface
{
  public:

    Game();
    ~Game();
    Game(const Game & ) =delete;
    Game & operator = (const Game & ) =delete;

    /**
     * Used to ask the command handler a response for a command.
     */
    int processCommand(const roguedm::Sentence&) override;

    /**
     * Used to ask the command handler an autocomplete suggestion.
     */
    const int autocomplete(roguedm::Sentence&) override;

    /**
     * Used to ask the command handler an autocomplete candidate list.
     */
    const roguedm::SentenceListReference autocompleteListOptions(
      const roguedm::Sentence&
    ) override;

    /**
     * Method used when the main app has time to allow a network management
     * step from the game loop.
     */
    void update() override;

};

} // namespace roguedm
