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
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,_ (RDM_STR_SETTINGS_NOBASE));
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

    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, _ (RDM_STR_SETTINGS_NEW));
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

ConfigException::ConfigException(const char* why) {
  reason = std::string(format_string(_ (RDM_STR_SETTINGS_NOLOAD), why));
}

const char* ConfigException::what() const throw() {
  return reason.c_str();
}

} // namespace roguedm
