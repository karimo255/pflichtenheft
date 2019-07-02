
/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden im Wesentlichen alle für das Spiel
 * nötigen Berechnungen durchgeführt. Z.B. das Generieren des Spiel-
 * feldes oder der Timer, wie lange ein Spieler für das Lösen
 * ein Spiel benötigt.
*/

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H


#include "../shared/shared.h"


extern int iMarks[9][9][MAX_MARKS];

/* Wird aufgerufen, wenn der Spieler einen Tipp haben möchte. Befüllt
 * die Notiz in der entsprechenden Zelle mit drei Zahlen, von denen
 * eine die richtige im Kontext des Sudokus darstellt.
 * 1. Parameter: x-Koordinate, an der sich der Spieler befindet
 * 2. Parameter: y-Koordinate, an der sich der Spieler befindet
 */
void fillNotesForCell(int iX_coordinate, int iY_coordinate);

/// x-coordinate of the cursor.
extern int iX_coordinate;

/// y-coordinate of the cursor.
extern int iY_coordinate;

/// game difficulty.
extern int iDifficulty;

/// game message
extern char cGameMessage[200];

/// is a session active.
extern int iIsGameActive;

/// current active menu item.
extern int iCurrentPosition;

extern char cUsername[8];
extern int *isUserLoggedIn;
extern int iExitTheGame;
extern int isSolvedAutomatic;

extern int *userId;
/// array which holds the game data.
extern int iGameData[9][9];

/// array which holds deleted cells to keep track of them.
extern int iDeletedCells[9][9];

/// array which holds the coordinates of user cells (cells to solve) to keep track of theme.
extern int iUserCells[9][9];

extern int iAnzahlDerTipps;
extern int iAnzahlDerHilfe;


extern int iErlaubteAnzahlDerTipps;
extern int iErlaubteAnzahlDerHilfe;

void fillNotesForCell(int iX_coordinate, int iY_coordinate);


/** Überpüft, ob sich ein bestimmtes Element in einen bestimmten Array
 * befindet.
 * 1. Parameter: zu überprüfender Array
 * 2. Parameter: Element, nach dem gesucht werden soll
 * 3. Parameter: Länge des Arrays
 * Rückgabewert: Stelle, an der das Element gefunden wurde oder -1, falls
 * es nicht im Array ist
 */
int isElementInArray(int iArray[], int iNumber, int iSize);

/** Setzt einen Array auf den Ursprungszustand zurück, indem alle Werte
 * auf Null gesetzt werden.
 * 1. Parameter: der zurückzusetzende Array
 * 2. Parameter: Länge des Arrays
 */
void resetArray(int iArray[], int iSize);

/** Generiert vor jedem neuen Spiel ein neues gelöstet Spielfeld. Sorgt
 * damit dafür, dass das Sudoku lösbar ist. Der Vorgang wird nach zwei
 * Sekunden neu gestartet, fall die Funktion zu keiner Lösung gekommen
 * ist, um eine Endlossschleife zu vermeiden.
 * 1. Parameter: leeres Spielfeld
 */
void generateGameData(int iGameData[][9]);

/** Löscht je nach Schwierigkeitsgrad mehr oder weniger zufällige Zellen
 * aus dem sichtbaren Spielfeld und speichert die gelöschten Zellen im
 * Array iDeletedCells;
 * 1. Parameter: Spielfeld
 * 2. Parameter: Schwierigkeitsgrad
 */
void deleteCells(int iGameData[][9], int iDifficulty);

/**
 * @param direction of where to navigate
 *
 * Moves the cursor in the desired direction
 */
void navigateTo(int direction);

/** Wird nur aufgerufen, wenn sich der Spieler im "Makiere-Modus" befindet.
 * Es können bis zu drei Zahlen in eine Notiz eingetragen werden. Wenn
 * die Notiz voll ist und eine weitere Zahl eingetragen wird, wird die Notiz
 * gelöscht und die weitere Zahl eingetragen.
 * 1. Parameter: x-Koordinate, an der der Spieler sich befindet
 * 2. Parameter: y-Koordinate, an der der Spieler sich befindet
 * 3. Parameter: Zahl, die der Spiele in die Notiz schreiben möchte
 */
