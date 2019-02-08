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

#include "Config.hpp"

#include <cstring>
#include <fstream>

#include <SDL2/SDL.h>

#include "strings.hpp"

namespace roguedm {

Config::Config() {

  std::ifstream cfgFile;
  configurationStatus = openConfigFile(cfgFile);
  if(!configurationStatus)
    throw ConfigException(configurationLastError.c_str());

  configurationStatus = parseConfigFile(cfgFile);
  if(!configurationStatus)
    throw ConfigException(configurationLastError.c_str());

}

Config::~Config() =default;

const bool Config::getConfigurationStatus() const {
  return configurationStatus;
}

const bool Config::getDoNotUseNetworking() const {
  return doNotUseNetworking;
}

void Config::setDoNotUseNetworking(bool newVal) {
  doNotUseNetworking = newVal;
}

bool Config::makeConfigFile() {

  std::ifstream cfgFileIn(
    RDM_PATH_HERE RDM_PATH_SHARE RDM_PATH_SEP "config.ini",
    std::ios_base::binary
  );
  if(!cfgFileIn) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,_ (RDM_STR_CFG_BASE_ERROR));
    configurationLastError = std::strerror(errno);
    return false;
  }

  std::ofstream cfgFileOut(
    RDM_PATH_HERE RDM_PATH_CONFIG RDM_PATH_SEP "config.ini",
    std::ios_base::binary
  );
  if(!cfgFileOut) {
    configurationLastError = std::strerror(errno);
    return false;
  }

  cfgFileOut << cfgFileIn.rdbuf();
  if(!cfgFileOut || !cfgFileIn) {
    configurationLastError = std::strerror(errno);
    return false;
  }

  return true;

}

bool Config::openConfigFile(std::ifstream &aFile) {

  aFile.open(
    RDM_PATH_HERE RDM_PATH_CONFIG RDM_PATH_SEP "config.ini",
    std::ios_base::in
  );

  // If opening the config file fails create a new one from the base.
  if(!aFile) {

    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, _ (RDM_STR_CFG_CREATE_NEW));
    if(!makeConfigFile())
      return false;

    aFile.open(
      RDM_PATH_HERE RDM_PATH_CONFIG RDM_PATH_SEP "config.ini",
      std::ios_base::in
    );
    if(!aFile) {
      configurationLastError = std::strerror(errno);
      return false;
    }

  }

  return true;

}

bool Config::parseConfigFile(std::ifstream &aFile) {

  bool in_comment = false;
  bool in_section = false;
  bool in_value = false;
  std::string current_content = "";
  std::string current_section = "general";
  std::string current_setting = "";

  // read the file one character at a time
  char ch;
  while (aFile >> std::noskipws >> ch) {

    // tabs are simply ignored, whenever they appear
    if(ch==RDM_CFG_PARSER_TAB)
      continue;

    // if we find a end line ..
    if (ch==RDM_CFG_PARSER_LINE_SEP || ch==RDM_CFG_PARSER_LINE_SEP_2) {

      // .. if we were in a comment consider the comment ended
      if (in_comment) {
        in_comment = false;
        continue;
      }

      // .. if we were parsing a section name, raise an error
      if(in_section) {
        configurationLastError = _ (RDM_STR_PARSER_INCP_SEC);
        return false;
      }

      // .. if we were parsing a value, store the new value ..
      if (in_value) {
        SDL_LogDebug(
          SDL_LOG_CATEGORY_APPLICATION,
          _ (RDM_STR_CFG_DEBUG),
          current_section.c_str(),
          current_setting.c_str(),
          current_content.c_str()
        );
        in_value = false;
      }

      // .. and clear the current content.
      current_content = "";
      continue;

    }

    // if we are in a comment, ignore everything but a comment end (see above)
    if (in_comment)
      continue;

    // detect a comment line
    if (ch==RDM_CFG_PARSER_COMMENT) {
      // but fail if we were parsing something else
      if(in_section || in_value || !current_content.empty() ) {
        configurationLastError = _ (RDM_STR_PARSER_CMT_OOP);
        return false;
      }
      in_comment = true;
      continue;
    }

    // detect a section start
    if (ch==RDM_CFG_PARSER_GROUP_START) {
      // but fail if we were parsing something else
      if(in_section || in_value || !current_content.empty() ) {
        configurationLastError = _ (RDM_STR_PARSER_SEC_OOP);
        return false;
      }
      in_section = true;
      continue;
    }

    // detect a section end and store its value
    if (in_section && ch==RDM_CFG_PARSER_GROUP_END) {
      // sections can't be empty
      if(current_content.empty()) {
        configurationLastError = _ (RDM_STR_PARSER_SEC_EMTY);
        return false;
      }
      // section ends can't overlap values
      if(in_value) {
        configurationLastError = _ (RDM_STR_PARSER_SEC_IVA);
        return false;
      }
      current_section = current_content.substr(0, current_content.length());
      current_content = "";
      in_section = false;
      continue;
    }

    // detect a section/value separator
    if (ch==RDM_CFG_PARSER_VALUE_SEP) {
      // fail if not after a setting
      if(current_content.empty()) {
        configurationLastError = _ (RDM_STR_PARSER_VSP_OOP);
        return false;
      }
      current_setting = current_content.substr(0, current_content.length());
      current_content = "";
      in_value = true;
      continue;
    }

    // in any other situation, store the character
    current_content += ch;

  }

  return true;

}

ConfigException::ConfigException(const char* why) {
  reason = std::string(format_string(_ (RDM_STR_CFG_LOAD_ERROR), why));
}

const char* ConfigException::what() const throw() {
  return reason.c_str();
}

} // namespace roguedm
