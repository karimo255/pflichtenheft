#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


#include "./headers/view.h"
#include "headers/game.h"
#include "headers/shared.h"

int arr[9][9];
int deletedCells[9][9];
int userCells[9][9];
int difficulty;
int isGameActive;
int currentPosition;

#ifdef __unix__
#include <termios.h>
#include <unistd.h>
static struct termios new_io;
static struct termios old_io;

int cbreak(int fd)
{
    if ((tcgetattr(fd, &old_io)) == -1)
        return -1;
    new_io = old_io;
    new_io.c_lflag = new_io.c_lflag & ~(ECHO | ICANON);
    new_io.c_cc[VMIN] = 1;
    new_io.c_cc[VTIME] = 0;

    if ((tcsetattr(fd, TCSAFLUSH, &new_io)) == -1)
        return -1;
    return 1;
}

int getch(void)
{
    int c;

    if (cbreak(STDIN_FILENO) == -1)
    {
        printf("Fehler bei der Funktion cbreak ... \n");
        exit(EXIT_FAILURE);
    }
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
    return c;
}

#elif __WIN32__ || _MSC_VER || __MS_DOS__
#include <conio.h>
#endif



int exitTheGame = 0;




int main()
{
    srand(time(NULL));

    currentPosition = MENU;
    difficulty = EASY;

    while (!exitTheGame)
    {
        system("clear");
        switch (currentPosition)
        {
            case MENU:
                renderMenu();
                break;

            case IN_GAME:
                if (isGameActive == 0)
                {
                    resetGameData(arr);
                    resetGameData(deletedCells);
                    resetGameData(userCells);
                    generateGameData(arr);
                    deleteCells(arr, difficulty);
                    isGameActive = 1;
                }

                renderInfoBox();
                renderCourt(arr, userCells, x, y, gameMessage);
                renderGameMenu();
                sprintf(gameMessage, "%s", "");
                break;

            case DIFFICULTY_DIALOG:
                renderDifficultyDialog();
                break;

            case DETAILS:
                renderDetails();
                break;
        }

        handleUserInput();
    }
    return 0;
}



void navigateTo(int pos)
{
    switch (pos)
    { // the real value
        case 'A':
            // code for arrow up
            x--;
            break;
        case 'B':
            // code for arrow down
            x++;
            break;
        case 'C':
            // code for arrow right
            y++;
            break;
        case 'D':
            // code for arrow left
            y--;
            break;
    }

    if (x > 8)
    {
        x = 8;
    }
    if (x < 0)
    {
        x = 0;
    }
    if (y > 8)
    {
        y = 8;
    }
    if (y < 0)
    {
        y = 0;
    }
}
void handleUserInput()
{
    int userInput;
    int pos;

    if ((userInput = getch()) == '\033')
    {          // if the first value is esc
        getch(); // skip the [
        navigateTo(getch());
    }
    else
    {
        switch (currentPosition)
        {

            case DIFFICULTY_DIALOG:
                if (userInput == 10) // enter pressed
                {
                    currentPosition = IN_GAME;
                }
                else if (isalpha(userInput))
                {
                    switch (userInput)
                    {
                        case 'a':
                            difficulty = EASY;
                            currentPosition = IN_GAME;
                            break;

                        case 'b':
                            difficulty = MEDIUM;
                            currentPosition = IN_GAME;
                            break;

                        case 'c':
                            difficulty = HARD;
                            currentPosition = IN_GAME;
                            break;
                    }
                }
                break;

            case MENU:
                if (isalpha(userInput))
                {
                    switch (userInput)
                    {
                        case 's':
                            if (isGameActive > 0)
                            {
                                break;
                            }

                            currentPosition = DIFFICULTY_DIALOG;
                            break;

                        case 'r':
                            if (isGameActive == 0)
                            {
                                break;
                            }

                            currentPosition = IN_GAME;
                            break;

                        case 'b':
                            currentPosition = DETAILS;
                            break;

                        case 'q':
                            exitTheGame = 1;
                            break;
                    }
                }
                break;

            case IN_GAME:
                if (isdigit(userInput))
                {
                    if (userCells[x][y] == 1)
                    {
                        arr[x][y] = userInput - '0';
                    }
                }
                else if (isalpha(userInput))
                {
                    switch (userInput)
                    {
                        case 'h':
                            solveCell(arr, x, y);
                            break;
                        case 's':
                            if (getGameStatus(arr) == NOT_FILLED)
                            {
                                sprintf(gameMessage, "%s", "Sudoku Noch nicht voll.");
                                break;
                            }

                            if (solveGame(arr) == 1)
                            {
                                sprintf(gameMessage, "%s", "Geloest");
                            }
                            else
                            {
                                sprintf(gameMessage, "%s", "Nicht geloest");
                            }
                            break;

                        case 'a':
                            resetGameData(arr);
                            isGameActive = 0;

                        case 'b':
                            currentPosition = MENU;
                            break;
                    }
                }
                break;
            case DETAILS:
                if (isalpha(userInput))
                {
                    switch (userInput)
                    {
                        case 'b':
                            currentPosition = MENU;
                            printf("Bestlist\n");
                            break;
                    }
                }
                break;
        }
    }
}



