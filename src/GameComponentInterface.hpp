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

namespace roguedm {

/**
 * \interface GameComponentInterface
 * \brief C++ interface for the game components called
 *        inside the game main state loop and related.
 *
 * This interface declare the members required by the game states loop
 * to work propertly. It includes the update() method.
 *
 * \see update()
 */
class GameComponentInterface
{

  public:

    /** Used by gamne loops to update a game component. */
    virtual void update() =0;

  protected:

    /** Cleared constructor to avoid interface use as a class. */
    GameComponentInterface() {};

    /** Cleared destructor to avoid interface use as a class. */
    virtual ~GameComponentInterface() {};

  private:

    /** Copy operator (private because is disabled by default). */
    GameComponentInterface(const GameComponentInterface&);

    /** Assing operator (private because is disabled by default). */
    void operator=(const GameComponentInterface&);

};

} // namespace roguedm

#endif // GAMECOMPONENTINTERFACE_HPP
