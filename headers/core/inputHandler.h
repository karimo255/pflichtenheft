/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden im Wesentlichen alle für das Spiel
 * nötigen Berechnungen durchgeführt. Z.B. das Generieren des Spiel-
 * feldes oder der Timer, wie lange ein Spieler für das Lösen
 * ein Spiel benötigt.
*/

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
