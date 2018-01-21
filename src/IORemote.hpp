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

#ifndef IOREMOTE_HPP
#define IOREMOTE_HPP

#include "CommandHandlerInterface.hpp"
#include "GameComponentInterface.hpp"

#include "Config.hpp"

#include "strings.hpp"

#include "gettext.hpp"

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define RDM_IOREMOTE_MODE_CREATE 0
#define RDM_IOREMOTE_MODE_RESET 1
#define RDM_IOREMOTE_MODE_DELETE 2

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

  public:

    /**
     * Singleton instance management.
     * \param instanceMode Concrete request for the instance manager.
     */
    static IORemote* instance(int instanceMode);

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
    const std::vector<Sentence>* autocompleteListOptions(const Sentence&);

    /**
     * Get the error code value (detecting contruction failures).
     * \return Status code to be returned with cstdlib exit() or 0 if ok.
     */
    int getErrorCode();

  private:

    /** Private constructor to Singleton isolation. */
    IORemote();

    /** Private destructor to Singleton isolation. */
    ~IORemote();

    /** Copy operator (private because is disabled by default). */
    IORemote(const IORemote&);

    /** Assing operator (private because is disabled by default). */
    void operator=(const IORemote&);

    /** Error code */
    int errorCode;

    /** Link to configuration class */
    Config *configuration;

};

} // namespace roguedm

#endif // IOREMOTE_HPP
