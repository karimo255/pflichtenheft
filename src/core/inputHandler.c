/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden im Wesentlichen alle für das Spiel
 * nötigen Berechnungen durchgeführt. Z.B. das Generieren des Spiel-
 * feldes oder der Timer, wie lange ein Spieler für das Lösen
 * ein Spiel benötigt.
*/

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
int iCurrentPosition;
int b = 0;
int *userID = 0;
int iDifficulty;

void navigateTo(int pos)
{
    switch (pos)
    { // the real value
    case UP:
        // code for arrow up
        iX_coordinate--;
        break;
    case DOWN:
        // code for arrow down
        iX_coordinate++;
        break;
    case RIGHT:
        // code for arrow right
        iY_coordinate++;
        break;
    case LEFT:
        // code for arrow left
        iY_coordinate--;
        break;
    case UP_LINUX:
        // code for arrow up
        iX_coordinate--;
        break;
    case DOWN_LINUX:
        // code for arrow down
        iX_coordinate++;
        break;
    case RIGHT_LINUX:
        // code for arrow right
        iY_coordinate++;
        break;
    case LEFT_LINUX:
        // code for arrow left
        iY_coordinate--;
        break;
    }

    if (iX_coordinate > 8)
    {
        iX_coordinate = 0;
    }
    if (iX_coordinate < 0)
    {
        iX_coordinate = 8;
    }
    if (iY_coordinate > 8)
    {
        iY_coordinate = 0;
    }
    if (iY_coordinate < 0)
    {
        iY_coordinate = 8;
    }
}

void handleUserNameInput()
{
    char ch = getch();

    if (ch != 13 && ch != '\n' && ch != EOF)
    { // no enter
        if (ch == 27)
        { // escape
            strcpy(username, "anonym");
            iCurrentPosition = DIFFICULTY_DIALOG;
        }
        else if (ch == 127 || ch == 8)
        {
            b--;
            if (b < 0)
            {
                b = 0;
            }
            username[b] = 0;
        }
        else if (b < 8)
        {
            username[b] = ch;
            b++;
        }
    }
    else if (strlen(username) > 0)
    { // enter
        username[b] = '\0';

        b = 0;
        if (username[0] == 0)
        {
            strcpy(username, "anonym");
        }
        else
        {
            userID = malloc(sizeof(int));
            *userID = 0;
            getUserID(username, userID);
            char test[200] = {0};
            sprintf(test, "userID: %d", *userID);
            strcpy(cGameMessage, test);
            if (*userID > 0)
            {
                iCurrentPosition = ENTER_PASSWORD;
            }
            else
            {
                iCurrentPosition = SET_PASSWORD;
            }
        }
    }
    free(userID);
}

void handleSetPasswordInput()
{
    char ch = getch();

    printf("%d\n", ch);
    if (ch != 13 && ch != '\n' && ch != EOF)
    { // not enter
        //   if (password[0] == 0) resetArray(password, 8);
        if (ch == 127 || ch == 8)
        { // delete backspace
            b--;
            if (b < 0)
            {
                b = 0;
            }
            password[b] = 0;
        }
        else if (b < 6)
        {
            password[b] = ch;
            b++;
        }
    }
    else
    { // enter
        registerUser(username, password, userID);
        clear_output();
        fflush(stdout);
        iCurrentPosition = DIFFICULTY_DIALOG;
    }
}

void handleEnterPasswordInput()
{
    char ch = getch();

    if (ch != 13 && ch != '\n' && ch != EOF)
    { // not enter
        if (ch == 127 || ch == 8)
        { // delete backspace
            b--;
            if (b < 0)
            {
                b = 0;
            }
            password[b] = 0;
        }
        else if (b < 6)
        {
            password[b] = ch;
            b++;
        }
    }
    else
    { // enter
        isUserLoggedIn = malloc(sizeof(int));
        *isUserLoggedIn = 0;

        loginUser(username, password, isUserLoggedIn);
        if (*isUserLoggedIn > 0)
        {
            iCurrentPosition = DIFFICULTY_DIALOG;
        }
        else
        {
            strcpy(cGameMessage, "Passwort ist falsch");
            resetArray(password, 6);
            b = 0;
        }
    }
}

void handleDifficultyDialogInput(int userInput)
{
    if (isalpha(userInput))
    {
        switch (userInput)
        {
        case 'a':
            iDifficulty = EASY;
            iCurrentPosition = IN_GAME;
            setConfig();
            break;

        case 'b':
            iDifficulty = MEDIUM;
            iCurrentPosition = IN_GAME;
            setConfig();
            break;

        case 'c':
            iDifficulty = HARD;
            iCurrentPosition = IN_GAME;
            setConfig();
            break;
        }
    }
}

void handleMenuInput(int userInput)
{
    if (isalpha(userInput))
    {
        switch (userInput)
        {
        case 's':
            if (iIsGameActive > 0)
            {
                break;
            }
            b = strlen(username);
            iCurrentPosition = USER_NAME;
            break;

        case 'r':
            if (iIsGameActive == 0)
            {
                break;
            }
            timer(TIMER_PAUSE);
            iCurrentPosition = IN_GAME;
            break;

        case 'b':
            iCurrentPosition = DETAILS_DIALOG;
            break;

        case 'k':
            iCurrentPosition = HELP;
            break;

        case 'q':
            exitTheGame = 1;
            break;
        }
    }
}

