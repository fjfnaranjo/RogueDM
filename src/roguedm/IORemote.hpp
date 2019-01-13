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
     */
    void update() override;

    /**
     * Used by IOLocal to ask the command handler a response for a command.
     */
    int processCommand(const Sentence&) override;

    /**
     * Used by IOLocal to ask the command handler a autocomplete suggestion.
     */
    const int autocomplete(Sentence&) override;

    /**
     * Used by IOLocal to ask the command handler a autocomplete list.
     */
    const SentenceListReference autocompleteListOptions(const Sentence&)
      override;

    /**
     * Get the error code value (detecting contruction failures).
     * \return Status code to be returned with cstdlib exit() or 0 if ok.
     */
    int getErrorCode();

  private:

    /** Error code */
    int errorCode;

    /** Link to configuration class */
    ConfigReference configuration;

};

typedef std::shared_ptr<IORemote> IORemoteReference;

} // namespace roguedm
