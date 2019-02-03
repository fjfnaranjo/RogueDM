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
     * Gets the current configuration status.
     * \return Returns false if config is on an inconsistent state.
     */
    const bool getConfigurationStatus() const;

    /**
     * Field doNotUseNetworking getter.
     * \return Field doNotUseNetworking value.
     */
    const bool getDoNotUseNetworking() const;

    /**
     * Field doNotUseNetworking setter.
     * \param newVal The new doNotUseNetworking field value.
     */
    void setDoNotUseNetworking(bool newVal);

  private:

    /**
     * Creates a new user config file from the base config file.
     * \return False on error.
     */
    bool makeConfigFile();

    /**
     * Open the config file and handle IO errors.
     * \param aFile A reference to a file object to associate with the config
     *              file.
     * \return False on error.
     */
    bool openConfigFile(std::ifstream &aFile);

    /** Configuration status to detect errors. */
    bool configurationStatus = false;

    /** Configuration status message. */
    std::string configurationLastError = std::string();

    /** Do-not-use-networking flag. */
    bool doNotUseNetworking = false;

};

typedef std::shared_ptr<Config> ConfigReference;

// TODO: Should I improve this exception?
class ConfigException : public std::exception {
  public:
    ConfigException() =delete;
    ConfigException(const char*);
    const char* what() const throw();
  private:
    std::string reason;
};

} // namespace roguedm
