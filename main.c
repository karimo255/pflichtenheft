/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: main.c
 * Datum:
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
#define HEIGHT 805
#define WIDTH 370
#include <Windows.h>
#include <conio.h>
#endif
int *userID;
int exitTheGame = 0;
int windows = 0;

sqlite3 *connection;

void resizeWindow()
{
#ifdef __unix__
    system("resize -s 48 48");
#endif

#ifdef __WIN32__
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    MoveWindow(hwnd, 550, 15, WIDTH, HEIGHT, TRUE);
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
char username[8] = {0};

void renderGame(struct sScore *scores);

void handleInput();

// Speicher am Ende freigeben.
int *bestScore = 0;

int remaining = 0;
int iMarks[9][9][MAX_MARKS];

int anzahlDerTipps = 0;
int anzahlDerHilfe = 0;

int erlaubteAnzahlDerTipps = 0;
int erlaubteAnzahlDerHilfe = 0;

void initDb()
{
    int rc = sqlite3_open("./sudoku.db", &connection);

    if (rc != SQLITE_OK)
    {
        printf("Failed to open the sudoku.db\n");
    }
    createUserTable();
    createScoreTable();
}

int main()
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
     * welchem "Screen" sich der Spieler befindet und je nachdem die entsprechenden
     * Funktionen ausführt.
     * Funktionen ausführt.
     * 1. Parameter: Zeiger auf die Struktur zum eintragen aller wichtigen Daten
     * (Nutzername, UserID, benötigte Zeit und Schwierigkeitsgrad)
     */

    while (!exitTheGame)
    {
        clear_output();
        renderGame(scores);
        fflush(stdout);
        resetArray(cGameMessage, 200);
        handleInput();
        if (iIsGameActive && getGameStatus(iGameData) == FILLED)
        {
            int solveState = solveGame(iGameData);
            if (solveState)
            {
                printf("solved2\n");
                if (userID != NULL && isSolvedAutomatic == 0)
                {
                    strcpy(cGameMessage, "insert.");

                    int _score = timer(TIMER_STATE);
                    insertScore(userID, _score, iDifficulty);
                }
                iIsGameActive = 0;
                printf("solved_game\n");
                iCurrentPosition = SOLVED_GAME;
            }
            else
            {
              strcpy(cGameMessage, "Das Spiel ist nicht korrekt geloest.");
            }
        }
    }

    sqlite3_close(connection);
    free(scores);
    free(bestScore);
    printf("Das Programm ist beendet\n");
    return 0;
}

void renderGame(struct sScore *scores)
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
        renderUsernameDialog(username);
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
        bestScore = malloc(sizeof(int));
        *bestScore = 0;
        getBestScore(bestScore, iDifficulty);
        remaining = getRemainingCells(iGameData);
        renderInfoBox(username, bestScore, iDifficulty, remaining);
        renderNotesBox(iX_coordinate, iY_coordinate);
        renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
        printGameMessage();
        renderGameMenu();
        sprintf(cGameMessage, "%s", "");
        break;
    case SET_MARK:
        remaining = getRemainingCells(iGameData);
        renderInfoBox(username, bestScore, iDifficulty, remaining);
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
        renderSolvedGame(isSolvedAutomatic, anzahlDerTipps, anzahlDerHilfe);
        renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
        break;
    }
}

void handleInput()
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
            navigateTo(getch()); // windows Navigation Tasten
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
