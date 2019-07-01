//
// Created by karim on 24.06.19.
//

#include "../shared/shared.h"

#ifndef SUDOKU_VIEW_H
#define SUDOKU_VIEW_H

#endif //SUDOKU_VIEW_H

void renderMarkModeMessage();

/** Gibt den Dialog, in dem der Spieler nach seinem Namen gefragt wird
 * aus.
 * aus.
 * 1. Parameter: bisheriger Benutzername (Zeichen werden einzeln
 * 1. Parameter: Zeiger auf die Variable mit dem Wert des bisherigern
 * entgegengenommen, um eine nutzerfreundliche Obfläche / Eingabe
 * Benutzernamens (Zeichen werden einzeln entgegengenommen, um eine
 * nutzerfreundliche Obfläche / Eingabe
 * und das Nutzen der Löschen-Taste zu ermöglichen)
 * und das Nutzen der Löschen-Taste zu ermöglichen)
 */
void renderUsernameDialog(char *username);

/** Gibt die Bestenliste (TOP 10) in Abhängigkeit von der Zeit und dem ausgewählten Schwierigkeitsgrad
 * aus.
 * aus.
 * 1. Parameter: Struktur, in der die Bestscores gespeichert sind
 * 1. Parameter: Zeiger auf Struktur, in der die Bestscores gespeichert sind
 * 2. Parameter: ausgewählter Schwierigkeitsgrad
 * 2. Parameter: ausgewählter Schwierigkeitsgrad
 */
void renderMenu();


/** Stellt das Grundgerüst für die Ausgabe der Bestenliste bereit bzw.
 * initialisiert diese Ausgabe
 * initialisiert diese Ausgabe
 * 1. Parameter: Struktur, in der die Bestscores gespeichert sind
 * 1. Parameter: Zeiger auf Struktur, in der die Bestscores gespeichert sind
 * 2. Parameter: ausgewählter Schwierigkeitsgrad
 * 2. Parameter: ausgewählter Schwierigkeitsgrad
 */

void renderDBestScoreDialog();
/**
 * \brief Function to render best-score scrren
 *
 * It shows infos about users and scores
 */
void renderDetails(struct score *scores, int difficulty);

/**
 * \brief Function to render the game grid
 *
 * It shows the game grid
 */
void renderCourt(int gameData[][9],int userCells[][9], int x_coordinate, int y_coordinate);

/** Gibt die Infobox aus, die sich über dem Spielfeld zur Spielzeit befindet
 * und alle wichtigen Informationen, wie die aktuell schon benötigte Zeit,
 * und alle wichtigen Informationen, wie die aktuell schon benötigte Zeit,
 * enthält.
 * enthält.
 * 1. Parameter: Nutzernamen, des aktuell spielenden Spielers
 * 1. Parameter: Zeiger auf die Variable, die den Nutzernamen des aktuell spielenden Spielers enthält
 * 2. Parameter: Highscore im aktuellen Schwierigkeitsgrad
 * 2. Parameter: Zeiger auf die Variable, die den Highscore im aktuellen Schwierigkeitsgrad enthält
 * 3. Parameter: aktuell ausgewählter Schwierigkeitsgrad
 * 3. Parameter: aktuell ausgewählter Schwierigkeitsgrad
 * 4. Parameter: Anzahl der Zellen, die der Spieler noch ausfüllen muss
 * 4. Parameter: Anzahl der Zellen, die der Spieler noch ausfüllen muss
 */
void renderInfoBox(char *username, int *score, int _difficulty, int remaining);

/**
 * \brief Function to render the game menu
 *
 * It shows the game menu
 */
void renderGameMenu();
void checkGameState();

/**
 * \brief Function to render the dialog after solved game
 *
 * It shows the game menu
 */
void renderSolvedGame(int solvedAutomatic, int anzahlDerTipps, int anzahlDerHilfe);

/**
 * \brief Function to render the difficult settings dialog
 *
 * It shows difficult dialog
 */
void renderDifficultyDialog();

/**
 * \brief Function to render the help dialog
 *
 * It shows help dialog
 */
void renderHelpDialog();

/**
 * \brief Function to get the length of integers (aka strlen for strings)
 *
 * @param input an integer to get length of
 * @return the length of the given integer
 *
 * It return the length of integer number (for optimal printing)
 */
int lenHelper(int input);

/**
* \brief Get count of not filled cells
*
* \param array gameData
* \return count of not filled cells
*/

int getRemainingCells(int array[][9]);

void print_list(struct score *head,int difficulty);


void initColors();

void printGameMessage();

void printColoredString(char text[], char color[], int newLine);

/** Dient zur formatierten Ausgabe einer farbigen Zahl im Spielfeld
 * 1. Parameter: Zahl, die gedruckt werden soll
 * 1. Parameter: Zahl, die gedruckt werden soll
 * 2. Parameter: Farbe, in der die Zahl gedruckt werden soll
 * 2. Parameter: Zeiger auf die Variable mit dem Wert für die Farbe,
 * in der die Zahl gedruckt werden soll
 * 3. Parameter: Soll danach eine neue Zeile folgen? (1 = Ja, 0 = Nein)
 * 3. Parameter: Soll danach eine neue Zeile folgen? (1 = Ja, 0 = Nein)
 */
void printColoredNumber(int number, char *color, int newLine);

/** Setzt die Farbe, in der ab sofort in die Konsole geschrieben werden soll
 * 1. Parameter: Farbe
 * 1. Parameter: Zeiger auf die Variable mit dem Wert für die Farbe
 */
void setPrintingColor(char *color);

void printStartOfLine();
void printEndOfLine();
void printTableLine();
void printEndOfTable();
void printEmptyTableLine();

void renderLoadingBar(int x);

void clear_output();


void renderNotesBox(int x,int y);

void renderSetPassword();

void renderEnterPassword();