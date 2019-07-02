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
char cPassword[6] = {0};
int iCurrentPosition;
int iB = 0;
int *iPUserID = 0;
int iDifficulty;

void navigateTo(int iPos)
{
    switch (iPos)
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
    char cCh = getch();

    if (cCh != 13 && cCh != '\n' && cCh != EOF)
    { // no enter
        if (cCh == 27)
        { // escape
            strcpy(cUusername, "anonym");
            iCurrentPosition = DIFFICULTY_DIALOG;
        }
        else if (cCh == 127 || cCh == 8)
        {
            iB--;
            if (iB < 0)
            {
                iB = 0;
            }
            cUusername[iB] = 0;
        }
        else if (iB < 8)
        {
            cUusername[iB] = cCh;
            iB++;
        }
    }
    else if (strlen(cUusername) > 0)
    { // enter
        cUusername[iB] = '\0';

        iB = 0;
        if (cUusername[0] == 0)
        {
            strcpy(cUusername, "anonym");
        }
        else
        {
            iPUserID = malloc(sizeof(int));
            *iPUserID = 0;
            getUserID(cUusername, iPUserID);
            char cM[200] = {0};
            sprintf(cM, "userID: %d", *iPUserID);
            strcpy(cGameMessage, cM);
            if (*iPUserID > 0)
            {
                iCurrentPosition = ENTER_PASSWORD;
            }
            else
            {
                iCurrentPosition = SET_PASSWORD;
            }
        }
    }
}

void handleSetPasswordInput()
{
    char cCh = getch();

    printf("%d\n", cCh);
    if (cCh != 13 && cCh != '\n' && cCh != EOF)
    { // not enter
        //   if (password[0] == 0) resetArray(password, 8);
        if (cCh == 127 || cCh == 8)
        { // delete backspace
            iB--;
            if (iB < 0)
            {
                iB = 0;
            }
            cPassword[iB] = 0;
        }
        else if (iB < 6)
        {
            cPassword[iB] = cCh;
            iB++;
        }
    }
    else
    { // enter
        registerUser(cUusername, cPassword, iPUserID);
        clear_output();
        fflush(stdout);
        iCurrentPosition = DIFFICULTY_DIALOG;
    }
}

void handleEnterPasswordInput()
{
    char cCh = getch();

    if (cCh != 13 && cCh != '\n' && cCh != EOF)
    { // not enter
        if (cCh == 127 || cCh == 8)
        { // delete backspace
            iB--;
            if (iB < 0)
            {
                iB = 0;
            }
            cPassword[iB] = 0;
        }
        else if (iB < 6)
        {
            cPassword[iB] = cCh;
            iB++;
        }
    }
    else
    { // enter
        isUserLoggedIn = malloc(sizeof(int));
        *isUserLoggedIn = 0;

        loginUser(cUusername, cPassword, isUserLoggedIn);
        if (*isUserLoggedIn > 0)
        {
            iCurrentPosition = DIFFICULTY_DIALOG;
        }
        else
        {
            strcpy(cGameMessage, "Passwort ist falsch");
            resetArray(cPassword, 6);
            iB = 0;
        }
    }
}

void handleDifficultyDialogInput(int iUserInput)
{
    if (isalpha(iUserInput))
    {
        switch (iUserInput)
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

void handleMenuInput(int iUserInput)
{
    if (isalpha(iUserInput))
    {
        switch (iUserInput)
        {
        case 's':
            if (iIsGameActive > 0)
            {
                break;
            }
            iB = strlen(cUusername);
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
            iExitTheGame = 1;
            break;
        }
    }
}

void handleInGameInput(int iUserInput)
{
    if (isdigit(iUserInput))
    {
        if (iUserCells[iX_coordinate][iY_coordinate] == 1)
        {
            iGameData[iX_coordinate][iY_coordinate] = iUserInput - '0';
        }
    }
    else if (isalpha(iUserInput))
    {
        switch (iUserInput)
        {
        case 'h':
            if (iDeletedCells[iX_coordinate][iY_coordinate] > 0)
            {
                if (iAnzahlDerHilfe == iErlaubteAnzahlDerTipps)
                {
                    strcpy(cGameMessage, "Anzahl der Hilfen verbraucht.");
                    // break;
                }
                iAnzahlDerHilfe++;
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
                if (iAnzahlDerTipps == iErlaubteAnzahlDerHilfe)
                {
                    strcpy(cGameMessage, "Anzahl der Tipps verbraucht.");
                    break;
                }
                iAnzahlDerTipps++;
                fillNotesForCell(iX_coordinate, iY_coordinate);
                timer(TIPP_USED);
            }
            else
            {
                strcpy(cGameMessage, "Tipp ist nicht verfuegbar");
            }

            break;
        case 'q':
            iExitTheGame = 1;
        case 'a':
            resetGameData(iGameData);
            iIsGameActive = 0;
            resetArray(cUusername, 8);
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

void handleSolvedGameInput(int iUserInput)
{
    if (isalpha(iUserInput))
    {
        switch (iUserInput)
        {
        case 'z':
            iCurrentPosition = MENU;
            break;
        case 'q':
            iExitTheGame = 1;
            break;
        }
    }
}

void handleSetMarkInput(int iUserInput)
{
    if (isdigit(iUserInput))
    {
        makeNote(iX_coordinate, iY_coordinate, iUserInput - '0');
    }
    if (isalpha(iUserInput))
    {
        switch (iUserInput)
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
            iExitTheGame = 1;
        }
    }
}

void handleDetailsInput(int iUserInput)
{
    if (isalpha(iUserInput))
    {
        switch (iUserInput)
        {
        case 'z':
            iCurrentPosition = MENU;
            break;
        }
    }
}

void handleDetailsDialogInput(int iUserInput)
{
    if (isalpha(iUserInput))
    {
        switch (iUserInput)
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

void handleHelpInput(int iUserInput)
{
    if (isalpha(iUserInput))
    {
        switch (iUserInput)
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
    iAnzahlDerTipps = 0;
    iAnzahlDerHilfe = 0;
    switch (iDifficulty)
    {
    case EASY:
        iErlaubteAnzahlDerTipps = 5;
        iErlaubteAnzahlDerHilfe = 8;
        break;
    case MEDIUM:
        iErlaubteAnzahlDerTipps = 4;
        iErlaubteAnzahlDerHilfe = 6;
        break;
    case HARD:
        iErlaubteAnzahlDerTipps = 3;
        iErlaubteAnzahlDerHilfe = 4;
        break;
    }
}
