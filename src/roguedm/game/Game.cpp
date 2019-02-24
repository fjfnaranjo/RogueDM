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

#include "../game/Game.hpp"

#include <memory>

namespace roguedm_game {

int Game::processCommand(const roguedm::Sentence&) {
  return RDM_COMMAND_UNKNOWN;
}
int Game::autocomplete(roguedm::Sentence&) const {
  return RDM_COMMAND_AC_NEXT;
}

roguedm::SentenceListSharedPtr Game::autocompleteListOptions(
  const roguedm::Sentence&
) const {
  return std::make_shared<roguedm::SentenceList>();
}

void Game::update() {}

} // namespace roguedm
