/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden im Wesentlichen alle für das Spiel
 * nötigen Berechnungen durchgeführt. Z.B. das Generieren des Spiel-
 * feldes oder der Timer, wie lange ein Spieler für das Lösen
 * ein Spiel benötigt.
*/

#include <stdlib.h>
#include "../../headers/core/view.h"
#include "../../headers/core/game.h"
#include "../../headers/shared/shared.h"
#include "../../headers/services/score_service.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <unistd.h>

int iX_coordinate = 0; ///< Cursor X Koordinate
int iY_coordinate = 0; ///< Cursor Y Koordinate
int iDifficulty = EASY; ///< Schwierigkeitsstufe
char cGameMessage[200] = {0}; ///< Hier werden Spiel Mitteilungen gespeichert
int iIsGameActive; ///< Beschreibt ob das Spiel aktiv ist.

int iGameData[9][9] = {0}; ///< Hier werden die spieldaten gespeichert
int iMarks[9][9][MAX_MARKS]; ///< Hier werden user User Notizen gespeichert
int iDeletedCells[9][9] = {0}; ///< Hier werden die gelöschten Zellen gespeichert
int iUserCells[9][9] = {0}; ///< Hier werden die Zellen, die vom User gefüllt sind gespeichert.
int *piUserID;  ///< Pointer zeigt auf die id des gerade angemelteden User.

int iElementsInSomeColumn[9] = {0}; ///< Repräsentiert die gesamte Spalten des Y Koordinate des Cursor.

time_t start, end, _pause;

void fillNotesForCell(int iX_coordinate, int iY_coordinate)
/* Wird aufgerufen, wenn der Spieler einen Tipp haben möchte. Befüllt
 * die Notiz in der entsprechenden Zelle mit drei Zahlen, von denen
 * eine die richtige im Kontext des Sudokus darstellt.
 * 1. Parameter: x-Koordinate, an der sich der Spieler befindet
 * 2. Parameter: y-Koordinate, an der sich der Spieler befindet
 */
{
    int iRandomIndexForSolution = rand() % 2;

    iMarks[iX_coordinate][iY_coordinate][iRandomIndexForSolution] = iDeletedCells[iX_coordinate][iY_coordinate];

    for (int i = 0; i < MAX_MARKS; i++)
    {
        if (iMarks[iX_coordinate][iY_coordinate][i] == 0)
        {
            int iNumber = generateRandomNumber();
            if (isElementInArray(iMarks[iX_coordinate][iY_coordinate], iNumber, MAX_MARKS) > 0)
            {
                i--;
                continue;
            }
            iMarks[iX_coordinate][iY_coordinate][i] = iNumber;
        }
    }
}

void solveCell(int iGameData[][9], int iX, int iY)
/* Wird aufgerufen, wenn der Spieler die Hilfefunktion nutzt. Aus dem
 * zweidimensionalen Array, in dem die gelöschten Zellen gespeichert sind,
 * wird der Lösungswert in das aktuelle Spielfeld eingetragen.
 * 1. Parameter: aktuelles Spielfeld
 * 2. Parameter: x-Koordinate, an der der Spieler sich befindet
 * 3. Parameter: y-Koordinate, an der der Spieler sich befindet
 */
{
    if (iDeletedCells[iX][iY] > 0)
    {
        iUserCells[iX][iY] = 0;
        iGameData[iX][iY] = iDeletedCells[iX][iY];
    }
}

void makeNote(int iX, int iY, int iSuggestion)
/* Wird nur aufgerufen, wenn sich der Spieler im "Makiere-Modus" befindet.
 * Es können bis zu drei Zahlen in eine Notiz eingetragen werden. Wenn
 * die Notiz voll ist und eine weitere Zahl eingetragen wird, wird die Notiz
 * gelöscht und die weitere Zahl eingetragen.
 * 1. Parameter: x-Koordinate, an der der Spieler sich befindet
 * 2. Parameter: y-Koordinate, an der der Spieler sich befindet
 * 3. Parameter: Zahl, die der Spiele in die Notiz schreiben möchte
 */
{
    for (int i = 0; i <= MAX_MARKS; i++)
    {
        if (iMarks[iX][iY][i] == 0)
        {
            iMarks[iX][iY][i] = iSuggestion;
            break;
        }
        else if (i == MAX_MARKS)
        {
            iMarks[iX][iY][0] = iSuggestion;
            for (int j = 1; j < MAX_MARKS; j++)
            {
                iMarks[iX][iY][j] = 0;
            }
        }
    }
}

