//
// Created by karim on 01.07.19.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#include "../../headers/core/inputHandler.h"
#include "../../headers/core/game.h"
#include "../../headers/services/user_service.h"
#include "../../headers/services/score_service.h"
#include "../../headers/services/connection.h"
#include "../../libs/sqlite3.h"
#include "../../headers/core/view.h"

#ifdef __unix__

#include <termios.h>
#include <unistd.h>
#include "../../headers/getch.h"

#elif __WIN32__
#include <Windows.h>
#include <conio.h>
#endif

int *isUserLoggedIn = 0;
char password[6] = {0};
char loginMessage[50];
int b=0;
int *userID = 0;
int difficulty;


void navigateTo(int pos) {
    switch (pos) { // the real value
        case UP:
            // code for arrow up
            x_coordinate--;
            break;
        case DOWN:
            // code for arrow down
            x_coordinate++;
            break;
        case RIGHT:
            // code for arrow right
            y_coordinate++;
            break;
        case LEFT:
            // code for arrow left
            y_coordinate--;
            break;
        case UP_LINUX:
            // code for arrow up
            x_coordinate--;
            break;
        case DOWN_LINUX:
            // code for arrow down
            x_coordinate++;
            break;
        case RIGHT_LINUX:
            // code for arrow right
            y_coordinate++;
            break;
        case LEFT_LINUX:
            // code for arrow left
            y_coordinate--;
            break;
    }

    if (x_coordinate > 8) {
        x_coordinate = 0;
    }
    if (x_coordinate < 0) {
        x_coordinate = 8;
    }
    if (y_coordinate > 8) {
        y_coordinate = 0;
    }
    if (y_coordinate < 0) {
        y_coordinate = 8;
    }
}

void handleUserNameInput(){
    char ch = getch();

    if (ch != 13 && ch != '\n' && ch != EOF) { // no enter
        if (strcmp(username, "Name eingeben...") == 0 ) resetArray(username, 30);
        if (ch == 27) { // escape
            strcpy(username, "anonym");
            currentPosition = DIFFICULTY_DIALOG;
        } else if (ch == 127 || ch == 8) {
            b--;
            if (b < 0) {
                b = 0;
            }
            username[b] = 0;

        } else if (b < 30) {
            username[b] = ch;
            b++;
        }
    } else if (strlen(username) > 0) { // enter
        username[b] = '\0';

        b = 0;
        if (strcmp(username, "Name eingeben...") == 0 || strcmp(username, "") == 0) {
            strcpy(username, "anonym");
        } else {
            userID = malloc(sizeof(int));
            getUserID(username, userID);
            if (*userID > 0) {
                currentPosition = ENTER_PASSWORD;
            } else {
                currentPosition = SET_PASSWORD;
            }
        }
    }
    free(userID);
}

void handleSetPasswordInput(){
    char ch = getch();

    printf("%d\n", ch);
    if (ch != 13 && ch != '\n' && ch != EOF) { // not enter
        //   if (password[0] == 0) resetArray(password, 8);
        if (ch == 127 || ch == 8) { // delete backspace
            b--;
            if (b < 0) {
                b = 0;
            }
            password[b] = 0;

        } else if (b < 6) {
            password[b] = ch;
            b++;
        }
    } else { // enter
        registerUser(username, password, userID);
        clear_output();
        fflush(stdout);
        currentPosition = DIFFICULTY_DIALOG;
    }
}

void handleEnterPasswordInput(){
    char ch = getch();

    if (ch != 13 && ch != '\n' && ch != EOF) { // not enter
        if (ch == 127 || ch == 8) { // delete backspace
            b--;
            if (b < 0) {
                b = 0;
            }
            password[b] = 0;

        } else if (b < 6) {
            password[b] = ch;
            b++;
        }
    } else { // enter
        isUserLoggedIn = malloc(sizeof(int));

        loginUser(username, password, isUserLoggedIn);
        if (*isUserLoggedIn > 0) {
            currentPosition = DIFFICULTY_DIALOG;
        } else {
            strcpy(gameMessage, "Passwort ist falsch");
            resetArray(password, 6);
            b = 0;
        }
    }
}

void handleDifficultyDialogInput(int userInput){
    if (isalpha(userInput)) {
        switch (userInput) {
            case 'a':
                difficulty = EASY;
                currentPosition = IN_GAME;
                setConfig();
                break;

            case 'b':
                difficulty = MEDIUM;
                currentPosition = IN_GAME;
                setConfig();
                break;

            case 'c':
                difficulty = HARD;
                currentPosition = IN_GAME;
                setConfig();
                break;
        }

    }
}

void handleMenuInput(int userInput){
    if (isalpha(userInput)) {
        switch (userInput) {
            case 's':
                if (isGameActive > 0) {
                    break;
                }

                currentPosition = USER_NAME;
                break;

            case 'r':
                if (isGameActive == 0) {
                    break;
                }
                timer(TIMER_PAUSE);
                currentPosition = IN_GAME;
                break;

            case 'b':
                currentPosition = DETAILS_DIALOG;
                break;

            case 'k':
                currentPosition = HELP;
                break;

            case 'q':
                exitTheGame = 1;
                break;
        }
    }
}

