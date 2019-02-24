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

#include "../Config.hpp"
#include "../GameComponentInterface.hpp"
#include "../commands/CommandHandlerInterface.hpp"
#include "../macros.hpp"

namespace roguedm {

/**
 * \brief The network management class.
 *
 * Manage all networking related operations and abstract its complexity.
 *
 * Pattern Singleton.
 */
class Network : CommandHandlerInterface, GameComponentInterface
{

  RDM_DECLARE_CLASS_AS_NOCPNOMV(Network)

  public:

    Network();
    ~Network();

    /** SDL resources class initializer */
    bool initNetwork();

    /**
     * Method used when the main app has time to allow a network management
     * step from the game loop.
     */
    void update() override;

    /** Request to process a command. */
    bool processCommand(const Command&) override;

    /** Request to identify a command in a sentence. */
    bool identifyCommand(const Sentence&, Command&) const override;

    /** Request a list of autocomplete options for a command. */
    CommandList getCompletionCandidates(const Command&)
      const override;

  private:

    /** Stores if SDL resources have been initialized successfully. */
    bool initSuccess;

    /** Link to configuration class */
    ConfigSharedPtr config;

};

} // namespace roguedm
