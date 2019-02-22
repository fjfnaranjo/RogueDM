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

#include "Config.hpp"

namespace roguedm {

/**
 * \brief Main application class.
 *
 * This class is used to parse the program arguments and do all the generic
 * steps expected on a CLI program launch (help text, I18N init, ...).
 *
 * It also runs the application stage loop. See \ref application-design.
 */
class Application {

  RDM_DECLARE_CLASS_AS_NOCPNOMV(Application)

  public:

    Application();

    /**
     * Runs the actual program inner/main code.
     * \param argc main() arguments count.
     * \param argv main() arguments values.
     * \return Status code to be returned by main().
     */
    int run(int argc, char *argv[]);

  private:

    /** Do an early search for a --verbose param to enable detailed logging. */
    void process_verbosity(int argc, char *argv[]);

    /**
     * Processes the program arguments.
     *
     * * Sets configuration from arguments.
     * * Prints usage information.
     * * Prints version information.
     *
     * \param argc main() arguments count.
     * \param argv main() arguments values.
     * \param configuration Global configuration object.
     */
    void process_arguments (
      int argc, char* argv[],
      const ConfigSharedPtr& configuration
    );

    int exitStatus;
    bool keepRunning;
    bool argumentsError;

};

} // namespace roguedm
