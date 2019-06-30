//
// Created by karim on 24.06.19.
//
#include <stdlib.h>
#include "../../headers/core/game.h"
#include "../../headers/shared/shared.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <values.h>
#include <unistd.h>

int x_coordinate=0;
int y_coordinate=0;
int difficulty=EASY;
char gameMessage[200] = {0};
int isGameActive;
int currentPosition=0;


int gameData[9][9] = {0};
int marks[9][9][MAX_MARKS];
int deletedCells[9][9] = {0};
int userCells[9][9] = {0};

int elementsInSomeColumn[9] = {0};

time_t start, end, _pause;

void fillNotesForCell(int x_coordinate, int y_coordinate)
{
        int randomIndexForSolution = rand() % 2;
        marks[x_coordinate][y_coordinate][randomIndexForSolution] = deletedCells[x_coordinate][y_coordinate];
        for (int i = 0; i < MAX_MARKS; i++) {
            if(marks[x_coordinate][y_coordinate][i] == 0) {
                int number = generateRandomNumber();
                if (isElementInArray(marks[x_coordinate][y_coordinate], number,MAX_MARKS) > 0) {
                    i--;
                    continue;
                }
                marks[x_coordinate][y_coordinate][i] = number;
            }
        }
}

void solveCell(int array[][9], int x, int y)
{
    if (deletedCells[x][y] > 0)
    {
        userCells[x][y] = 0;
        array[x][y] = deletedCells[x][y];
    }
}

void makeNote(int x, int y, int suggestion){
    for (int i = 0; i < MAX_MARKS; ++i) {
        if(marks[x][y][i] == 0){
            marks[x][y][i] = suggestion;
            break;
        }
    }
}

void solveAll(int gameData[][9], int deletedCells[][9]){
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (deletedCells[x][y] != 0) {
                gameData[x][y] = deletedCells[x][y];
            }

        }
    }
}

int solveGame(int a[][9])
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            int number = a[x][y];
            a[x][y] = 0;
            if (isElementInArray(a[x], number,9) >= 0 || number == 0)
            {
                a[x][y] = number;
                return 0;
            }

            resetArray(elementsInSomeColumn, 9);
            for (int l = 0; l < sizeof(elementsInSomeColumn); l++)
            {
                elementsInSomeColumn[l] = a[l][y];
            }

            if (isElementInArray(elementsInSomeColumn, number,9) >= 0 || isElementInBox(a, x - x % 3, y - y % 3, number) >= 0)
            {
                a[x][y] = number;
                return 0;
            }
            a[x][y] = number;
        }
    }
    return 1;
}

int generateRandomNumber()
{
    return 1 + rand() % 9;
}

void resetArray(int array[], int size)
{
    for (int l = 0; l < 9; l++)
    {
        array[l] = 0;
    }
}
void resetGameData(int array[][9])
{
    for (int _x = 0; _x < 9; _x++)
    {
        for (int _y = 0; _y < 9; _y++)
        {
            array[_x][_y] = 0;
        }
    }
}

int isElementInArray(int array[], int ele, int size)
{
    for (int x = 0; x < size; x++)
    {
        if (array[x] == ele && ele != 0)
        {
            return x;
        }
    }
    return -1;
}

int isElementInBox(int arr[][9], int box_start_row, int box_start_col, int ele)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
        {

            if (arr[row + box_start_row][col + box_start_col] == ele && ele != 0)
            {
                return 1;
            }
        }
    return -1;
}

void generateGameData(int a[][9])
{
    time_t start_t, end_t;
    double diff_t;
    time(&start_t);
    resetGameData(gameData);
    srand(time(NULL));
    int steps=0;
    system("clear");
    printf("          Generieren von Spieldaten....\n");
    usleep(500000);

    for (int _x = 0; _x < 9; _x++)
    {
        for (int _y = 0; _y < 9; _y++)
        {
            steps++;
            time(&end_t);
            diff_t = difftime(end_t, start_t);
            if(diff_t > 2){
                generateGameData(gameData);
                break; // das ist der fix;
			}
            int number = generateRandomNumber();

            if (isElementInArray(a[_x], number,9) >= 0)
            { // number darf nur einmal in row vorkommen.
                _y--;
                resetArray(elementsInSomeColumn,9);
                continue;
            }

            resetArray(elementsInSomeColumn,9);
            for (int l = 0; l < 9; l++)
            {
                elementsInSomeColumn[l] = a[l][_y];
            }

            // number darf nur einmal in column und box vorkommen.
            if (isElementInArray(elementsInSomeColumn, number,9) >= 0 || isElementInBox(a, _x - _x % 3, _y - _y % 3, number) >= 0)
            {
                resetArray(a[_x],9);
                _x--;
                break;
            }
            a[_x][_y] = number;
        }
    }
    printf("\n");
}


int generateNumberByInterval(int x, int y)
{
    return x + rand() % (y - x + 1);
}

void deleteCells(int array[][9], int difficulty)
{
    for (int x = 1; x <= 3; x++)
    {
        for (int y = 1; y <= 3; y++)
        {
            int tmp = difficulty;
            while (tmp > 0)
            {
                int r = generateNumberByInterval(3 * (x - 1), 3 * x - 1);
                int c = generateNumberByInterval(3 * (y - 1), 3 * y - 1);
                if (array[r][c] > 0)
                { // not already deleted
                    deletedCells[r][c] = array[r][c];
                    userCells[r][c] = 1;
                    array[r][c] = 0;
                }
                tmp--;
            }
        }
    }
}


int getGridStatus(int array[][9])
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (array[x][y] == 0)
            {
                return NOT_FILLED;
            }
        }
    }
    return FILLED;
}


int getGameStatus(int array[][9])
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (array[x][y] == 0)
            {
                return NOT_FILLED;
            }
        }
    }
    return FILLED;
}


int timer(int action) {

    static int first = 0, paused = 0;
    static int timer = 0, zwErg = 0;

    switch(action) {
        case TIMER_STATE:
            break;
        case TIMER_PAUSE:
            if (paused == 0) {
                _pause = time(NULL);
                paused++;
            } else {
                end = time(NULL);
                zwErg += (end - _pause);
                paused--;
            }
            break;
        case TIMER_START:
            first = 1;
            zwErg = 0;
            paused = 0;
            break;
        case TIPP_USED:
            zwErg -= 15;
            break;
        case RESET_TIMER:
            printf("Timeer reseted");
            system("clear");
            first = 1;
            zwErg = 0;
            paused = 0;
            timer = 0;
            break;
        case HELP_USED:
            zwErg -= 25;
            break;
        default:
            break;
    }

    if(first) {
        start = time(NULL);
        first = 0;
    }

    end = time(NULL);

    timer = end - start;
    timer -= zwErg;

    return timer;

}

void timeToString(int userTime, char stringTime[]) {
    int seconds = userTime % 60;
    int minutes = userTime / 60;

    char s[2]={0};
    char m[2]={0};

    if(seconds < 10){
        s[0] = '0';
        s[1] = '\0';
    }

    if(minutes < 10){
        m[0] = '0';
        m[1] = '\0';
    }
    sprintf(stringTime, "%s%d:%s%d", m, minutes, s, seconds );
    stringTime[5] = '\0';
}


int checkGameSolved()
{
    if (getGameStatus(gameData) == FILLED) {
        return solveGame(gameData);
    }
    return 0;
}