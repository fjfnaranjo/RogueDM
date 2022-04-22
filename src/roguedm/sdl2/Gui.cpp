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

#include "../commands/CommandHandlers.hpp"

namespace roguedm_gui {

Gui::Gui() {

  initSuccess = false;

  config = roguedm::Config::instance();

  defaultCWidth = 0;
  defaultCHeight = 0;
  maxCols = 0;
  maxRows = 0;

  commandComposer = std::make_shared<CommandComposer>();
  roguedm::CommandHandlers::instance()->registerCommandHandler(commandComposer);

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

  paintInterface(renderer, defaultStamper, maxCols, maxRows,
                 defaultCWidth, defaultCHeight);

}

void Gui::resetScreenSize(SDL_Window *window) {
  int ww, wh;
  SDL_GetWindowSize(window, &ww, &wh);
  maxCols = static_cast<int>(floor(ww / defaultCWidth) - 1);
  maxRows = static_cast<int>(floor(wh / defaultCHeight) - 1);
}

void Gui::processText(SDL_Event event) {
  commandComposer->composeText(event);
}

void Gui::processKey(SDL_KeyboardEvent keyEvent) {

  switch (keyEvent.keysym.sym) {

    // Enter
    case SDLK_RETURN:
      processLine();
      break;

      // TAB
    case SDLK_TAB:
      tryAutocompletion();
      break;

    default:
    commandComposer->composeKey(keyEvent);

  }

}

void Gui::tryAutocompletion() {
  auto commandHandlers = roguedm::CommandHandlers::instance()
      ->getCommandHandlers();

  // There is already a command
  if (commandComposer->hasCommand()) {

    // Try complete the current command
    roguedm::Command command = commandComposer->getCommand();
    roguedm::CommandList candidateCommands;
    for (const auto &commandHandler : commandHandlers) {
      roguedm::CommandList posibleCandidates = commandHandler
          ->getCompletionCandidates(command);
      for (const auto &possibleCandidate : posibleCandidates)
        candidateCommands.insert(candidateCommands.end(), possibleCandidate);
    }
    // TODO(fjfnaranjo): Do more stuff with this list
    if (1 == candidateCommands.size())
      commandComposer->setCommand(candidateCommands[0]);

    // There is not a command
  } else {

    // Try to identify a command from the sentence
    roguedm::Sentence sentence = commandComposer->getRawSentence();
    roguedm::Command candidateCommand;
    for (const auto &commandHandler : commandHandlers)
      if (
      RDM_CMD_IDENTIFY_DONE
          == commandHandler->identifyCommand(sentence, candidateCommand))
        commandComposer->setCommand(candidateCommand);

  }

}

void Gui::processLine() {
  auto commandHandlers = roguedm::CommandHandlers::instance()
      ->getCommandHandlers();

  roguedm::Command currentCommand = commandComposer->getCommand();

  // TODO(fjfnaranjo): The loop can be moved to CommandHandlers

  // Process action
  for (const auto &commandHandler : commandHandlers)
    if (
    RDM_CMD_PROCESS_DONE == commandHandler->processCommand(currentCommand)) {

      // Push the command in the historic
      commandComposer->commandHistoryPush(currentCommand);

      // Reset the history pointer
      commandComposer->resetCommand();
    }

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

void Gui::paintInterface(SDL_Renderer *renderer,
                                      std::shared_ptr<CharmapStamper> stamper,
                                      int maxCols, int maxRows,
                                      int defaultCWidth, int defaultCHeight) {

  SDL_Rect dialogMain;
  SDL_Rect dialogCell;
  SDL_Rect dialogCreature;
  SDL_Rect dialogText;

  // Panels
  // TODO(fjfnaranjo): Move box drawing to other class or a helper function
  stamper->drawBox(0, 0, maxCols, maxRows, renderer);
  dialogMain.x = 1;
  dialogMain.y = 1;
  dialogMain.w = static_cast<int>(maxCols - (maxCols / 4));
  dialogMain.h = static_cast<int>(maxRows - (maxRows / 3));
  dialogText.x = 1;
  dialogText.y = dialogMain.h + 3;
  dialogText.w = dialogMain.w;
  dialogText.h = maxRows - dialogMain.h - 4;
  stamper->drawVSeparator(dialogText.w + 2, 0, maxRows, renderer);
  stamper->drawHSeparator(0, dialogText.y - 1, dialogText.w + 2, renderer);
  dialogCell.x = dialogText.w + 3;
  dialogCell.y = 1;
  dialogCell.w = maxCols - dialogMain.w - 4;
  dialogCell.h = static_cast<int>(maxRows - ((maxRows / 8) * 2));
  stamper->drawHSeparator(dialogCell.x - 1, dialogCell.y + dialogCell.h + 1,
                          dialogCell.w + 2, renderer);
  dialogCreature.x = dialogCell.x;
  dialogCreature.y = dialogCell.y + dialogCell.h + 2;
  dialogCreature.w = dialogCell.w;
  dialogCreature.h = dialogCell.h * 2;
  stamper->drawHSeparator(dialogCreature.x - 1,
                          dialogCreature.y + dialogCreature.h + 1,
                          dialogCreature.w + 2, renderer);

  commandComposer->paintCommandLine(renderer, defaultStamper, maxCols, maxRows,
                                    defaultCWidth, defaultCHeight);

  // TODO(fjfnaranjo): Paint the command history if fits

}

}  // namespace roguedm_gui