void solveAll(int iGameData[][9], int iDeletedCells[][9])
/* Wird aufgerufen, wenn der Spieler aufgibt und das Sudoku auflösen lassen
 * möchte. Alle Felder, die durch den Nutzer hätten gefüllt werden sollen,
 * werden von dem Array der gelöschten Zellen in das Spielfeld über-
 * tragen.
 * 1. Parameter: aktuelles Spielfeld
 * 2. Parameter: Array mit den gelöschten Zellen (Lösung)
 */
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (iDeletedCells[x][y] != 0)
            {
                iGameData[x][y] = iDeletedCells[x][y];
            }
        }
    }
}

int solveGame(int iGameData[][9])
/* Wird aufgerufen, wenn die keine Zellen mehr ausgefüllt werden müssen.
 * Überprüft, ob der Spieler das Spielfeld richtig gelöst hat.
 * 1. Parameter: aktuelles Spielfeld
 * Rückgabewert: Richtig gelöst (1) oder es existiert mindestens ein Fehler (0)
 */
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            int iNumber = iGameData[x][y];

            iGameData[x][y] = 0;

            if (isElementInArray(iGameData[x], iNumber, 9) >= 0 || iNumber == 0)
            {
                iGameData[x][y] = iNumber;
                return 0;
            }

            resetArray(iElementsInSomeColumn, 9);
            for (int l = 0; l < sizeof(iElementsInSomeColumn); l++)
            {
                iElementsInSomeColumn[l] = iGameData[l][y];
            }

            if (isElementInArray(iElementsInSomeColumn, iNumber, 9) >= 0 ||
                isElementInBox(iGameData, x - x % 3, y - y % 3, iNumber) >= 0)
            {
                iGameData[x][y] = iNumber;
                return 0;
            }
            iGameData[x][y] = iNumber;
        }
    }

    return 1;
}

int generateRandomNumber()
/* Generiert eine Zufallszahl zwischen 1 und 9.
 * Rückgabewert: generierte Zufallszahl
 */
{
    return 1 + rand() % 9;
}

void resetArray(int iArray[], int iSize)
/* Setzt einen Array auf den Ursprungszustand zurück, indem alle Werte
 * auf Null gesetzt werden.
 * 1. Parameter: der zurückzusetzende Array
 * 2. Parameter: Länge des Arrays
 */
{
    for (int l = 0; l < 9; l++)
    {
        iArray[l] = 0;
    }
}

void resetGameData(int iGmaeData[][9])
/* Setzt das sichtbare Spielfeld in seinen Ursprungszustand zurück, indem
 * alle Werte auf Null gesetzt werden.
 * 1. Parameter: aktuelles Spielfeld
 */
{
    for (int _x = 0; _x < 9; _x++)
    {
        for (int _y = 0; _y < 9; _y++)
        {
            iGmaeData[_x][_y] = 0;
        }
    }
}

int isElementInArray(int iArray[], int iNumber, int iSize)
/* Überpüft, ob sich ein bestimmtes Element in einen bestimmten Array
 * befindet.
 * 1. Parameter: zu überprüfender Array
 * 2. Parameter: Element, nach dem gesucht werden soll
 * 3. Parameter: Länge des Arrays
 * Rückgabewert: Stelle, an der das Element gefunden wurde oder -1, falls
 * es nicht im Array ist
 */
{
    for (int x = 0; x < iSize; x++)
    {
        if (iArray[x] == iNumber && iNumber != 0)
        {
            return x;
        }
    }

    return -1;
}

int isElementInBox(int iArr[][9], int iBox_start_row, int iBox_start_col, int iEle)
/* Dient zur Überprüfung, ob eine bestimmte Zahl in einer bestimmten der
 * neun Unterquadrate des Sudokus vorhanden ist.
 * 1. Parameter: aktuelles Spielfeld
 * 2. Parameter: y-Koordinate der oberen, linken Ecke der Box
 * 3. Parameter: x-Koordinate der obenen, linken Ecke der Box
 * 4. Parameter: Element, nach dem gesucht werden soll
 * Rückgabewert: Gefunden? 1 -> Ja, -1 -> Nein
 */
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
        {

            if (iArr[row + iBox_start_row][col + iBox_start_col] == iEle && iEle != 0)
            {
                return 1;
            }
        }
    return -1;
}

