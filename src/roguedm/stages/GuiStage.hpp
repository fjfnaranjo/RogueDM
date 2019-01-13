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

namespace roguedm_game {

/**
 * \brief The GUI stage class.
 *
 * This stage represents an standard user run of the application. It creates
 * a visual GUI and allow hosting and connection of games.
 */
class GuiStage : public roguedm::StageInterface
{

  public:

    /**
     * Default constructor (project guidelines requires always a constructor,
     * even if it will be empty).
     */
    GuiStage();

    /**
     * Default destructor (project guidelines requires always a destructor,
     * even if it will be empty).
     */
    virtual ~GuiStage();

    /**
     * \brief The GUI stage code.
     */
    roguedm::StageResponse execute() override;

  private:

    /** Copy operator (private because is disabled by default). */
    GuiStage(const GuiStage&);

    /** Assign operator (private because is disabled by default). */
    void operator=(const GuiStage&);

    /** Member to store the stage desired application exit code. */
    int status;

};

} // namespace roguedm_main
