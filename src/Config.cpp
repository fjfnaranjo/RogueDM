// Copyright 2010 Google Code project roguedm development team.

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
 * \file Config.cpp
 * \brief File containing the configuration class definitions.
 */

#include "Config.hpp"

namespace roguedm {

Config::Config() {
  configurationStatus = 0;
  doNotUseNetworking = 0;
}

Config::~Config() {}

// Singleton instance manager.
Config* Config::instance(int instanceMode) {

  static Config *instance;

  if(instance) {
    switch(instanceMode) {
      case RDM_CONFIG_MODE_RESET:
        delete instance;
        instance = new Config();
        break;
      case RDM_CONFIG_MODE_DELETE:
        delete instance;
      case RDM_CONFIG_MODE_CREATE:
        break;
      default:
        return 0;
        break;
    }
  } else {
    switch(instanceMode) {
      case RDM_CONFIG_MODE_CREATE:
        instance = new Config();
        break;
      case RDM_CONFIG_MODE_DELETE:
      case RDM_CONFIG_MODE_RESET:
      default:
        return 0;
        break;
    }
  }

  return instance;

}

const int Config::getConfigurationStatus() {
  return configurationStatus;
}

const int Config::getDoNotUseNetworking() {
  return doNotUseNetworking;
}

void Config::setDoNotUseNetworking(int newVal) {
  doNotUseNetworking = newVal;
}

} // namespace roguedm
