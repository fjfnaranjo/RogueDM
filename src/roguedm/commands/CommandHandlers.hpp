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

#include "CommandHandlerInterface.hpp"
#include "../macros.hpp"

namespace roguedm {

// TODO(fjfnaranjo): This is missing docs and a mech to unregister handlers

class CommandHandlers {

RDM_DECLARE_CLASS_AS_SINGLETON(CommandHandlers)

 public:

  CommandHandlers();

  void registerCommandHandler(CommandHandlerSharedPtr c);

  CommandHandlerList getCommandHandlers();

 private:

  CommandHandlerList commandHandlers;

};

typedef std::shared_ptr<CommandHandlers> CommandHandlersSharedPtr;

}  // namespace roguedm
