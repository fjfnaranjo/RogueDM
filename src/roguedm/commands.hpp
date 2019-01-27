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
 * \file commands.hpp
 * \brief Declarations for command handlers types and operations.
 *
 * This file declares the important Sentence[Word[std::string]] types used to
 * serialize the commands that control the application/game.
 *
 * It also declares the identity mechanism to refer to different game element
 * in an unequivocal way.
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
  std::string wordContent;
  unsigned int wordClass;
  unsigned int wordId;
};

/** \brief Word's vector to contain full sentences. */
typedef std::vector<Word> Sentence;

/** \brief Reference to a sentence. */
typedef std::shared_ptr<Sentence> SentenceReference;

/** \brief Senteces's vector to contain a list of sentences. */
typedef std::vector<Sentence> SentenceList;

/** \brief Reference to a vector of sentences. */
typedef std::shared_ptr<SentenceList> SentenceListReference;

} // namespace roguedm
