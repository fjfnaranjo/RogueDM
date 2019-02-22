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

/**
 * \brief Value to report that no more stages should be executed.
 */
#define RDM_STAGE_EXIT nullptr

namespace roguedm {

// This is a forward declaration for usage in the StageInterfaceReference
// declaration.
class StageInterface;

/**
 * \brief Shared pointer to a stage.
 */
typedef std::shared_ptr<StageInterface> StageInterfaceReference;

/**
 * \brief Response for stages implementing the StageInterface.
 */
struct StageResponse {
  int status;
  StageInterfaceReference nextStage;
};

/**
 * \interface StageInterface
 * \brief Interface for the different stages used in the application.
 *
 * Every class must extends this interface if it wants to work as a stage for
 * the application.
 */
class StageInterface
{

  RDM_DECLARE_CLASS_AS_INTERFACE(StageInterface)

  public:

    /** The stage main code. */
    virtual StageResponse execute() =0;

};

} // namespace roguedm
