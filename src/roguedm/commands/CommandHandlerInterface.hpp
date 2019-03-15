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

#include <memory>
#include <vector>

#include "Command.hpp"

#define RDM_CMD_PROCESS_UNKNOWN  false
#define RDM_CMD_PROCESS_DONE     true

#define RDM_CMD_IDENTIFY_UNKNOWN false
#define RDM_CMD_IDENTIFY_DONE    true

namespace roguedm {

/**
 * \interface CommandHandlerInterface
 * \brief C++ interface for any command/messages handler.
 *
 * Declares the class interface used by command handlers to expose a simple API
 * to react to commands and also to allow interactive composition for them
 * (auto completion).
 *
 */
class CommandHandlerInterface {

RDM_DECLARE_CLASS_AS_INTERFACE (CommandHandlerInterface)

 public:

  /**
   * \brief Request to process a command.
   *
   * Used to ask a command handler to process a command if he can.
   *
   * If a command handler returns \ref RDM_CMD_PROCESS_DONE, the command can
   * be considered completed. If it returns \ref RDM_CMD_PROCESS_UNKNOWN, the
   * command should be issued to another command handler.
   *
   * \param command The command.
   * \return See method description.
   */
  virtual bool processCommand(const Command& command) =0;

  /**
   * \brief Request to identify a command in a sentence.
   *
   * Used to ask a command handler to try to identify the command in a
   * sentence. Usually using the first word.
   *
   * A command should also be provided. If the command can be identified, the
   * provided command will be modified to construct an equivalent to the
   * sentence in command form and \ref RDM_CMD_IDENTIFY_DONE will be
   * returned. If the command can't be identified \ref RDM_CMD_IDENTIFY_UNKNOWN
   * will be returned.
   *
   * \param sentence The sentence.
   * \param[out] command The command to be modified. See method description.
   * \return See method description.
   */
  virtual bool identifyCommand(const Sentence& sentence,
                               Command& command) const =0;

  /**
   * \brief Request a list of autocomplete options for a command.
   *
   * This method builds an autocomplete candidate list for the provided
   * command or an empty list if there is not valid complete options.
   *
   * \param command The current command.
   * \return The list of valid autocomplete options.
   */
  virtual CommandList getCompletionCandidates(const Command& command) const =0;

};

typedef std::shared_ptr<CommandHandlerInterface> CommandHandlerSharedPtr;

typedef std::vector<CommandHandlerSharedPtr> CommandHandlerList;

}  // namespace roguedm
