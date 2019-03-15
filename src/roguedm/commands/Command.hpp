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

#include <string>
#include <vector>

#include "Sentence.hpp"

// Command's words.
#define RDM_CMD_QUIT         u8"quit"       // End app.
#define RDM_CMD_PSAY         u8"psay"       // Player say to all.

namespace roguedm {

/**
 * \brief Struct to contain a command name and its parameters.
 *
 * A Command consist in a name identifying the particular command (its function)
 * and a list of params.
 */
struct Command {
  std::string name;
  Sentence params;
};

/** \brief Command's vector to contain a list of commands. */
typedef std::vector<Command> CommandList;

/** \brief Shared pointer to a vector of commands. */
typedef std::shared_ptr<CommandList> CommandListSharedPtr;

}  // namespace roguedm
