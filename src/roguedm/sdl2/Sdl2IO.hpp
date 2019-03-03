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

#include <map>
#include <memory>

#include <SDL.h>

#include "Gui.hpp"
#include "../Config.hpp"
#include "../GameComponentInterface.hpp"
#include "../commands/CommandHandlerInterface.hpp"

namespace roguedm_gui {

/**
 * \brief Manages the fundamental SDL2 state and interactions between the GUI
 *        and other IO classes.
 *
 * Manage the SDL interface with the player. The console graphical output and
 * the texts input. Also handles the vast majority of the console command line
 * features.
 */
class Sdl2IO :
    roguedm::CommandHandlerInterface,
    roguedm::GameComponentInterface
{

  RDM_DECLARE_CLASS_AS_NOCPNOMV(Sdl2IO)

  public:

    Sdl2IO();
    ~Sdl2IO();

    /** Init state and SDL2 resources. */
    bool initSdl2IO();

    /** Clear the state and free SDL2 resources. */
    void resetSdl2IO();

    /** Update the state each game tick and interact with SDL2. */
    void update() override;

    /** Request to process a command. */
    bool processCommand(const roguedm::Command&) override;

    /** Request to identify a command in a sentence. */
    bool identifyCommand(const roguedm::Sentence&, roguedm::Command&)
      const override;

    /** Request a list of autocomplete options for a command. */
    roguedm::CommandList getCompletionCandidates(const roguedm::Command&)
      const override;

    /**
     * Add a new command handler to the internal list, after erasing any
     * previous occurrence of that handler.
     * \param c The command handler.
     */
    void registerCommandHandler(CommandHandlerInterface *c);

    /**
     * Remove all references to the command handler from the internal list.
     * \param c The command handler.
     */
    void unregisterCommandHandler(CommandHandlerInterface *c);

    /**
     * Tell the outside that the user wants to quit the app.
     * \return 1 if user want to quit, 0 if not
     */
    int mustHalt();

  private:

    /** Stores if SDL resources have been initialized successfully. */
    bool initSuccess;

    /**
     * Method internally called to manage the SDL event queue.
     */
    void eventsManager();

    /** Manage a text composition from the SDL events manager */
    void processText(SDL_Event*);

    /** Manage a key sent from the SDL events manager */
    void processKey(SDL_Event*);

    /** Try to find a valid autocompletion for the current command. */
    void tryAutocompletion();

    /** Process the command line. */
    void processLine();

    /** App configuration singleton reference. */
    roguedm::ConfigSharedPtr config;

    /** True when user wants to quit. */
    int appDone;

    /** Keep the SDL_GetTicks return value to implement a FPS lock. */
    int ticks;

    /** The command handlers list. */
    std::vector<CommandHandlerInterface*> commandHandlers;

    /** SDL window */
    SDL_Window *window;

    /** SDL main rendering surface */
    SDL_Renderer *renderer;

    /** Unique pointer to the default GUI drawing class. */
    std::unique_ptr<Gui> gui;

};

} // namespace roguedm_gui
