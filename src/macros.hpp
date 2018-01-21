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

#ifndef MACROS_HPP
#define MACROS_HPP

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
    static ClassName* instance(const int instanceMode) {              \
      static ClassName *instance;                                     \
      if(instance) {                                                  \
        switch(instanceMode) {                                        \
          case SINGLETON_RESET:                                       \
            delete instance;                                          \
            instance = new ClassName();                               \
            break;                                                    \
          case SINGLETON_DELETE:                                      \
            delete instance;                                          \
          case SINGLETON_CREATE:                                      \
            break;                                                    \
          default:                                                    \
            return 0;                                                 \
            break;                                                    \
        }                                                             \
      } else {                                                        \
        switch(instanceMode) {                                        \
          case SINGLETON_CREATE:                                      \
            instance = new ClassName();                               \
            break;                                                    \
          case SINGLETON_DELETE:                                      \
          case SINGLETON_RESET:                                       \
          default:                                                    \
            return 0;                                                 \
            break;                                                    \
        }                                                             \
      }                                                               \
      return instance;                                                \
    }                                                                 \
    static const int SINGLETON_CREATE = 1;                            \
    static const int SINGLETON_RESET = 2;                             \
    static const int SINGLETON_DELETE = 3;                            \
  private:                                                            \
    ClassName();                                                      \
    ~ClassName();                                                     \
    ClassName(const ClassName & ) =delete;                            \
    ClassName & operator = (const ClassName & ) =delete;

#endif // MACROS_HPP
