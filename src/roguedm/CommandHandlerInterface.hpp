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

  RDM_DECLARE_CLASS_AS_INTERFACE(CommandHandlerInterface);

  public:

    /**
     * \brief Used to ask the command handler a response for a command.
     *
     * If a command proccesor returns 0, the command will be issued to other
     * command proccesor. If it returns 1, the command line will be reset.
     * \param s A reference to the current command line.
     * \return 0 or 1, see interface description.
     */
    virtual int processCommand(const Sentence& s) =0;

    /**
     * \brief Used to ask the command handler an autocomplete suggestion.
     *
     * This method modify the command line and complete it, returning 1 to
     * notify Sdl2IO that the work is done. If it can't do it, it will
     * return 0 and another command handler will try.
     * \param[out] s A reference to the current command line.
     * \return 0 or 1, see interface description.
     */
    virtual const int autocomplete(Sentence& s) =0;

    /**
     * \brief Used to ask the command handler an autocomplete candidate list.
     *
     * This method build a autocomplete list for the current command or an empty
     * list if there is not valid complete options.
     * \param s A reference to the current command line.
     * \return The Sentence vector or an empty one.
     */
    virtual const SentenceListReference autocompleteListOptions(
      const Sentence& s
    ) =0;

};

} // namespace roguedm
