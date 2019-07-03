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

/** Wird aufgerufen, wenn der Nutzer sich im Dialog befindet, in dem er
 * seinen Namen angeben soll. Ermöglicht zudem das Überspringen dieses
 * Schrittes. Der Spieler ist dann unter "anonym" angemeldet.
 * Es werden einzelne Buchstaben eingelesen, um den Dialog dynamisch zu
 * gestalten und das nutzen der Löschen-Taste zu ermöglichen, um einzelne
 * Buchstaben löschen zu können. So wird ein möglichst nutzerfreundliches
 * Erlebnis garantiert.
 */
void handleUserNameInput();

/** Wird aufgerufen, wenn sich ein neuer Nutzer anmeldet und ein
 * Passwort setzen möchte. Dieses wird nach einer Bestätigung durch
 * das Drücken der Entertaste mit dem Nutzernamen und der UserID in
 * der Datenbank hinterlegt (bzw. die Funktion, die dies erledigt wird
 * aufgerufen).
 * Es werden einzelne Buchstaben eingelesen, um den Dialog dynamisch zu
 * gestalten und das nutzen der Löschen-Taste zu ermöglichen, um einzelne
 * Buchstaben löschen zu können. So wird ein möglichst nutzerfreundliches
 * Erlebnis garantiert.
 */
void handleSetPasswordInput();

/** Wird aufgerufen, wenn sich ein bereits registrierter Nutzer anmelden
 * möchte. Dieser kann nun sein Passwort eingeben. Das eingegebene Passwort
 * wird dem in der Datenbank hinterlegtem Passwort verglichen und auf
 * Übereinstimmung geprüft.
 * Es werden einzelne Buchstaben eingelesen, um den Dialog dynamisch zu
 * gestalten und das nutzen der Löschen-Taste zu ermöglichen, um einzelne
 * Buchstaben löschen zu können. So wird ein möglichst nutzerfreundliches
 * Erlebnis garantiert.
 */
void handleEnterPasswordInput();


/** Wird aufgerufen, wenn sich ein bereits registrierter Nutzer anmelden
 * möchte. Dieser kann nun sein Passwort eingeben. Das eingegebene Passwort
 * wird dem in der Datenbank hinterlegtem Passwort verglichen und auf
 * Übereinstimmung geprüft.
 * Es werden einzelne Buchstaben eingelesen, um den Dialog dynamisch zu
 * gestalten und das nutzen der Löschen-Taste zu ermöglichen, um einzelne
 * Buchstaben löschen zu können. So wird ein möglichst nutzerfreundliches
 * Erlebnis garantiert.
 */
void handleDifficultyDialogInput(int iUserInput);

/** Wird aufgerufen, wenn der Spieler eine Eingabe im Hauptmenü tätigt.
 * Verarbeiet die Eingabe des Nutzers und ruft die nötigen Funtkionen
 * auf, um den Nutzer zu den nächsten "Screens" weiterzuleiten.
 * 1. Parameter: Eingabe des Nutzers
 */
void handleMenuInput(int iUserInput);

/** Wird aufgerufen, wenn der Nutzer eine Eingabe tätigt, wenn er sich gerade
 * im laufenden Spiel befindet. Ruft nötige Funktionen auf, um die Eingabe
 * des Nutzers der Legende entsprechend zu verarbeiten.
 * 1. Parameter: Eingabe des Nutzers
 */
void handleInGameInput(int iUserInput);


/** Wird aufgerufen, wenn der Nutzer eine Eingabe im "Winscreen" tätigt.
 * Eingabe wird sachgerecht behandelt.
 * 1. Parameter: Eingabe des Nutzers
 */
void handleSolvedGameInput(int iUserInput);

void handleSetMarkInput(int iUserInput);

/** Wird aufgerufen, sobald der Nutzer versucht etwas einzugeben, während
 * er sich in der Bestenliste befindet. "z" bringt den Spieler zurück zum
 * Menü.
 * 1. Parameter: Eingabe des Nutzers
 */
void handleDetailsInput(int iUserInput);

/** Wenn der Spieler eine Eingabe im Dialog, in dem er den Schwierigkeitsgrad
 * für die Bestenliste auswählen soll, tätigt, wird diese Funktion aufgerufen
 * und seine Eingabe wird verarbeitet.
 * 1. Parameter: Eingabe des Nutzers
 */
void handleDetailsDialogInput(int iUserInput);

/** Wird aufgerufen, sobald der Nutzer versucht etwas einzugeben, während
 * er sich in den Spielregeln befindet. "z" bringt den Spieler zurück zum
 * Menü oder zum laufenden Spiel; ausgehend davon, ob es noch ein ungelöstes
 * Sudokufeld gibt.
 * 1. Parameter: Eingabe des Nutzers
 */
void handleHelpInput(int iUserInput);

/** Wird aufgerufen, sobald der Nutzer versucht etwas einzugeben, während
 * er sich in den Spielregeln befindet. "z" bringt den Spieler zurück zum
 * Menü oder zum laufenden Spiel; ausgehend davon, ob es noch ein ungelöstes
 * Sudokufeld gibt.
 * 1. Parameter: Eingabe des Nutzers
 */

/** Setzt bzw. initialisiert die erlaubte Anzahl, wie oft der Spieler
 * die Tipp- und Hilfefunktion nutzen darf. Setzt ausßerdem die bereits
 * genutzten Funktionen zurück.
 */
void setConfig();
#endif //SUDOKU_INPUTHANDLER_H
