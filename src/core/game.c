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

int x=0;
int y=0;
int difficulty=EASY;
char gameMessage[200] = {0};
int isGameActive;
int currentPosition=0;


int arr[9][9] = {0};
int marks[9][9][MAX_MARKS] = {0};
int deletedCells[9][9] = {0};
int userCells[9][9] = {0};

int elementsInSomeColumn[9] = {0};

time_t start, end, _pause;


void solveCell(int array[][9], int x, int y)
{
    if (deletedCells[x][y] > 0)
    {
        userCells[x][y] = 0;
        array[x][y] = deletedCells[x][y];
    }
}

void mark(int x,int y, int suggestion){
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
            if (isElementInArray(a[x], number) >= 0 || number == 0)
            { // number darf nur einmal in row vorkommen.
                a[x][y] = number;
                return 0;
            }

            resetArray(elementsInSomeColumn);
            for (int l = 0; l < sizeof(elementsInSomeColumn); l++)
            {
                elementsInSomeColumn[l] = a[l][y];
            }

            // number darf nur einmal in column und box vorkommen.
            if (isElementInArray(elementsInSomeColumn, number) >= 0 || isElementInBox(a, x - x % 3, y - y % 3, number) >= 0)
            {
                a[x][y] = number;
                ;
                return 0;
            }
            a[x][y] = number;
        }
    }
    //resetGameData(arr);
    return 1;
}

int generateRandomNumber()
{
    return 1 + rand() % 9;
}

void resetArray(int array[])
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

int isElementInArray(int array[], int ele)
{
    for (int x = 0; x < 9; x++)
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
    resetGameData(arr);
    srand(time(NULL));

    for (int _x = 0; _x < 9; _x++)
    {
        for (int _y = 0; _y < 9; _y++)
        {
            time(&end_t);
            diff_t = difftime(end_t, start_t);
            if(diff_t > 3){
                generateGameData(arr);
                break; // das ist der fix;
			}
            int number = generateRandomNumber();

            if (isElementInArray(a[_x], number) >= 0)
            { // number darf nur einmal in row vorkommen.
                _y--;
                resetArray(elementsInSomeColumn);
                continue;
            }

            resetArray(elementsInSomeColumn);
            for (int l = 0; l < 9; l++)
            {
                elementsInSomeColumn[l] = a[l][_y];
            }

            // number darf nur einmal in column und box vorkommen.
            if (isElementInArray(elementsInSomeColumn, number) >= 0 || isElementInBox(a, _x - _x % 3, _y - _y % 3, number) >= 0)
            {
                resetArray(a[_x]);
                _x--;
                break;
            }
            a[_x][_y] = number;
        }
    }
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
    static long int timer = 0, zwErg = 0;

    switch(action) {
        case 0:
            break;
        case 1:
            if (paused == 0) {
                _pause = time(NULL);
                paused++;
            } else {
                end = time(NULL);
                zwErg += (end - _pause);
                paused--;
            }
            break;
        case 2:
            first = 1;
            zwErg = 0;
            paused = 0;
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

    char s[2];
    char m[2];

    if(seconds < 10){
        s[0] = '0';
    }

    if(minutes < 10){
        m[0] = '0';
    }

    sprintf(stringTime, "%s%d:%s%d", m, minutes, s, seconds );
}


int checkGameSolved()
{
    if (getGameStatus(arr) == FILLED) {
        if (solveGame(arr) == 1) {
            currentPosition = SOLVED_GAME;
            isGameActive = 0;
            sprintf(gameMessage, "%s", "Das Spiel ist geloest");
            return 1;
        } else {
            sprintf(gameMessage, "%s", "Das Spiel ist nicht korrekt geloest");
            return 0;
        }
    }
}