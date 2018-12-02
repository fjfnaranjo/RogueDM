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
 * \file StateMachine.hpp
 * \brief File used to hold the StateMachine class declarations.
 */

#pragma once

#include "StateInterface.hpp"

namespace roguedm {

/**
 * \brief A simple state machine.
 *
 * This class is used to manage the different states of the application.
 *
 * Each state provides specific behavior and features.
 *
 * Simple states execute single tasks and finish. Other states may interact
 * with the user interactively using the CLI interface. A more complex state
 * will define a particular group of classes working as actor/entities inside
 * an main loop controlled by the state itself and provide a game-like rich
 * graphical interface.
 *
 * The class acting as a state must extend the StateInterface interface.
 * The machine just calls the execute method in the state classes and delegate
 * all the control to them. So, each state should implement their own game
 * loop or similar time management mechanism if they are interactive.
 *
 * The state machine deletes the state classes when they finish, don't
 * store/use references to the states outside this class.
 * \see StateInterface
 * \see StateInterface.execute()
 */
class StateMachine
{

  public:

    /**
     * Constructor to start an empty state machine. A new InitialState will
     * be created and used as first state.
     */
    StateMachine();

    /**
     * Constructor to start the state machine with a particular first state to
     * be executed.
     * \param firstState The first state to be executed.
     */
    StateMachine(StateInterfaceReference firstState);

    /**
     * Start the state machine.
     */
    int run();

  protected:

  private:

    /**
     * Specify the next state to be managed (launched when the method
     * start its called the first time or when another state finish their own
     * execute method).
     * \see StateInterface.execute()
     */
    StateInterfaceReference nextState;

    /** Status return code for state machine. */
    StateResponse lastResponse;

};

} // namespace roguedm
