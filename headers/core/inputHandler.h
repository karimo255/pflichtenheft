//
// Created by
//

#ifndef SUDOKU_INPUTHANDLER_H
#define SUDOKU_INPUTHANDLER_H

#include "../../headers/shared/shared.h"

void handleUserNameInput();

void handleSetPasswordInput();

void handleEnterPasswordInput();

void handleDifficultyDialogInput(int userInput);

void handleMenuInput(int userInput);

void handleInGameInput(int userInput);

void handleSolvedGameInput(int userInput);

void handleSetMarkInput(int userInput);

void handleDetailsInput(int userInput);

void handleDetailsDialogInput(int userInput);

void handleHelpInput(int userInput);

void setConfig();
#endif //SUDOKU_INPUTHANDLER_H
