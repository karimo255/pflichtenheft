/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: main.c
 * Datum: 24.06.19
 * Beschreibung: Das ist das Hauptprogramm. Von hier aus wird der Mainloop ausgeführt,
 * der das Spiel kontrolliert. Es wird überprüft, in welchem "Screen" sich der Nutzer
 * gerade befindet und ob er Eingaben tätigt.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "headers/core/inputHandler.h"
#include "headers/core/game.h"
#include "headers/services/user_service.h"
#include "headers/services/score_service.h"
#include "headers/services/connection.h"
#include "libs/sqlite3.h"
#include "headers/core/view.h"

#define TRUE 1
#define FALSE 0

#ifdef __unix__

#include <termios.h>
#include <unistd.h>

#include "headers/getch.h"

#elif __WIN32__
#define HEIGHT 825
#define WIDTH 420
#include <Windows.h>
#include <conio.h>
#endif

int iWindows = 0;

void resizeWindow()
{
#ifdef __unix__
    system("resize -s 48 48");
#endif

#ifdef __WIN32__
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    MoveWindow(hwnd, 550, 5, WIDTH, HEIGHT, TRUE);
    setvbuf(stdout, NULL, _IOFBF, 10000);
#endif
}

int iGameData[9][9];
int iDeletedCells[9][9];
int iUserCells[9][9];
int iDifficulty;
int iIsGameActive;
int isSolvedAutomatic;
int iCurrentPosition;
char cUusername[8] = {0};
int iExitTheGame = 0;
int iMarks[9][9][MAX_MARKS];
int iAnzahlDerTipps = 0;
int iAnzahlDerHilfe = 0;
int iErlaubteAnzahlDerHilfe = 0;
int iErlaubteAnzahlDerTipps = 0;
int remaining = 0;

struct sScore *scores;

void renderGame(struct sScore *scores);
void handleInput();

/* Speicher am Ende freigeben. */
int *iBestScore = 0;

int *piUserID;

int *piIsUserLoggedIn;


sqlite3 *psqlConnection;

void initDb()
/** Baut Verbindung zur Datenbank auf und erstellt bei Bedarf
 * eine neue Datenbank mit entsprechenden Tabellen.
 */
{
    int rc = sqlite3_open("./db/sudoku.db", &psqlConnection);

    if (rc != SQLITE_OK)
    {
        printf("Failed to open the sudoku.db\n");
    }
    createUserTable();
    createScoreTable();
}

int main()
/** Hauptfunktion. "Grundfunktionalitäten" werden bereitgestellt.
 * Hierzu zählen das Setzen der Fenstergröße, das Initialisieren der
 * Farben, die Herstellung einer Verbindung zur Datenbank ( + eventuelles
 * Erstellen dieser) und das Starten des Hauptspielzykluses.
 */
{
    initDb();
    resizeWindow();
    initColors();

    fflush(stdout);

    srand(time(NULL));


    iCurrentPosition = MENU;
    iDifficulty = EASY;

    /** Hauptspielzyklus. Er hält das Spiel am laufen, indem er überpüft, in
     * welchem "Screen" sich der Spieler befindet und je nachdem die entsprechenden
     * welchem "Screen" sich der Spieler befindet und je nachdem die entsprechenden
     * Funktionen ausführt.
     * Funktionen ausführt.
     * 1. Parameter: Zeiger auf die Struktur zum eintragen aller wichtigen Daten
     * (Nutzername, UserID, benötigte Zeit und Schwierigkeitsgrad)
     */
    while (!iExitTheGame)
    {
        scores = malloc(sizeof(struct sScore));
        scores->next = NULL;
        getScores(scores);
        clear_output();
        renderGame(scores);
        fflush(stdout);
        resetArray(cGameMessage, 200);
        handleInput();
        checkGameState();
    }

    sqlite3_close(psqlConnection);

    free(scores);
    free(iBestScore);
    free(piUserID);
    free(piIsUserLoggedIn);

    printf("Das Programm ist beendet\n");
    return 0;
}