void generateGameData(int iGameData[][9])
/* Generiert vor jedem neuen Spiel ein neues gelöstet Spielfeld. Sorgt
 * damit dafür, dass das Sudoku lösbar ist. Der Vorgang wird nach zwei
 * Sekunden neu gestartet, fall die Funktion zu keiner Lösung gekommen
 * ist, um eine Endlossschleife zu vermeiden.
 * 1. Parameter: leeres Spielfeld
 */
{
    time_t start_t, end_t;
    double iDiff_t;

    time(&start_t);

    resetGameData(iGameData);

    srand(time(NULL));
    clear_output();
    printf("              Generiere Spieldaten ...\n");
    fflush(stdout);
    usleep(500000);

    for (int _x = 0; _x < 9; _x++)
    {
        for (int _y = 0; _y < 9; _y++)
        {
            time(&end_t);
            iDiff_t = difftime(end_t, start_t);
            if (iDiff_t > 2)
            {
                generateGameData(iGameData);
                break;
            }
            int number = generateRandomNumber();

            /* Zahl darf nur einmal in der Zeile vorkommen. */
            if (isElementInArray(iGameData[_x], number, 9) >= 0)
            {
                _y--;
                resetArray(iElementsInSomeColumn, 9);
                continue;
            }

            resetArray(iElementsInSomeColumn, 9);
            for (int l = 0; l < 9; l++)
            {
                iElementsInSomeColumn[l] = iGameData[l][_y];
            }

            /* Zahl darf nur einmal in der Spalte und Box vorkommen. */
            if (isElementInArray(iElementsInSomeColumn, number, 9) >= 0 ||
                isElementInBox(iGameData, _x - _x % 3, _y - _y % 3, number) >= 0)
            {
                resetArray(iGameData[_x], 9);
                _x--;
                break;
            }
            iGameData[_x][_y] = number;
        }
    }
    printf("\n");

    fflush(stdout);
    clear_output();
}

int generateNumberByInterval(int iX, int iY)
/* Generiert eine Zufallszahl in einem definierbaren Intervall.
 * 1. Parameter: Intervall-Start
 * 2. Parameter: Intervall-Ende
 * Rückgabewert: generierte Zufallszahl
 */
{
    return iX + rand() % (iY - iX + 1);
}

void deleteCells(int iArray[][9], int iDifficulty)
/* Löscht je nach Schwierigkeitsgrad mehr oder weniger zufällige Zellen
 * aus dem sichtbaren Spielfeld und speichert die gelöschten Zellen im
 * Array iDeletedCells;
 * 1. Parameter: Spielfeld
 * 2. Parameter: Schwierigkeitsgrad
 */
{
    int iDelete;

    /* Bestimmung der zu löschenden Zellen in Abhängigkeit vom Schwierigkeitsgrad */
    switch(iDifficulty)
    {
        case 5:
            iDelete = 42;
            break;
        case 7:
            iDelete = 49;
            break;
        case 8:
            iDelete = 56;
            break;
    }

    for (int x = 1; x <= 3; x++)
    {
        for (int y = 1; y <= 3; y++)
        {
            int tmp = iDifficulty;
            while (tmp > 0)
            {
                int r = generateNumberByInterval(3 * (x - 1), 3 * x - 1);
                int c = generateNumberByInterval(3 * (y - 1), 3 * y - 1);
                if (iArray[r][c] > 0)
                { // not already deleted
                    iDeletedCells[r][c] = iArray[r][c];
                    iUserCells[r][c] = 1;
                    iArray[r][c] = 0;
                    iDelete--;
                }
                tmp--;
            }
        }
    }

    while (iDelete > 0)
    {
        int iX = rand() % 9;
        int iY = rand() % 9;

        /* Überprüfung, ob Zelle nicht bereits gelöscht wurde */
        if (iGameData[iX][iY] > 0)
        {
            iDeletedCells[iX][iY] = iGameData[iX][iY];
            iUserCells[iX][iY] = 1;
            iGameData[iX][iY] = 0;
            iDelete--;
        }
    }
}

