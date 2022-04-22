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

#pragma once

#include "../GameComponentInterface.hpp"
#include "../commands/Command.hpp"
#include "../commands/CommandHandlerInterface.hpp"
#include "../commands/Sentence.hpp"

namespace roguedm_game {

/**
 * \brief Class for all game rules related tasks.
 *
 * This class manages the game rules and anything related to it.
 */
class Game : public roguedm::CommandHandlerInterface,
    public roguedm::GameComponentInterface {

RDM_DECLARE_CLASS_AS_NOCPNOMV(Game)

 public:

  Game() = default;

  /** Request to process a command. */
  bool processCommand(const roguedm::Command&) override;

  /** Request to identify a command in a sentence. */
  bool identifyCommand(const roguedm::Sentence&, roguedm::Command&) const
      override;

  /** Request a list of autocomplete options for a command. */
  roguedm::CommandList getCompletionCandidates(const roguedm::Command&) const
      override;

  /**
   * Method used when the main app has time to allow a network management
   * step from the game loop.
   */
  void update() override;

};

}  // namespace roguedm
