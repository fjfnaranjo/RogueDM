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

/**
 * \file IORemote.hpp
 * \brief File containing the IORemote class declarations.
 */

#pragma once

#include "macros.hpp"
#include "CommandHandlerInterface.hpp"
#include "Config.hpp"
#include "GameComponentInterface.hpp"

namespace roguedm {

/**
 * \brief The network management class.
 *
 * Manage all networking related operations and abstract its complexity.
 *
 * Pattern Singleton.
 */
class IORemote : CommandHandlerInterface, GameComponentInterface
{

  RDM_DECLARE_CLASS_AS_SINGLETON(IORemote)

  public:

    /**
     * Method used when the main app has time to allow a network management
     * step from the game loop.
     * \see GameComponentInterface.update()
     */
    void update();

    /**
     * Used by IOLocal to ask the command handler a response for a command.
     * \see CommandHandlerInterface.processCommand()
     */
    int processCommand(const Sentence&);

    /**
     * Used by IOLocal to ask the command handler a autocomplete suggestion.
     * \see CommandHandlerInterface.autocomplete()
     */
    const int autocomplete(Sentence&);

    /**
     * Used by IOLocal to ask the command handler a autocomplete list.
     * \see CommandHandlerInterface.autocompleteListOptions()
     */
    const SentenceList* autocompleteListOptions(const Sentence&);

    /**
     * Get the error code value (detecting contruction failures).
     * \return Status code to be returned with cstdlib exit() or 0 if ok.
     */
    int getErrorCode();

  private:

    /** Error code */
    int errorCode;

    /** Link to configuration class */
    Config *configuration;

};

} // namespace roguedm