void handleInGameInput(int userInput){
    if (isdigit(userInput)) {
        if (userCells[x_coordinate][y_coordinate] == 1) {
            gameData[x_coordinate][y_coordinate] = userInput - '0';
        }
    } else if (isalpha(userInput)) {
        switch (userInput) {
            case 'h':
                if (deletedCells[x_coordinate][y_coordinate] > 0) {
                    if (anzahlDerHilfe == erlaubteAnzahlDerHilfe) {
                        strcpy(gameMessage, "Anzahl der Hilfen verbraucht.");
                        break;
                    }
                    anzahlDerHilfe++;
                    solveCell(gameData, x_coordinate, y_coordinate);
                    timer(HELP_USED);
                } else {
                    strcpy(gameMessage, "Zelle ist nicht leer.");
                }


                break;
            case 't':
                if (deletedCells[x_coordinate][y_coordinate] > 0) {
                    if (anzahlDerTipps == erlaubteAnzahlDerTipps) {
                        strcpy(gameMessage, "Anzahl der Tipps verbraucht.");
                        break;
                    }
                    anzahlDerTipps++;
                    fillNotesForCell(x_coordinate, y_coordinate);
                    timer(TIPP_USED);
                } else {
                    strcpy(gameMessage, "Tipp ist nicht verfuegbar");
                }

                break;
            case 'q':
                exitTheGame = 1;
            case 'a':
                resetGameData(gameData);
                isGameActive = 0;
                timer(RESET_TIMER);

            case 'p':
                timer(TIMER_PAUSE);
                currentPosition = MENU;
                break;
            case 'k':
                timer(TIMER_PAUSE);
                currentPosition = HELP;
                break;
            case 's':
                solveAll(gameData, deletedCells);
                isSolvedAutomatic = 1;
                break;
            case 'm':
                if (gameData[x_coordinate][y_coordinate] == 0) {
                    currentPosition = SET_MARK;
                } else {
                    strcpy(gameMessage, "Markiere-Mous nicht verfuegbar.");
                }
                break;
        }
    }
}

void handleSolvedGameInput(int userInput){
    if (isalpha(userInput)) {
        switch (userInput) {
            case 'z':
                currentPosition = MENU;
                break;
            case 'q':
                exitTheGame = 1;
                break;
        }
    }
}

void handleSetMarkInput(int userInput){
    if (isdigit(userInput)) {
        makeNote(x_coordinate, y_coordinate, userInput - '0');
    }
    if (isalpha(userInput)) {
        switch (userInput) {
            case 'm':
                strcpy(gameMessage, "Notizen erstellt");
                currentPosition = IN_GAME;
                break;
            case 'd':
                resetArray(marks[x_coordinate][y_coordinate], MAX_MARKS);
                strcpy(gameMessage, "Notizen geloescht");
                currentPosition = IN_GAME;
                break;
            case 'q':
                exitTheGame = 1;
        }
    }
}

void handleDetailsInput(int userInput){
    if (isalpha(userInput)) {
        switch (userInput) {
            case 'z':
                currentPosition = MENU;

                break;
        }
    }
}

void handleDetailsDialogInput(int userInput){
    if (isalpha(userInput)) {
        switch (userInput) {
            case 'e':
                difficulty = EASY;
                currentPosition = DETAILS;
                break;
            case 'm':
                difficulty = MEDIUM;
                currentPosition = DETAILS;
                break;
            case 's':
                difficulty = HARD;
                currentPosition = DETAILS;
                break;
            case 'z':
                currentPosition = MENU;
                break;
        }
    }
}

void handleHelpInput(int userInput){
    if (isalpha(userInput)) {
        switch (userInput) {
            case 'z':
                if (isGameActive > 0) {
                    timer(TIMER_PAUSE);
                    currentPosition = IN_GAME;
                } else {
                    currentPosition = MENU;
                }
                break;
        }
    }
}

void setConfig() {
    anzahlDerTipps = 0;
    anzahlDerHilfe = 0;
    switch (difficulty) {
        case EASY:
            erlaubteAnzahlDerHilfe = 5;
            erlaubteAnzahlDerTipps = 8;
            break;
        case MEDIUM:
            erlaubteAnzahlDerHilfe = 4;
            erlaubteAnzahlDerTipps = 6;
            break;
        case HARD:
            erlaubteAnzahlDerHilfe = 3;
            erlaubteAnzahlDerTipps = 4;
            break;
    }
}

void checkGameState() {
    if (isGameActive && getGameStatus(gameData) == FILLED && deletedCells[0][0] > 0) {
        int solveState = solveGame(gameData);
        if (solveState == 1) {
            if (*userID != 0 && isSolvedAutomatic == 0 && strcmp(username, "anonym") != 0) {
                int _score = timer(TIMER_STATE);
                insertScore(userID, _score, difficulty);
            }
            currentPosition = SOLVED_GAME;
            isGameActive = 0;
        } else {
            strcpy(gameMessage, "Das Spiel ist nicht korrekt geloest.");
        }
    }
}

clearMemory()
{
    free(isUserLoggedIn);
}