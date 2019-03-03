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
 * \file translations.hpp
 * \brief Translation table between UTF8 sequences and ASCII 850 positions.
 */

#pragma once

#include <map>
#include <string>

namespace roguedm_gui {

// WARNING: If this has to be modified dynamically later, remember is bounded to
// its compilation unit b/c static

/** Table used to translate a UTF-8 input character to a ASCII 850 position. */
static std::map<std::string,int> transUtf8 = {

  {u8" ", 32},
  {u8"!", 33},
  {u8"\"", 34},
  {u8"#", 35},
  {u8"$", 36},
  {u8"%", 37},
  {u8"&", 38},
  {u8"'", 39},
  {u8"(", 40},
  {u8")", 41},
  {u8"*", 42},
  {u8"+", 43},
  {u8",", 44},
  {u8"-", 45},
  {u8".", 46},
  {u8"/", 47},

  {u8"0", 48},
  {u8"1", 49},
  {u8"2", 50},
  {u8"3", 51},
  {u8"4", 52},
  {u8"5", 53},
  {u8"6", 54},
  {u8"7", 55},
  {u8"8", 56},
  {u8"9", 57},
  {u8":", 58},
  {u8";", 59},
  {u8"<", 60},
  {u8"=", 61},
  {u8">", 62},
  {u8"?", 63},

  {u8"@", 64},
  {u8"A", 65},
  {u8"B", 66},
  {u8"C", 67},
  {u8"D", 68},
  {u8"E", 69},
  {u8"F", 70},
  {u8"G", 71},
  {u8"H", 72},
  {u8"I", 73},
  {u8"J", 74},
  {u8"K", 75},
  {u8"L", 76},
  {u8"M", 77},
  {u8"N", 78},
  {u8"O", 79},

  {u8"P", 80},
  {u8"Q", 81},
  {u8"R", 82},
  {u8"S", 83},
  {u8"T", 84},
  {u8"U", 85},
  {u8"V", 86},
  {u8"W", 87},
  {u8"X", 88},
  {u8"Y", 89},
  {u8"Z", 90},
  {u8"[", 91},
  {u8"\\", 92},
  {u8"]", 93},
  {u8"^", 94},
  {u8"_", 95},

  {u8"`", 96},
  {u8"a", 97},
  {u8"b", 98},
  {u8"c", 99},
  {u8"d", 100},
  {u8"e", 101},
  {u8"f", 102},
  {u8"g", 103},
  {u8"h", 104},
  {u8"i", 105},
  {u8"j", 106},
  {u8"k", 107},
  {u8"l", 108},
  {u8"m", 109},
  {u8"n", 110},
  {u8"o", 111},

  {u8"p", 112},
  {u8"q", 113},
  {u8"r", 114},
  {u8"s", 115},
  {u8"t", 116},
  {u8"u", 117},
  {u8"v", 118},
  {u8"w", 119},
  {u8"x", 120},
  {u8"y", 121},
  {u8"z", 122},
  {u8"{", 123},
  {u8"|", 124},
  {u8"}", 125},
  {u8"~", 126},

  {u8"\u00c7", 128},
  {u8"\u00fc", 129},
  {u8"\u00e9", 130},
  {u8"\u00e7", 135},
  {u8"\u00c9", 144},
  {u8"\u00e1", 160},
  {u8"\u00ed", 161},
  {u8"\u00f3", 162},
  {u8"\u00fa", 163},
  {u8"\u00f1", 164},
  {u8"\u00d1", 165},
  {u8"\u00aa", 166},
  {u8"\u00ba", 167},
  {u8"\u00bf", 168},
  {u8"\u00a1", 173},
  {u8"\u00c1", 181},
  {u8"\u20ac", 213},
  {u8"\u00cd", 214},
  {u8"\u00d3", 224},
  {u8"\u00da", 233},

};

}  // namespace roguedm_gui