int getGameStatus(int iArray[][9])
/* Überprüft, ob alle Felder des sichtbaren Spielfeldes aufgefüllt
 * wurden.
 * 1. Parameter: sichtbares Spielfeld
 * Rückgabewert: Vollständig (FILLED) oder unvollständig (NOT_FILLED)
 */
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (iArray[x][y] == 0)
            {
                return NOT_FILLED;
            }
        }
    }

    return FILLED;
}

int timer(int iAction)
/* Stoppuhr für das Spiel. Wird zur Messung der benötigten Zeit zum
 * Lösen des Sudokus verwendet. Stoppuhr kann (neu-)gestartet, ge-
 * stopped und pausiert werden.
 * 1. Parameter: Aktion für die Stoppuhr (Zeit abfragen, Zurück-
 * setzen, Starten, Stoppen)
 */
{

    static int iFirst = 0, iPaused = 0;
    static int iTimer = 0, iZwErg = 0;

    switch (iAction)
    {
        /* Keine Aktion, da nur aktuelle Zeit ausgegeben werden soll */
        case TIMER_STATE:
            break;

        /* Stoppuhr pausieren. Beim ersten Durchlauf, wird ein aktueller
         * Zeitstempel genommen, beim zweiten Durchlauf, wird die
         * pausierte Zeit zum Zwischenergebnis aufaddiert */
        case TIMER_PAUSE:
            if (iPaused == 0)
            {
                _pause = time(NULL);
                iPaused++;
            }
            else
            {
                end = time(NULL);
                iZwErg += (end - _pause);
                iPaused--;
            }
            break;

        /* iFirst auf 1 setzen, um Timer zu starten (erster Duchlauf) */
        case TIMER_START:
            iFirst = 1;
            iZwErg = 0;
            iPaused = 0;
            break;

        /* Stoppuhr bzw. alle zugehörigen Werte zurücksetzen. */
        case RESET_TIMER:
            iFirst = 1;
            iZwErg = 0;
            iPaused = 0;
            iTimer = 0;
            break;

        /* Vom Startzeitstempel 15 Sekunden abziehen, die als zusätzliche
         * 15 Strafsekungen gerechnet werden. */
        case TIPP_USED:
            start -= 15;
            break;

        /* Vom Startzeitstempel 30 Sekunden abziehen, die als zusätzliche
         * 30 Strafsekungen gerechnet werden. */
        case HELP_USED:
            start -= 30;
        default:
            break;
    }

    if (iFirst)
    {
        start = time(NULL);
        iFirst = 0;
    }

    end = time(NULL);

    iTimer = end - start;
    iTimer -= iZwErg;

    return iTimer;
}

void timeToString(int iUserTime, char cStringTime[])
/* Wandelt eine Zeitangabe in Sekunden in das "00:00"-Format um.
 * 1. Parameter: Zeitangabe in Sekunden
 * 2. Parameter: Char-Array, in den die formatierte Zeit geschrieben
 * werden soll
 */
{
    int seconds = iUserTime % 60;
    int minutes = iUserTime / 60;

    char cS[2] = {0};
    char cM[2] = {0};

    if (seconds < 10)
    {
        cS[0] = '0';
        cS[1] = '\0';
    }

    if (minutes < 10)
    {
        cM[0] = '0';
        cM[1] = '\0';
    }
    sprintf(cStringTime, "%s%d:%s%d", cM, minutes, cS, seconds);
    cStringTime[5] = '\0';
}

void checkGameState()
/* Leitet nötige Schritte zur Überprüfung, ob das Sudoku vollständig
 * und richtig gelöst wurde, ein.
 */
{
    if (iIsGameActive && getGameStatus(iGameData) == FILLED) {
        int solveState = solveGame(iGameData);
        if (solveState) {

            if (piUserID != 0 && isSolvedAutomatic == 0) {
                strcpy(cGameMessage, "insert.");

                int _score = timer(TIMER_STATE);
                insertScore(piUserID, _score, iDifficulty);
            }

            iIsGameActive = 0;
            iCurrentPosition = SOLVED_GAME;
        } else {
            strcpy(cGameMessage, "Das Spiel ist nicht korrekt geloest.");
        }
    }
}