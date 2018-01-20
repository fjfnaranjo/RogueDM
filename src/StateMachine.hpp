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

#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include "StateInterface.hpp"
#include "InitialState.hpp"

namespace roguedm {

/**
 * \brief State machine class.
 *
 * This class is used to manage the different states for the game. A state, is
 * a moment of the execution with a determined group of classes working inside
 * the game loop.
 *
 * The class acting as a state, must extends the StateInterface interface.
 * Also, because this class only calls the execute method in the states classes,
 * any class should implement their own game loop or time management mechanism.
 *
 * The machine deletes the state classes, don't store/use references outside
 * this class.
 *
 * \see StateInterface
 * \see StateInterface.execute()
 */
class StateMachine
{

  public:

    /**
     * Sets the first state to be executed.
     *
     * \param firstState This parameter should be use to set the first
     *                   state to be executed.
     */
    StateMachine(StateInterface* firstState);

    /**
     * Default destructor (project guidelines requires always a destructor,
     * even if it will be empty).
     */
    virtual ~StateMachine();

    /**
     * \brief Method used by the main application class to start the state
     *        machine.
     *
     * As stated before, the machine deletes the state classes, don't store/use
     * references outside this class.
     */
    void start();

    /**
     * Get the state machine application exit status code.
     * \return Status code to be returned with cstdlib exit().
     */
    int getStatus();

  protected:

  private:

    /** Copy operator (private because is disabled by default). */
    StateMachine(const StateMachine&);

    /** Assing operator (private because is disabled by default). */
    void operator=(const StateMachine&);

    /** Field to store the current machine state. */
    StateInterface* currentState;

    /**
     * Specify the next state to be managed (launched when the method
     * start its called the first time or when another state finish their own
     * execute method).
     * \see StateMachine.start()
     * \see StateInterface.execute()
     */
    StateInterface* nextState;

    /** Status return code for state machine */
    int status;

};

} // namespace roguedm

#endif // STATEMACHINE_HPP
