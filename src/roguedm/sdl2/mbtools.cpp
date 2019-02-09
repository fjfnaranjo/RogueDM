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

#include "mbtools.hpp"

namespace roguedm_gui {

std::size_t multibyteLenght(const std::string &string) {
  std::size_t characterCount = 0;
  int currentShift = 0;
  int bytesReaded = 0;
  do {
    currentShift += bytesReaded;
    bytesReaded = mbrtowc(NULL, &string[currentShift], MB_CUR_MAX, NULL);
    if(bytesReaded>0) characterCount++;
  } while (bytesReaded>0);
  return (bytesReaded<0) ? bytesReaded : characterCount;
}

std::string multibyteCharacterByIndex(const std::string &string, const std::size_t position) {
  std::string lastCharacterString(MB_CUR_MAX, '\0');
  wchar_t lastCharacter;
  std::size_t characterCount = 0;
  std::size_t characterSize = 0;
  int currentShift = 0;
  int bytesReaded = 0;
  do {
    currentShift += bytesReaded;
    bytesReaded = mbrtowc(&lastCharacter, &string[currentShift], MB_CUR_MAX, NULL);
    if(bytesReaded>0) characterCount++;
    if((characterCount-1)==position) {
      characterSize = wcrtomb(&lastCharacterString[0], lastCharacter, NULL);
      lastCharacterString.resize(characterSize);
      return lastCharacterString;
    }
  } while (bytesReaded>0);
  return u8"";
}

std::string multibyteSubstr(const std::string &string, const std::size_t start, const std::size_t size) {
  if(size==0)
    return u8"";
  std::string newString;
  int startByte = 0;
  std::size_t characterCount = 0;
  int currentShift = 0;
  int bytesReaded = 0;
  do {
    currentShift += bytesReaded;
    bytesReaded = mbrtowc(NULL, &string[currentShift], MB_CUR_MAX, NULL);
    if(bytesReaded>0) characterCount++;
    if((characterCount-1)==start && bytesReaded>0)
      startByte = currentShift;
    if((characterCount-1)==start+size)
      return string.substr(startByte, currentShift-startByte);
    if(bytesReaded==0 && startByte>0)
      return string.substr(startByte, currentShift-startByte);
    if(bytesReaded==0)
      return u8"";
  } while (bytesReaded>0);
  return u8"";
}

} // namespace roguedm_gui
