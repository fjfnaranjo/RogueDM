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
#include <string>

#include <SDL.h>

#include "strings.hpp"

namespace roguedm {

Config::Config() {
  configurationLoadError = "";
  sections = {};
}

bool Config::loadFromFile() {
  std::ifstream cfgFile;
  if (!openConfigFile(cfgFile))
    return false;
  if (!parseConfigFile(cfgFile))
    return false;
  return true;
}

std::string Config::getConfigurationLoadError() const {
  return configurationLoadError;
}

bool Config::hasSection(const std::string &section) const {
  const auto sectionsIt = sections.find(section);
  if (sectionsIt == sections.end())
    return false;
  else
    return true;
}

bool Config::hasSetting(const std::string &section,
                        const std::string &setting) const {
  if (!hasSection(section))
    return false;
  const auto sectionsIt = sections.find(section);
  const auto settingsIt = sectionsIt->second.find(setting);
  if (settingsIt == sectionsIt->second.end())
    return false;
  else
    return true;
}

void Config::setSettingValue(const std::string &section,
                             const std::string &setting,
                             const std::string &value) {
  if (section.empty() || setting.empty() || value.empty())
    return;
  if (!hasSection(section)) {
    ConfigSettings newSetting = { { setting, value } };
    sections[section] = newSetting;
  }
  sections[section][setting] = value.substr(0, value.length());
}

void Config::setSettingIntValue(const std::string &section,
                                const std::string &setting, const int &value) {
  setSettingValue(section, setting, std::to_string(value));
}

void Config::setSettingBoolValue(const std::string &section,
                                 const std::string &setting,
                                 const bool &value) {
  if (value)
    setSettingValue(section, setting, RDM_CFG_PARSER_TRUE);
  else
    setSettingValue(section, setting, RDM_CFG_PARSER_FALSE);
}

std::string Config::getSettingValue(const std::string &section,
                                    const std::string &setting,
                                    const std::string &default_) const {
  if (hasSetting(section, setting)) {
    const auto sectionsIt = sections.find(section);
    const auto settingsIt = sectionsIt->second.find(setting);
    return settingsIt->second;
  } else {
    return default_;
  }
}

int Config::getSettingIntValue(const std::string &section,
                               const std::string &setting,
                               const int &default_) const {
  return std::stoi(getSettingValue(section, setting, std::to_string(default_)));
}

bool Config::getSettingBoolValue(const std::string &section,
                                 const std::string &setting,
                                 const bool &default_) const {
  if (RDM_CFG_PARSER_TRUE
      == getSettingValue(section, setting,
                         default_ ? RDM_CFG_PARSER_TRUE : RDM_CFG_PARSER_FALSE))
    return true;
  else
    return false;
}

bool Config::makeConfigFile() {

  std::ifstream cfgFileIn(
      RDM_PATH_HERE RDM_PATH_SHARE RDM_PATH_SEP "config.ini",
      std::ios_base::binary);
  if (!cfgFileIn) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, RDM_STR_CFG_BASE_ERROR);
    configurationLoadError = std::strerror(errno);
    return false;
  }

  std::ofstream cfgFileOut(
  RDM_PATH_HERE RDM_PATH_CONFIG RDM_PATH_SEP "config.ini",
                           std::ios_base::binary);
  if (!cfgFileOut) {
    configurationLoadError = std::strerror(errno);
    return false;
  }

  cfgFileOut << cfgFileIn.rdbuf();
  if (!cfgFileOut || !cfgFileIn) {
    configurationLoadError = std::strerror(errno);
    return false;
  }

  return true;

}

bool Config::openConfigFile(std::ifstream &aFile) {

  aFile.open(RDM_PATH_HERE RDM_PATH_CONFIG RDM_PATH_SEP "config.ini",
             std::ios_base::in);

  // If opening the config file fails create a new one from the base.
  if (!aFile) {

    SDL_Log(RDM_STR_CFG_CREATE_NEW);
    if (!makeConfigFile())
      return false;

    aFile.open(
    RDM_PATH_HERE RDM_PATH_CONFIG RDM_PATH_SEP "config.ini",
               std::ios_base::in);
    if (!aFile) {
      configurationLoadError = std::strerror(errno);
      return false;
    }

  }

  return true;

}

bool Config::parseConfigFile(std::ifstream &aFile) {

  bool inComment = false;
  bool inSection = false;
  bool inValue = false;
  std::string currentContent = "";
  std::string currentSection = "general";
  std::string currentSetting = "";

  // read the file one character at a time
  char ch;
  while (aFile >> std::noskipws >> ch) {

    // tabs are simply ignored, whenever they appear
    if (ch == RDM_CFG_PARSER_TAB)
      continue;

    // if we find a end line ..
    if (ch == RDM_CFG_PARSER_LINE_SEP || ch == RDM_CFG_PARSER_LINE_SEP_2) {

      // .. if we were in a comment consider the comment ended
      if (inComment) {
        inComment = false;
        continue;
      }

      // .. if we were parsing a section name, raise an error
      if (inSection) {
        configurationLoadError = RDM_STR_PARSER_INCP_SEC;
        return false;
      }

      // .. if we were parsing a value, store the new value ..
      if (inValue) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
        RDM_STR_CFG_DEBUG,
                     currentSection.c_str(), currentSetting.c_str(),
                     currentContent.c_str());
        setSettingValue(currentSection, currentSetting, currentContent);
        inValue = false;
      }

      // .. and clear the current content.
      currentContent = "";
      continue;

    }

    // if we are in a comment, ignore everything but a comment end (see above)
    if (inComment)
      continue;

    // detect a comment line
    if (ch == RDM_CFG_PARSER_COMMENT) {
      // but fail if we were parsing something else
      if (inSection || inValue || !currentContent.empty()) {
        configurationLoadError = RDM_STR_PARSER_CMT_OOP;
        return false;
      }
      inComment = true;
      continue;
    }

    // detect a section start
    if (ch == RDM_CFG_PARSER_GROUP_START) {
      // but fail if we were parsing something else
      if (inSection || inValue || !currentContent.empty()) {
        configurationLoadError = RDM_STR_PARSER_SEC_OOP;
        return false;
      }
      inSection = true;
      continue;
    }

    // detect a section end and store its value
    if (inSection && ch == RDM_CFG_PARSER_GROUP_END) {
      // sections can't be empty
      if (currentContent.empty()) {
        configurationLoadError = RDM_STR_PARSER_SEC_EMTY;
        return false;
      }
      // section ends can't overlap values
      if (inValue) {
        configurationLoadError = RDM_STR_PARSER_SEC_IVA;
        return false;
      }
      currentSection = currentContent.substr(0, currentContent.length());
      currentContent = "";
      inSection = false;
      continue;
    }

    // detect a section/value separator
    if (ch == RDM_CFG_PARSER_VALUE_SEP) {
      // fail if not after a setting
      if (currentContent.empty()) {
        configurationLoadError = RDM_STR_PARSER_VSP_OOP;
        return false;
      }
      currentSetting = currentContent.substr(0, currentContent.length());
      currentContent = "";
      inValue = true;
      continue;
    }

    // in any other situation, store the character
    currentContent += ch;

  }

  return true;

}

}  // namespace roguedm
