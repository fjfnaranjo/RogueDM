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
 * \file Config.cpp
 * \brief File containing the configuration class definitions.
 */

#include "Config.hpp"

namespace roguedm {

Config::Config() {
  configurationStatus = 0;
  doNotUseNetworking = 0;
}

Config::~Config() =default;

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
