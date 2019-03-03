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

#include <string>

#include <SDL.h>

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

/**
 * \brief Struct to define word kinds colors and enclosing decorations.
 *
 * * fgColor: Color for the foreground.
 * * bgColor: Color for the background.
 * * lDecorator: Left-side of the enclosing decoration.
 * * rDecorator: Right-side of the enclosing decoration.
 */
struct WordKind {
  SDL_Color fgColor;
  SDL_Color bgColor;
  std::string lDecorator;
  std::string rDecorator;
};

// WARNING: If this has to be modified dynamically later, remember is bounded to
// its compilation unit b/c static

/** Table holding all the word kinds */
static WordKind wordKinds[] = {
    {{200, 200, 200, 0},{   0,   0,   0, 0}, u8""      , u8""      },
    {{255, 255, 255, 0},{   0,   0,   0, 0}, u8"\\"    , u8""      },
    {{  0, 128,   0, 0},{   0,   0,   0, 0}, u8"'"     , u8"'"     },
    {{  0, 255,   0, 0},{   0,   0,   0, 0}, u8"'"     , u8"'"     },
    {{  0,  64, 255, 0},{   0,   0,   0, 0}, u8""      , u8"?"     },
    {{  0, 192, 255, 0},{   0,   0,   0, 0}, u8""      , u8"?"     },
    {{255,   0,   0, 0},{   0,   0,   0, 0}, u8""      , u8"!!"    },
    {{128,   0,   0, 0},{   0,   0,   0, 0}, u8""      , u8"!"     },
    {{255,   0, 192, 0},{   0,   0,   0, 0}, u8""      , u8"!!!"   },
    {{255, 128,   0, 0},{   0,   0,   0, 0}, u8"["     , u8"]"     },
    {{224, 224,   0, 0},{   0,   0,   0, 0}, u8"{"     , u8"}"     },
    {{255, 255,   0, 0},{   0,   0,   0, 0}, u8"{"     , u8"}*"    },
    {{192, 255,  64, 0},{   0,   0,   0, 0}, u8"{"     , u8"}s"    },
    {{255, 255, 255, 0},{   0,   0,   0, 0}, u8"{"     , u8"}**"   },
    {{192, 192,  64, 0},{   0,   0,   0, 0}, u8"{"     , u8"}***"  },
};

/**
 * \brief Struct to contain a word content, its kind and its id.
 *
 * This type holds a 'kind' value (\ref WordKind) to identify different words
 * with different purposes and an unique identifier to refer to particular
 * elements in the session state.
 */
struct Word {
  std::string content;
  unsigned int kind;
  unsigned int id;
};

}  // namespace roguedm
