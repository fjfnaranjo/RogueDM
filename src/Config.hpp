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
 * \file Config.hpp
 * \brief File containing the configuration management class declarations.
 */

#pragma once

#include "macros.hpp"

namespace roguedm {

/**
 * \brief The configuration management class.
 *
 * Manage all operations related to load, store or change configuration.
 *
 * Pattern Singleton.
 */
class Config
{

  RDM_DECLARE_CLASS_AS_SINGLETON(Config)

  public:

    /**
     * Gets the current configuration status code
     * \return The status code.
     */
    const int getConfigurationStatus();

    /**
     * Field doNotUseNetworking getter.
     * \return Field doNotUseNetworking value.
     */
    const int getDoNotUseNetworking();

    /**
     * Field doNotUseNetworking setter.
     * \param newVal The new doNotUseNetworking field value.
     */
    void setDoNotUseNetworking(int newVal);

  private:

    /** Configuration staus to detect errors. */
    int configurationStatus;

    /** Do-not-use-networking flag. */
    int doNotUseNetworking;

};

} // namespace roguedm
