/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden im Wesentlichen alle für das Spiel
 * nötigen Berechnungen durchgeführt. Z.B. das Generieren des Spiel-
 * feldes oder der Timer, wie lange ein Spieler für das Lösen
 * ein Spiel benötigt.
*/

#include "../shared/shared.h"

#ifndef SUDOKU_VIEW_H
#define SUDOKU_VIEW_H

#endif //SUDOKU_VIEW_H

/** Ausgabe des Hinweises auf den "Markieren-Modus"
 */
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
void renderUsernameDialog(char *pcUsername);

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

/** Stellt das Grundgerüst für die Ausgabe der Bestenliste bereit bzw.
 * initialisiert diese Ausgabe
 * 1. Parameter: Struktur, in der die Bestscores gespeichert sind
 * 2. Parameter: ausgewählter Schwierigkeitsgrad
 */
void renderDetails(struct sScore *scores, int difficulty);

/** Gibt das "sichtbare" Spielfeld in der Konsole aus.
 * 1. Parameter: sichtbares Spielfeld
 * 2. Parameter: Array, in dem gespeichert ist, welche Zellen vom Nutzer befüllt werden
 * müssen
 * 3. Parameter: x-Koordinate, an der sich der Cursor des Spielers befindet
 * 4. Parameter: y-Koordinate, an der sich der Cursor des Spielers befindet
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
void renderInfoBox(char *username, int *score, int iCurrentUserBestScore, int _difficulty, int remaining);

/** Gibt die Legende aus, die dem Spieler aufzeigt, welche Tasten er drücken kann, um
 * mit dem Spiel zu interagieren.
 */
void renderGameMenu();

/** Gibt den "Winscreen" in der Konsole aus, wenn das Sudoku vollständig gelöst wurde.
 * 1. Parameter: Wurde das Spiel aufgelöst oder hat der Spieler es selbst gelöst
 * (1 = es wurde aufglöst, 0 = Spieler hat es gelöst)
 * 2. Parameter: Anzahl der benutzten Tipps
 * 3. Parameter: Anzahl der benutzten Zelllösugen
 */
void renderSolvedGame(int solvedAutomatic, int anzahlDerTipps, int anzahlDerHilfe);

/** Ausgabe des Dialoges, in dem der Spieler den Schwierigkeitsgrad
 * für sein Spiel wählen kann.
 */
void renderDifficultyDialog();

/** Gibt die Spielregeln in der Konsole aus.
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

/** Ermittelt die Anzahl der Zellen, die noch befüllt werden müssen.
 * 1. Parameter: sichtbares Spielfeld
 */
int getRemainingCells(int iArray[][9]);

/** Gibt die Bestenliste (TOP 10) in Abhängigkeit von der Zeit und dem ausgewählten Schwierigkeitsgrad
* aus.
* 1. Parameter: Zeiger auf Struktur, in der die Bestscores gespeichert sind
* 2. Parameter: ausgewählter Schwierigkeitsgrad
*/
void print_list(struct sScore *head, int iDifficulty);


void initColors();

/** Gibt die kontextbezogene Nachricht an den Spieler aus (z.B. "Passwort falsch")
 */
void printGameMessage();

/** Dient zur formatierten Ausgabe eines vorgegeben Strings
 * 1. Parameter: String, der gedruckt werden soll
 * 2. Parameter: Farbe, in der der String gedruckt werden soll
 * 3. Parameter: Soll danach eine neue Zeile folgen? (1 = Ja, 0 = Nein)
 */
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

/** Ermittelt die Anzahl der Zellen, die noch befüllt werden müssen.
 * 1. Parameter: sichtbares Spielfeld
 */
void printStartOfLine();

/** Ausgabe des Endes einer Zeile, die zu einer Box mit Informationen
 * gehört (Beispiel: Spielregeln).
 */
void printEndOfLine();

/** Ausgabe einer Zeile, die zu einer Box mit Informationen gehört
 * (Beispiel: Spielregeln).
 * 1. Parameter: Text, der gedruckt werden sollen
 */
void printTableLine();

/** Ausgabe der Zeile, die das Ende einer Box mit Informationen darstellt
 * (Beispiel: Spielregeln).
 */
void printEndOfTable();
void printEmptyTableLine();

void renderLoadingBar(int x);

/** Löscht den aktuellen Konsoleninhalt. Überprüft zunächst das
 * laufende Betriebssystem, um einen ordnungsgemäßen Ablauf zu
 * gewährleisten.
 */
void clear_output();

/** Augabe - sofern vorhanden - der Notizen des Spielers (bzw. Tipps).*/
void renderNotesBox(int x,int y);

/** Gibt den Dialog aus, in dem ein neuer Spieler sein Passwort
 * setzen kann.
 */
void renderSetPassword();

/** Gibt den Dialog aus, in dem ein bereits registrierter Spieler
 * sein Passwort eingeben kann.
 */
void renderEnterPassword();