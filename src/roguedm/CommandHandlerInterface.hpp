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
 * \file CommandHandlerInterface.hpp
 * \brief File used to contain the CommandHandlerInterface interface.
 */

#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "macros.hpp"

// Word types (from 0 to n).
#define RDM_WCLASS_NORMAL          0
#define RDM_WCLASS_COMMAND         1
#define RDM_WCLASS_NPC_ALLIED      2
#define RDM_WCLASS_NPC_ALLIED_CMB  3
#define RDM_WCLASS_NPC_NEUTRAL     4
#define RDM_WCLASS_NPC_NEUTRAL_CMB 5
#define RDM_WCLASS_NPC_ENEMY       6
#define RDM_WCLASS_NPC_ENEMY_LIGHT 7
#define RDM_WCLASS_NPC_ENEMY_HARD  8
#define RDM_WCLASS_PLACE           9
#define RDM_WCLASS_OBJECT          10
#define RDM_WCLASS_OBJECT_MAGIC    11
#define RDM_WCLASS_OBJECT_SET      12
#define RDM_WCLASS_OBJECT_UNIQ     13
#define RDM_WCLASS_OBJECT_EPIC     14

// Number of word types (used to define the size of the array who holds types).
#define RDM_WCLASS_TOTAL           15

// Empty command
#define RDM_WEMPTY                 u8""

namespace roguedm {

/** \brief Struct to contain a word content and type. */
struct Word {
  char[] wordContent;
  int wordClass;
};

/** \brief Word's vector to contain full sentences. */
typedef std::vector<Word> Sentence;

/** \brief Reference to a sentence. */
typedef std::shared_ptr<Sentence> SentenceReference;

/** \brief Senteces's vector to contain a list of sentences. */
typedef std::vector<Sentence> SentenceList;

/** \brief Reference to a vector of sentences. */
typedef std::shared_ptr<SentenceList> SentenceListReference;

/**
 * \interface CommandHandlerInterface
 * \brief C++ interface for any IOLocal command/messages procesor.
 *
 * This interface declare the members required by a game command proccesor
 * to work propertly. It use the Sentence type to alter the the current
 * application command line, and also to return a list of posible
 * values for autocomplete.
 *
 * A vector of classes extending this interface is mantained inside the IOLocal
 * class. This classes are asked to solve the command by order, stopping in the
 * first that handle it correctly.
 * \see Sentence
 */
class CommandHandlerInterface
{

  RDM_DECLARE_CLASS_AS_INTERFACE(CommandHandlerInterface);

  public:

    /**
     * \brief Used by IOLocal to ask the command handler a response for a
     *        command.
     *
     * If a command proccesor returns 0, the command will be issued to other
     * command proccesor. If it returns 1, the command line will be reset.
     * \param s A reference to the current command line.
     * \return 0 or 1, see interface description.
     */
    virtual int processCommand(const Sentence& s) =0;

    /**
     * \brief Used by IOLocal to ask the command handler a autocomplete
     *        suggestion.
     *
     * This method modify the command line and complete it, returning 1 to
     * notify IOLocal that the work is done. If it can't do it, it will
     * return 0 and another command handler will try.
     * \param[out] s A reference to the current command line.
     * \return 0 or 1, see interface description.
     */
    virtual const int autocomplete(Sentence& s) =0;

    /**
     * \brief Used by IOLocal to ask the command handler a autocomplete list.
     *
     * This method build a autocomplete list for the current command or an empty
     * list if there is not valid complete options.
     * \param s A reference to the current command line.
     * \return The Sentence vector or an empty one.
     */
    virtual const SentenceListReference autocompleteListOptions(
      const Sentence& s
    ) =0;

};

} // namespace roguedm
