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

#include "commands.hpp"

// TODO: This should be bool

#define RDM_COMMAND_DONE    0
#define RDM_COMMAND_UNKNOWN 1

#define RDM_COMMAND_AC_COMPLETED 0
#define RDM_COMMAND_AC_NEXT      1

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
class CommandHandlerInterface
{

  RDM_DECLARE_CLASS_AS_INTERFACE(CommandHandlerInterface)

  public:

    /**
     * \brief Request to process a command.
     *
     * Used to ask a command handler to process a command if he can.
     *
     * If a command handler returns \ref RDM_COMMAND_DONE, the command can
     * be considered completed. If it returns \ref RDM_COMMAND_UNKNOWN, the
     * command should be issued to another command handler.
     * \param command The command.
     * \return See method description.
     */
    virtual int processCommand(const Sentence& command) =0;

    /**
     * \brief Request to autocomplete a command.
     *
     * Used to ask a command handler to autocomplete a partial command if he
     * can.
     *
     * It will complete the command it if possible, returning
     * \ref RDM_COMMAND_AC_COMPLETED on success or \ref RDM_COMMAND_AC_NEXT to
     * signify that another command handler should try to complete it.
     * \param[out] command The command.
     * \return 0 or 1, see method description.
     */
    virtual int autocomplete(Sentence& command) const =0;

    /**
     * \brief Request a list of autocomplete options.
     *
     * This method builds an autocomplete candidate list for the current
     * command or an empty list if there is not valid complete options.
     * \param command The current command.
     * \return The list of valid autocomplete options.
     */
    virtual SentenceList autocompleteListOptions(
      const Sentence& command
    ) const =0;

};

} // namespace roguedm