void makeNote(int iX, int iY, int iSuggestion);

/** Dient zur Überprüfung, ob eine bestimmte Zahl in einer bestimmten der
 * neun Unterquadrate des Sudokus vorhanden ist.
 * 1. Parameter: aktuelles Spielfeld
 * 2. Parameter: y-Koordinate der oberen, linken Ecke der Box
 * 3. Parameter: x-Koordinate der obenen, linken Ecke der Box
 * 4. Parameter: Element, nach dem gesucht werden soll
 * Rückgabewert: Gefunden? 1 -> Ja, -1 -> Nein
 */
int isElementInBox(int iGameData[][9], int iBoxStartRow, int iBoxStartCol, int iElement);

/** Generiert eine Zufallszahl zwischen 1 und 9.
 * Rückgabewert: generierte Zufallszahl
 */
int generateRandomNumber();

/** Wird aufgerufen, wenn die keine Zellen mehr ausgefüllt werden müssen.
 * Überprüft, ob der Spieler das Spielfeld richtig gelöst hat.
 * 1. Parameter: aktuelles Spielfeld
 * Rückgabewert: Richtig gelöst (1) oder es existiert mindestens ein Fehler (0)
 */
int solveGame(int iGameData[][9]);

/**
 * @param iGameData[][] which holds the game data.
 *
 * Sets the value of array items to zero.
 */
void resetGameData(int iGameData[][9]);

/** Generiert eine Zufallszahl in einem definierbaren Intervall.
 * 1. Parameter: Intervall-Start
 * 2. Parameter: Intervall-Ende
 * Rückgabewert: generierte Zufallszahl
int generateNumberByInterval(int x, int y);

/**
 * @param array[][] which holds the game data.
 * @return Returns a integer which indicates if the grid if filled complete.
 *
 *  Checks if the grid if filled complete.
 */
int getGridStatus(int array[][9]);

/** Wird aufgerufen, wenn der Spieler die Hilfefunktion nutzt. Aus dem
 * zweidimensionalen Array, in dem die gelöschten Zellen gespeichert sind,
 * wird der Lösungswert in das aktuelle Spielfeld eingetragen.
 * 1. Parameter: aktuelles Spielfeld
 * 2. Parameter: x-Koordinate, an der der Spieler sich befindet
 * 3. Parameter: y-Koordinate, an der der Spieler sich befindet
 */
void solveCell(int iGameData[][9], int iX, int iY);

/** Überprüft, ob alle Felder des sichtbaren Spielfeldes aufgefüllt
 * wurden.
 * 1. Parameter: sichtbares Spielfeld
 * Rückgabewert: Vollständig (FILLED) oder unvollständig (NOT_FILLED)
 */
int getGameStatus(int array[][9]);


void solveAll(int gameData[][9], int deletedCells[][9]);

/** Stoppuhr für das Spiel. Wird zur Messung der benötigten Zeit zum
 * Lösen des Sudokus verwendet. Stoppuhr kann (neu-)gestartet, ge-
 * stopped und pausiert werden.
 * 1. Parameter: Aktion für die Stoppuhr (Zeit abfragen, Zurück-
 * setzen, Starten, Stoppen)
 */
int timer(int iAction);


/** Wandelt eine Zeitangabe in Sekunden in das "00:00"-Format um.
 * 1. Parameter: Zeitangabe in Sekunden
 * 2. Parameter: Char-Array, in den die formatierte Zeit geschrieben
 * werden soll
 */
void timeToString(int iUserTime, char cStringTime[]);

/** Leitet nötige Schritte zur Überprüfung, ob das Sudoku vollständig
 * und richtig gelöst wurde, ein.
 */
int checkGameSolved();

#endif //SUDOKU_GAME_H
