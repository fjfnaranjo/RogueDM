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

#include <memory>
#include <vector>

#include "Word.hpp"

namespace roguedm {

/** \brief Word's vector to contain full sentences. */
typedef std::vector<Word> Sentence;

/** \brief Shared pointer to a sentence. */
typedef std::shared_ptr<Sentence> SentenceSharedPtr;

/** \brief Senteces's vector to contain a list of sentences. */
typedef std::vector<Sentence> SentenceList;

/** \brief Shared pointer to a vector of sentences. */
typedef std::shared_ptr<SentenceList> SentenceListSharedPtr;

}  // namespace roguedm
