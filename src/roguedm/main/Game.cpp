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

#include "Game.hpp"

#include <memory>

namespace roguedm_main {

Game::Game() {}

Game::~Game() {}

int Game::processCommand(const roguedm::Sentence&) { return 0; }
const int Game::autocomplete(roguedm::Sentence&) { return 0; };

const roguedm::SentenceListReference Game::autocompleteListOptions(
  const roguedm::Sentence&
) {
  return std::make_shared<roguedm::SentenceList>();
};

void Game::update() {}

} // namespace roguedm
