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
 * \file MainState.hpp
 * \brief File containing the MainState class declarations.
 */

#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

#include "Game.hpp"
#include "../IOLocal.hpp"
#include "../IORemote.hpp"

#include "../StateInterface.hpp"

#include "../Config.hpp"

#include "../strings.hpp"

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#include <cstdio>

namespace roguedm_main {

/**
 * \brief The main state class.
 *
 * This state represents the game application behavior.
 * \see roguedm::StateInterface
 * \see roguedm::StateInterface.execute()
 */
class MainState : public roguedm::StateInterface
{

  public:

    /**
     * Default constructor (project guidelines requires always a constructor,
     * even if it will be empty).
     */
    MainState();

    /**
     * Default destructor (project guidelines requires always a destructor,
     * even if it will be empty).
     */
    virtual ~MainState();

    /**
     * \brief This method hold the main state tasks, this is, all the pure game
     *        related operations.
     *
     * Override from StateInterface.
     * \see roguedm::StateInterface
     * \see roguedm::StateInterface.execute()
     */
    roguedm::StateResponse execute();

  private:

    /** Copy operator (private because is disabled by default). */
    MainState(const MainState&);

    /** Assing operator (private because is disabled by default). */
    void operator=(const MainState&);

    /** Member to store the state desired application exit code. */
    int status;

};

} // namespace roguedm_main

#endif // MAINSTATE_HPP
