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
 * \file GameComponentInterface.hpp
 * \brief File used to contain the GameComponentInterface interface.
 */

#ifndef GAMECOMPONENTINTERFACE_HPP
#define GAMECOMPONENTINTERFACE_HPP

// Useful macros.
#include "macros.hpp"

namespace roguedm {

/**
 * \interface GameComponentInterface
 * \brief C++ interface for the game components called
 *        inside the game main state loop and related.
 *
 * This interface declare the members required by the game states loop
 * to work properly. It includes the update() method.
 * \see update()
 */
class GameComponentInterface
{

  RDM_DECLARE_CLASS_AS_INTERFACE(GameComponentInterface);

  public:

    /** Used by main loops to update a component. */
    virtual void update() =0;

};

} // namespace roguedm

#endif // GAMECOMPONENTINTERFACE_HPP
