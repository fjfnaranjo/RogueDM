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

#include "Gui.hpp"

#include <memory>
#include <string>

namespace roguedm_gui {

Gui::Gui() {

  initSuccess = false;

  config = roguedm::Config::instance();

  defaultCWidth = 0;
  defaultCHeight = 0;
  maxCols = 0;
  maxRows = 0;

  commandComposer = std::make_unique<CommandComposer>();
  consoleHistory = {};

}

Gui::~Gui() {
  resetGui();
}

bool Gui::initGui(SDL_Renderer *renderer) {

  // Words, command line and history init
  if (!initCharmaps(renderer))
    return false;

  // Clear color
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  SDL_ShowCursor(true);

  initSuccess = true;
  return true;

}

void Gui::resetGui() {
  if (initSuccess) {
    defaultStamper.reset();
    initSuccess = false;
  }
}

void Gui::update(SDL_Renderer *renderer) {

  commandComposer->paintCommandLine(renderer, defaultStamper, maxCols, maxRows,
                                    defaultCWidth, defaultCHeight);

  // TODO(fjfnaranjo): Paint the command history if fits

}

void Gui::consoleHistoryPush(roguedm::Sentence command) {
  consoleHistory.push_back(command);
}

void Gui::resetScreenSize(SDL_Window *window) {
  int ww, wh;
  SDL_GetWindowSize(window, &ww, &wh);
  maxCols = static_cast<int>(floor(ww / defaultCWidth) - 1);
  maxRows = static_cast<int>(floor(wh / defaultCHeight) - 1);
}

bool Gui::initCharmaps(SDL_Renderer *renderer) {

  // Default charmap texture
  std::string defaultCharmap =
  RDM_CFG_CHARMAP_PREFIX
      + config->getSettingValue("general", "charmaps", "default");

  defaultStamper = std::make_unique<CharmapStamper>();
  if (!defaultStamper->loadDefaultCharmap(renderer, defaultCharmap))
    return false;

  defaultCHeight = defaultStamper->getCHeight();
  defaultCWidth = defaultStamper->getCWidth();

  return true;

}

}  // namespace roguedm_gui