void renderGame(struct sScore *scores)
/** Hauptspielzyklus. Er hält das Spiel am laufen, indem er überpüft, in
* welchem "Screen" sich der Spieler befindet und je nachdem die entsprechenden
* Funktionen ausführt.
* 1. Parameter: Zeiger auf die Struktur zum eintragen aller wichtigen Daten
* (Nutzername, UserID, benötigte Zeit und Schwierigkeitsgrad)
*/
{
    switch (iCurrentPosition)
    {
        case SET_PASSWORD:
            renderSetPassword();
            break;
        case ENTER_PASSWORD:
            renderEnterPassword();
            break;
        case MENU:
            renderMenu();
            break;
        case USER_NAME:
            renderUsernameDialog(cUusername);
            break;
        case IN_GAME:
            if (iIsGameActive == 0)
            {
                resetGameData(iGameData);
                resetGameData(iDeletedCells);
                resetGameData(iUserCells);
                generateGameData(iGameData);
                deleteCells(iGameData, iDifficulty);
                resetArray(iMarks[iX_coordinate][iY_coordinate], MAX_MARKS);
                iIsGameActive = 1;
                timer(TIMER_START);
                isSolvedAutomatic = 0;
            }
            iBestScore = malloc(sizeof(int));
            *iBestScore = 0;
            getBestScore(iBestScore, iDifficulty);
            int iCurrentUserBestScore = 0;
            if (piUserID!= 0 && *piUserID > 0){
                iCurrentUserBestScore = getBestScoreByUserID(*piUserID);
            }
            remaining = getRemainingCells(iGameData);
            renderInfoBox(cUusername, iBestScore, iCurrentUserBestScore, iDifficulty, remaining);
            renderNotesBox(iX_coordinate, iY_coordinate);
            renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
            printGameMessage();
            renderGameMenu();
            sprintf(cGameMessage, "%s", "");
            break;
        case SET_MARK:
            remaining = getRemainingCells(iGameData);
            iCurrentUserBestScore = getBestScoreByUserID(*piUserID);
            renderInfoBox(cUusername, iBestScore, iCurrentUserBestScore, iDifficulty, remaining);
            renderMarkModeMessage();
            renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
            renderGameMenu();
            sprintf(cGameMessage, "%s", "");
            break;
        case DIFFICULTY_DIALOG:
            renderDifficultyDialog();
            break;
        case DETAILS_DIALOG:
            renderDBestScoreDialog();
            break;
        case DETAILS:
            renderDetails(scores, iDifficulty);
            break;
        case HELP:
            renderHelpDialog();
            break;
        case SOLVED_GAME:
            renderSolvedGame(isSolvedAutomatic, iAnzahlDerTipps, iAnzahlDerHilfe);
            renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
            break;
    }
}

void handleInput()
/** Ruft die nötigen Funktionen für die Verarbeitung der Nutzereingabe auf.
 */
{
    if (iCurrentPosition == USER_NAME)
    {
        handleUserNameInput();
    }
    else if (iCurrentPosition == SET_PASSWORD)
    {
        handleSetPasswordInput();
    }
    else if (iCurrentPosition == ENTER_PASSWORD)
    {
        handleEnterPasswordInput();
    }
    else
    {
        int iUserInput;

        if ((iUserInput = getch()) == 224)
        {
            /* Windows Navigationstasten */
            navigateTo(getch());
        }
        else
        {
            /* Linux Navigationstasten */
            navigateTo(iUserInput);
            switch (iCurrentPosition)
            {
                case DIFFICULTY_DIALOG:
                    handleDifficultyDialogInput(iUserInput);
                    break;
                case MENU:
                    handleMenuInput(iUserInput);
                    break;
                case IN_GAME:
                    handleInGameInput(iUserInput);
                    break;
                case SOLVED_GAME:
                    handleSolvedGameInput(iUserInput);
                    break;
                case SET_MARK:
                    handleSetMarkInput(iUserInput);
                    break;
                case DETAILS:
                    handleDetailsInput(iUserInput);
                    break;
                case DETAILS_DIALOG:
                    handleDetailsDialogInput(iUserInput);
                    break;
                case HELP:
                    handleHelpInput(iUserInput);
                    break;
            }
        }
    }
}
