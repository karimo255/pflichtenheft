//
// Created by karim on 24.06.19.
//
#include <stdlib.h>
#include "../../headers/core/game.h"
#include "../../headers/shared/shared.h"
#include <stdio.h>

int x=0;
int y=0;
int difficulty=EASY;
char gameMessage[200] = {0};
int isGameActive=0;
int currentPosition=0;


int arr[9][9] = {0};
int deletedCells[9][9] = {0};
int userCells[9][9] = {0};

int elementsInSomeColumn[9] = {0};


void solveCell(int array[][9], int x, int y)
{
    if (deletedCells[x][y] > 0)
    {
        array[x][y] = deletedCells[x][y];
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
    resetGameData(arr);
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
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            array[x][y] = 0;
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
	int countOfTrys=1000;
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
			if(countOfTrys == 0){
				printf("Problem gefunden");
				resetGameData(a);
				generateGameData(a);
				countOfTrys=1000;
			}
            int number = generateRandomNumber();
            if (isElementInArray(a[x], number) >= 0)
            { // number darf nur einmal in row vorkommen.
				countOfTrys--;
                y--;
                continue;
            }

            resetArray(elementsInSomeColumn);
            for (int l = 0; l < sizeof(elementsInSomeColumn); l++)
            {
                elementsInSomeColumn[l] = a[l][y];
            }

            // number darf nur einmal in column und box vorkommen.
            if (isElementInArray(elementsInSomeColumn, number) >= 0 || isElementInBox(a, x - x % 3, y - y % 3, number) >= 0)
            {
                resetArray(a[x]);
				countOfTrys--;
                x--;
                break;
            }
			countOfTrys=1000;
            a[x][y] = number;
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