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
    iWindows = 1;
#endif
}

sqlite3 *psqlConnection;

int iExitTheGame = 0;
int iGameData[9][9];
int iDeletedCells[9][9];
int iUserCells[9][9];
int iDifficulty;
int iIsGameActive;
int isSolvedAutomatic;
int iCurrentPosition;
char cUsername[8] = {0};

/* Speicher am Ende freigeben. */
int *piBestScore = 0;

int iRemaining = 0;
int iMarks[9][9][MAX_MARKS];
int iAnzahlDerTipps = 0;
int iAnzahlDerHilfe = 0;
int iErlaubteAnzahlDerTipps = 0;
int iErlaubteAnzahlDerHilfe = 0;

void renderGame(struct sScore *scores);

void handleInput();

void initDb()
/* Baut Verbindung zur Datenbank auf und erstellt bei Bedarf
 * eine neue Datenbank mit entsprechenden Tabellen.
 */
{
    int rc = sqlite3_open("./sudoku.db", &psqlConnection);

    if (rc != SQLITE_OK)
    {
        printf("Failed to open the sudoku.db\n");
    }
    createUserTable();
    createScoreTable();
}

int main()
/* Hauptfunktion. "Grundfunktionalitäten" werden bereitgestellt.
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

    struct sScore *scores;
    scores = malloc(sizeof(struct sScore));
    scores->next = NULL;
    getScores(scores);
    iCurrentPosition = MENU;
    iDifficulty = EASY;

    /** Hauptspielzyklus. Er hält das Spiel am laufen, indem er überpüft, in
     * welchem "Screen" sich der Spieler befindet und je nachdem die entsprechenden
     * Funktionen ausführt.
     * 1. Parameter: Zeiger auf die Struktur zum eintragen aller wichtigen Daten
     * (Nutzername, UserID, benötigte Zeit und Schwierigkeitsgrad)
     */
    while (!iExitTheGame)
    {
        clear_output();
        renderGame(scores);
        fflush(stdout);
        resetArray(cGameMessage, 200);
        if (iCurrentPosition == IN_GAME && iWindows == 1)
        {
            int iEnde = 0;
            time_t now, notNow;
            now = time(NULL);

            while ((time(&notNow) - now) == 0 && !iEnde)
            {
                if (kbhit())
                {
                    handleInput();
                    checkGameState();
                    iEnde = 1;
                }
                Sleep(1);
            }
        }
        else
        {
            handleInput();
            checkGameState();
        }
    }

    sqlite3_close(psqlConnection);

    free(scores);
    free(piBestScore);

    printf("Das Programm ist beendet\n");

    return 0;
}

void renderGame(struct sScore *scores)
/* Hauptspielzyklus. Er hält das Spiel am laufen, indem er überpüft, in
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
            renderUsernameDialog(cUsername);
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
            piBestScore = malloc(sizeof(int));
            *piBestScore = 0;
            getBestScore(piBestScore, iDifficulty);
            iRemaining = getRemainingCells(iGameData);
            renderInfoBox(cUsername, piBestScore, iDifficulty, iRemaining);
            renderNotesBox(iX_coordinate, iY_coordinate);
            renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
            printGameMessage();
            renderGameMenu();
            sprintf(cGameMessage, "%s", "");
            break;
        case SET_MARK:
            iRemaining = getRemainingCells(iGameData);
            renderInfoBox(cUsername, piBestScore, iDifficulty, iRemaining);
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
/* Ruft die nötigen Funktionen für die Verarbeitung der Nutzereingabe auf.
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
        int userInput;

        if ((userInput = getch()) == 224)
        {
            navigateTo(getch()); // iWindows Navigation Tasten
        }
        else
        {
            navigateTo(userInput); // linux Navigation Tasten
            switch (iCurrentPosition)
            {
            case DIFFICULTY_DIALOG:
                handleDifficultyDialogInput(userInput);
                break;
            case MENU:
                handleMenuInput(userInput);
                break;
            case IN_GAME:
                handleInGameInput(userInput);
                break;
            case SOLVED_GAME:
                handleSolvedGameInput(userInput);
                break;
            case SET_MARK:
                handleSetMarkInput(userInput);
                break;
            case DETAILS:
                handleDetailsInput(userInput);
                break;
            case DETAILS_DIALOG:
                handleDetailsDialogInput(userInput);
                break;
            case HELP:
                handleHelpInput(userInput);
                break;
            }
        }
    }
}
