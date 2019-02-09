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
 * application CLI interface.
 */

#pragma once

#include <locale>
#include <string>

#define _ gettext

#include "paths.hpp"
#include "version.hpp"

#define RDM_STR_MAX              255

#define RDM_STR_VERSION_STRING   "Rogue's Dungeon Master Version %s .\n"
#define RDM_STR_USAGE_VERSION    "--version"
#define RDM_STR_USAGE_HELP       "--help"
#define RDM_STR_USAGE_LOCAL      "--local"
#define RDM_STR_USAGE            "usage: %s [--version] [--help] [--local]\n"

#define RDM_STR_USAGE_UKNOWN     "Unknown program argument: %s\n"

#define RDM_STR_CFG_CREATE_NEW   "Existing config file not accessible, trying to create a new one.\n"
#define RDM_STR_CFG_BASE_ERROR   "Can't access the basic config file. Check application paths.\n"
#define RDM_STR_CFG_LOAD_ERROR   "Error loading settings: %s.\n"
#define RDM_STR_CFG_DEBUG        "Section: %s Setting: %s Value: %s\n"

#define RDM_STR_PARSER_INCP_SEC  "Incomplete section.\n"
#define RDM_STR_PARSER_CMT_OOP   "Comment start out of place.\n"
#define RDM_STR_PARSER_SEC_OOP   "Section start out of place.\n"
#define RDM_STR_PARSER_VSP_OOP   "Setting/value separator out of place.\n"
#define RDM_STR_PARSER_SEC_EMTY  "Empty section.\n"
#define RDM_STR_PARSER_SEC_IVA   "Section started inside a value.\n"

#define RDM_STR_SDL_ERROR        "SDL error reported: %s\n"
#define RDM_STR_SDL_NET_ERROR    "SDLNet error reported: %s\n"

#define RDM_STR_NOT_NETWORKING   "Networking disabled...\n"

#define RDM_ERR_SETTINGS_ERROR   1
#define RDM_ERR_ARGS_ERROR       2
#define RDM_ERR_SDL2_ERROR       3
#define RDM_ERR_NETWORK_ERROR    4

namespace roguedm {

std::string format_string(const char* format, ...);

}
