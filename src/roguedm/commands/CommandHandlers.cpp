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

#include "CommandHandlers.hpp"

namespace roguedm {

CommandHandlers::CommandHandlers() {
  commandHandlers = {};
}

void CommandHandlers::registerCommandHandler(CommandHandlerSharedPtr c) {
  commandHandlers.push_back(c);
}

CommandHandlerList CommandHandlers::getCommandHandlers() {
  return commandHandlers;
}

}  // namespace roguedm
