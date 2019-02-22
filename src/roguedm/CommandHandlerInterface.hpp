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
     * \brief Used to ask the command handler a response for a command.
     *
     * If a command handler returns \ref RDM_COMMAND_DONE, the command can
     * be considered completed. If it returns \ref RDM_COMMAND_UNKNOWN, the
     * command should be issued to another command handler.
     * \param s The command sentence.
     * \return 0 or 1, see method description.
     */
    virtual int processCommand(const Sentence& s) =0;

    /**
     * \brief Used to ask the command handler an autocomplete suggestion.
     *
     * Modify the command sentence and complete it if possible, returning
     * \ref RDM_COMMAND_AC_COMPLETED on success or \ref RDM_COMMAND_AC_NEXT to
     * signify that another command handler should try to complete it.
     * \param[out] s The command sentence.
     * \return 0 or 1, see method description.
     */
    virtual int autocomplete(Sentence& s) const =0;

    /**
     * \brief Used to ask the command handler an autocomplete candidate list.
     *
     * This method builds an autocomplete candidate list for the current
     * command or an empty list if there is not valid complete options.
     * \param s A reference to the current command line.
     * \return The Sentence vector or an empty one.
     */
    virtual SentenceListReference autocompleteListOptions(
      const Sentence& s
    ) const =0;

};

} // namespace roguedm
