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
 * \file macros.hpp
 * \brief File used for common define macros.
 */

#pragma once

#include <memory>

// Interface declaration macro.
//
// By:  paercebal - Raoul Borges - http://www.linkedin.com/in/raoulborges -
//      https://softwareengineering.stackexchange.com/users/26547/paercebal
// See: https://softwareengineering.stackexchange.com/questions/235674/what _
//      -is-the-pattern-for-a-safe-interface-in-c

/**
 * \brief Class-as-interface macro.
 *
 * This is a macro used to protect some methods in classes intended as
 * interfaces.
 */
#define RDM_DECLARE_CLASS_AS_INTERFACE(ClassName)                     \
  public:                                                             \
    virtual ~ClassName() =default;                                    \
  protected:                                                          \
    ClassName() =default;                                             \
    ClassName(const ClassName & ) =delete;                            \
    ClassName & operator = (const ClassName & ) =delete;              \
  private:

// Inspired by the previous one...

/**
 * \brief Class-as-singleton macro for declaration.
 *
 * This is a macro used to protect some methods in classes intended as
 * singletons.
 */
#define RDM_DECLARE_CLASS_AS_SINGLETON(ClassName)                     \
  public:                                                             \
    static std::shared_ptr<ClassName> instance() {                    \
      static std::shared_ptr<ClassName> instance =                    \
        std::make_shared<ClassName>();                                \
      return instance;                                                \
    }                                                                 \
    virtual ~ClassName() =default;                                    \
    ClassName(const ClassName&) =delete;                              \
    ClassName& operator=(const ClassName&) =delete;                   \
  private:

/**
 * \brief Class-as-nocopy-nomove macro for declaration.
 *
 * This is a macro used to protect some methods in classes intended as
 * non-copyable non-moveable.
 */
#define RDM_DECLARE_CLASS_AS_NOCPNOMV(ClassName)                      \
  public:                                                             \
    ClassName(const ClassName&) =delete;                              \
    ClassName& operator=(const ClassName&) =delete;                   \
  private:
