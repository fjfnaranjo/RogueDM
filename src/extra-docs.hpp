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
 * \file extra-docs.hpp
 * \brief File used to document the namespaces and store Doxygen's extra docs.
 */

#ifndef EXTRA_DOCS_HPP
#define EXTRA_DOCS_HPP

/**
 * \namespace roguedm
 * \brief General applicaction namespace to avoid general scope polution.
 *
 * This namespace is used for the general application operations and types.
 * Another namespaces must be created to contain game state specific stuff, like
 * roguedm_main. This namespace also includes local keyboar->display interaction
 * and multiplayer interaction.
 */

/**
 * \namespace roguedm_main
 * \brief Namespace for application main state (game) stuff.
 *
 * This is the namespace for the main state or the game itself. It contents all
 * the operations, references and types related with the game interaction.
 */

#endif // EXTRA_DOCS_HPP
