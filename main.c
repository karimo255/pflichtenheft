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

int exitTheGame = 0;
int windows = 0;

sqlite3 *connection;

void resizeWindow() {
#ifdef __unix__
    system("resize -s 48 48");
#endif

#ifdef __WIN32__
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    MoveWindow(hwnd, 550, 15, WIDTH, HEIGHT, TRUE);
    setvbuf(stdout, NULL, _IOFBF, 10000);
#endif
}


int gameData[9][9];
int deletedCells[9][9];
int userCells[9][9];
int difficulty;
int isGameActive;
int isSolvedAutomatic;
int currentPosition;
char username[50] = "Name eingeben...\0";


// Speicher am Ende freigeben.
int *bestScore = 0;

int remaining = 0;
int marks[9][9][MAX_MARKS];


int anzahlDerTipps = 0;
int anzahlDerHilfe = 0;


int erlaubteAnzahlDerTipps = 0;
int erlaubteAnzahlDerHilfe = 0;

void initDb(){
    int rc = sqlite3_open("./sudoku.db", &connection);

    if (rc != SQLITE_OK) {
        printf("Failed to open the sudoku.db\n");
    }
    createUserTable();
    createScoreTable();
}

int main() {
    resizeWindow();
    initColors();

    fflush(stdout);
    srand(time(NULL));

    struct score *scores;
    scores = malloc(sizeof(score));
    scores->next = NULL;
    getScores(scores);
    currentPosition = MENU;
    difficulty = EASY;

        while (!exitTheGame) {
            clear_output();
            switch (currentPosition) {
                case SET_PASSWORD:
                    renderSetPassword();
                    fflush(stdout);
                    break;
                case ENTER_PASSWORD:
                    renderEnterPassword();
                    fflush(stdout);
                    break;
                case MENU:
                    renderMenu();
                    fflush(stdout);
                    break;
                case USER_NAME:
                    renderUsernameDialog(username);
                    fflush(stdout);
                    break;
                case IN_GAME:
                    if (isGameActive == 0) {
                        resetGameData(gameData);
                        resetGameData(deletedCells);
                        resetGameData(userCells);
                        generateGameData(gameData);
                        deleteCells(gameData, difficulty);
                        resetArray(marks[x_coordinate][y_coordinate], MAX_MARKS);
                        isGameActive = 1;
                        timer(TIMER_START);
                        isSolvedAutomatic = 0;
                    }
                    bestScore = malloc(sizeof(int));
                    getBestScore(bestScore, difficulty);
                    remaining = getRemainingCells(gameData);
                    renderInfoBox(username, bestScore, difficulty, remaining);
                    renderNotesBox(x_coordinate, y_coordinate);
                    renderCourt(gameData, userCells, x_coordinate, y_coordinate);
                    printGameMessage();
                    renderGameMenu();
                    sprintf(gameMessage, "%s", "");
                    fflush(stdout);
                    free(bestScore);
                    break;
                case SET_MARK:
                    remaining = getRemainingCells(gameData);
                    renderInfoBox(username, bestScore, difficulty, remaining);
                    renderMarkModeMessage();
                    renderCourt(gameData, userCells, x_coordinate, y_coordinate);
                    renderGameMenu();
                    sprintf(gameMessage, "%s", "");
                    fflush(stdout);
                    break;
                case DIFFICULTY_DIALOG:
                    renderDifficultyDialog();
                    fflush(stdout);
                    break;
                case DETAILS_DIALOG:
                    renderDBestScoreDialog();
                    fflush(stdout);
                    break;
                case DETAILS:
                    renderDetails(scores, difficulty);
                    fflush(stdout);
                    break;
                case HELP:
                    renderHelpDialog();
                    fflush(stdout);
                    break;
                case SOLVED_GAME:
                    renderSolvedGame(isSolvedAutomatic, anzahlDerTipps, anzahlDerHilfe);
                    renderCourt(gameData, userCells, x_coordinate, y_coordinate);
                    fflush(stdout);
                    break;
            }
            resetArray(gameMessage, 200);
            if (currentPosition == USER_NAME) {
                handleUserNameInput();
            } else if (currentPosition == SET_PASSWORD) {
                handleSetPasswordInput();
            } else if (currentPosition == ENTER_PASSWORD) {
                handleEnterPasswordInput();
            } else {
                int userInput;

                if ((userInput = getch()) == 224) {
                    navigateTo(getch()); // windows Navigation Tasten
                } else {
                    navigateTo(userInput); // linux Navigation Tasten
                    switch (currentPosition) {
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
                        case SET_MARK :
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
            checkGameState();
        }
        
    sqlite3_close(connection);
    free(scores);
    printf("Das Programm ist beendet\n");
    return 0;
}