void handleInGameInput(int userInput)
{
    if (isdigit(userInput))
    {
        if (iUserCells[iX_coordinate][iY_coordinate] == 1)
        {
            iGameData[iX_coordinate][iY_coordinate] = userInput - '0';
        }
    }
    else if (isalpha(userInput))
    {
        switch (userInput)
        {
        case 'h':
            if (iDeletedCells[iX_coordinate][iY_coordinate] > 0)
            {
                if (anzahlDerHilfe == erlaubteAnzahlDerHilfe)
                {
                    strcpy(cGameMessage, "Anzahl der Hilfen verbraucht.");
                   // break;
                }
                anzahlDerHilfe++;
                solveCell(iGameData, iX_coordinate, iY_coordinate);
                timer(HELP_USED);
            }
            else
            {
                strcpy(cGameMessage, "Zelle ist nicht leer.");
            }
            break;
        case 't':
            if (iDeletedCells[iX_coordinate][iY_coordinate] > 0)
            {
                if (anzahlDerTipps == erlaubteAnzahlDerTipps)
                {
                    strcpy(cGameMessage, "Anzahl der Tipps verbraucht.");
                    break;
                }
                anzahlDerTipps++;
                fillNotesForCell(iX_coordinate, iY_coordinate);
                timer(TIPP_USED);
            }
            else
            {
                strcpy(cGameMessage, "Tipp ist nicht verfuegbar");
            }

            break;
        case 'q':
            exitTheGame = 1;
        case 'a':
            resetGameData(iGameData);
            iIsGameActive = 0;
            resetArray(username, 8);
            timer(RESET_TIMER);

        case 'p':
            timer(TIMER_PAUSE);
            iCurrentPosition = MENU;
            break;
        case 'k':
            timer(TIMER_PAUSE);
            iCurrentPosition = HELP;
            break;
        case 's':
            solveAll(iGameData, iDeletedCells);
            isSolvedAutomatic = 1;
            iCurrentPosition = SOLVED_GAME;
            break;
        case 'm':
            if (iGameData[iX_coordinate][iY_coordinate] == 0)
            {
                iCurrentPosition = SET_MARK;
            }
            else
            {
                strcpy(cGameMessage, "Markiere-Mous nicht verfuegbar.");
            }
            break;
        }
    }
}

void handleSolvedGameInput(int userInput)
{
    if (isalpha(userInput))
    {
        switch (userInput)
        {
        case 'z':
            iCurrentPosition = MENU;
            break;
        case 'q':
            exitTheGame = 1;
            break;
        }
    }
}

void handleSetMarkInput(int userInput)
{
    if (isdigit(userInput))
    {
        makeNote(iX_coordinate, iY_coordinate, userInput - '0');
    }
    if (isalpha(userInput))
    {
        switch (userInput)
        {
        case 'm':
            strcpy(cGameMessage, "Notizen erstellt");
            iCurrentPosition = IN_GAME;
            break;
        case 'd':
            resetArray(iMarks[iX_coordinate][iY_coordinate], MAX_MARKS);
            strcpy(cGameMessage, "Notizen geloescht");
            iCurrentPosition = IN_GAME;
            break;
        case 'q':
            exitTheGame = 1;
        }
    }
}

void handleDetailsInput(int userInput)
{
    if (isalpha(userInput))
    {
        switch (userInput)
        {
        case 'z':
            iCurrentPosition = MENU;
            break;
        }
    }
}

void handleDetailsDialogInput(int userInput)
{
    if (isalpha(userInput))
    {
        switch (userInput)
        {
        case 'e':
            iDifficulty = EASY;
            iCurrentPosition = DETAILS;
            break;
        case 'm':
            iDifficulty = MEDIUM;
            iCurrentPosition = DETAILS;
            break;
        case 's':
            iDifficulty = HARD;
            iCurrentPosition = DETAILS;
            break;
        case 'z':
            iCurrentPosition = MENU;
            break;
        }
    }
}

void handleHelpInput(int userInput)
{
    if (isalpha(userInput))
    {
        switch (userInput)
        {
        case 'z':
            if (iIsGameActive > 0)
            {
                timer(TIMER_PAUSE);
                iCurrentPosition = IN_GAME;
            }
            else
            {
                iCurrentPosition = MENU;
            }
            break;
        }
    }
}

void setConfig()
{
    anzahlDerTipps = 0;
    anzahlDerHilfe = 0;
    switch (iDifficulty)
    {
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
//
//void checkGameState()
//{
//    printf("solved_game\n");
//
//    if (iIsGameActive && getGameStatus(iGameData) == FILLED)
//    {
//        int solveState = solveGame(iGameData);
//        if (solveState)
//        {
//            printf("solved2\n");
//            if (*userID != 0 && isSolvedAutomatic == 0 && strcmp(username, "anonym") != 0)
//            {
//                strcpy(cGameMessage, "insert.");
//
//                int _score = timer(TIMER_STATE);
//                insertScore(userID, _score, iDifficulty);
//            }
//            iIsGameActive = 0;
//            resetGameData(iGameData);
//            printf("solved_game\n");
//            iCurrentPosition = SOLVED_GAME;
//
//        }
//        else
//        {
//           // strcpy(cGameMessage, "Das Spiel ist nicht korrekt geloest.");
//        }
//    }
//}