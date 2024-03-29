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

#include <memory>

#include <SDL.h>

#include "CharmapStamper.hpp"
#include "CommandComposer.hpp"
#include "../macros.hpp"

namespace roguedm_gui {

/** \brief Main GUI class. */
class Gui {

RDM_DECLARE_CLASS_AS_NOCPNOMV(Gui)

 public:

  Gui();
  ~Gui();

  /** Init state and SDL2 resources. */
  bool initGui(SDL_Renderer*);

  /** Clear the state and free SDL2 resources. */
  void resetGui();

  /** Update the state each game tick and interact with SDL2. */
  void update(SDL_Renderer*);

  /** Updates the screen if window resizes. */
  void resetScreenSize(SDL_Window*);

  /** Manage a text composition from the SDL events manager */
  void processText(SDL_Event);

  /** Manage a key sent from the SDL events manager */
  void processKey(SDL_KeyboardEvent);

 private:

  /** Stores if SDL resources have been initialized successfully. */
  bool initSuccess;

  /** Define the initial word type table. */
  bool initCharmaps(SDL_Renderer*);

  /** App configuration singleton reference. */
  roguedm::ConfigSharedPtr config;

  /** Character width of the default charmap. */
  int defaultCWidth;

  /** Character height of the default charmap. */
  int defaultCHeight;

  /** Screen size in columns. */
  int maxCols;

  /** Screen size in rows. */
  int maxRows;

  /** Shared pointer to the default charmap drawing class. */
  std::shared_ptr<CharmapStamper> defaultStamper;

  /** Paints the interface. */
  void paintInterface(SDL_Renderer*, std::shared_ptr<CharmapStamper>, int,
                        int, int, int);

  /** The command composer for this GUI. */
  std::shared_ptr<CommandComposer> commandComposer;

  /** Process the command line. */
  void processLine();

  /** Try to find a valid autocompletion for the current command. */
  void tryAutocompletion();

};

}  // namespace roguedm_gui
