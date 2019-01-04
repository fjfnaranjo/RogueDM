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
 * \file strings.hpp
 * \brief File containing the strings used by the application CLI.
 *
 * This file should be used to store all strings and templates exposed to the
 * application CLI interface. For convenience, it also defines the gettext
 * macro.
 */

#pragma once

#include "version.hpp"

#define RDM_STR_VERSION_STRING   "Rogue's Dungeon Master Version "
#define RDM_STR_USAGE_VERSION    "--version"
#define RDM_STR_USAGE_HELP       "--help"
#define RDM_STR_USAGE_LOCAL      "--local"
#define RDM_STR_USAGE            "usage: %s [--version] [--help] [--local]"

#define RDM_STR_USAGE_UKNOWN     "Unknown program argument: %s\n"

#define RDM_STR_SDL_ERROR        "SDL error reported: %s\n"
#define RDM_STR_SDL_NET_ERROR    "SDLNet error reported: %s\n"

#define RDM_STR_NOT_NETWORKING   "Networking disabled...\n"
