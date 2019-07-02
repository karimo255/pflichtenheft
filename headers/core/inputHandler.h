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

void handleDifficultyDialogInput(int iUserInput);

void handleMenuInput(int iUserInput);

void handleInGameInput(int iUserInput);

void handleSolvedGameInput(int iUserInput);

void handleSetMarkInput(int iUserInput);

void handleDetailsInput(int iUserInput);

void handleDetailsDialogInput(int iUserInput);

void handleHelpInput(int iUserInput);

void setConfig();
#endif //SUDOKU_INPUTHANDLER_H
