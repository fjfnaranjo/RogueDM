// Copyright 2022 Fco. Jose Fdez. Naranjo AKA github.com/fjfnaranjo

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

#include <map>
#include <memory>
#include <string>

#include "macros.hpp"

#define RDM_CFG_PARSER_COMMENT '#'
#define RDM_CFG_PARSER_GROUP_START '['
#define RDM_CFG_PARSER_GROUP_END ']'
#define RDM_CFG_PARSER_VALUE_SEP '='
#define RDM_CFG_PARSER_LINE_SEP '\n'
#define RDM_CFG_PARSER_LINE_SEP_2 '\r'
#define RDM_CFG_PARSER_TAB '\t'

#define RDM_CFG_PARSER_TRUE "yes"
#define RDM_CFG_PARSER_FALSE "no"

#define RDM_CFG_CHARMAP_PREFIX "charmap:"

namespace roguedm {

/**
 * This is the typedef for the Setting->Value map.
 *
 * See \ref app-configuration.
 */
typedef std::map<std::string, std::string> ConfigSettings;

/**
 * This is the typedef for the Section->Setting map.
 *
 * See \ref app-configuration.
 */
typedef std::map<std::string, ConfigSettings> ConfigSections;

/**
 * \brief The configuration management class.
 *
 * Manage all operations related to load, store or change configuration.
 */
class Config {

RDM_DECLARE_CLASS_AS_SINGLETON(Config)

 public:

  Config();

  /** Try to load the settings from the configuration file. */
  bool loadFromFile();

  /** Returns the last error produced in \ref loadFromFile . */
  std::string getConfigurationLoadError() const;

  /**
   * Check if a section exists by its name.
   *
   * See \ref app-configuration.
   */
  bool hasSection(const std::string&) const;

  /**
   * Check if a section and a setting exists by their names.
   *
   * See \ref app-configuration.
   */
  bool hasSetting(const std::string&, const std::string&) const;

  /**
   * Set a new setting value for a particular section.
   *
   * This will create a new section and setting if needed.
   *
   * See \ref app-configuration.
   */
  void setSettingValue(const std::string&, const std::string&,
                       const std::string&);

  /**
   * Works like \ref setSettingValue but stores values of type int.
   */
  void setSettingIntValue(const std::string&, const std::string&, const int&);

  /**
   * Works like \ref setSettingValue but stores values of type bool.
   */
  void setSettingBoolValue(const std::string&, const std::string&, const bool&);

  /**
   * Get the value for a particular setting in a particular section.
   *
   * See \ref app-configuration.
   */
  std::string getSettingValue(const std::string&, const std::string&,
                              const std::string&) const;

  /**
   * Works like \ref getSettingValue but returns values of type int.
   */
  int getSettingIntValue(const std::string&, const std::string&,
                         const int&) const;

  /**
   * Works like \ref getSettingValue but returns values of type bool.
   */
  bool getSettingBoolValue(const std::string&, const std::string&,
                           const bool&) const;

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

  /** Configuration load error message if any. */
  std::string configurationLoadError;

  /**
   * Parses a configuration file into the \ref sections member
   *
   * ```
   * [section]
   * setting=value
   * ```
   */
  bool parseConfigFile(std::ifstream &aFile);

  /**
   * Section->Setting->Value 2 level mapping with the app settings.
   *
   * See \ref app-configuration.
   */
  ConfigSections sections;

};

typedef std::shared_ptr<Config> ConfigSharedPtr;

}  // namespace roguedm
