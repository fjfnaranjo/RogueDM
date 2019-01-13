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

#include "../StageInterface.hpp"

namespace roguedm_main {

/**
 * \brief The main stage class.
 *
 * This stage represents the game application behavior.
 */
class Main : public roguedm::StageInterface
{

  public:

    /**
     * Default constructor (project guidelines requires always a constructor,
     * even if it will be empty).
     */
    Main();

    /**
     * Default destructor (project guidelines requires always a destructor,
     * even if it will be empty).
     */
    virtual ~Main();

    /**
     * \brief This method hold the main stage tasks, this is, all the pure game
     *        related operations.
     */
    roguedm::StageResponse execute() override;

  private:

    /** Copy operator (private because is disabled by default). */
    Main(const Main&);

    /** Assign operator (private because is disabled by default). */
    void operator=(const Main&);

    /** Member to store the stage desired application exit code. */
    int status;

};

} // namespace roguedm_main
