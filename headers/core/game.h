
//
// Created by karim on 24.06.19.
//
#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H


#include <headers/shared/shared.h>

extern int marks[9][9][MAX_MARKS];


/// x-coordinate of the cursor.
extern int x_coordinate;

/// y-coordinate of the cursor.
extern int y_coordinate;

/// game difficulty.
extern int difficulty;

/// game message
extern char gameMessage[200];

/// is a session active.
extern int isGameActive;

/// current active menu item.
extern int currentPosition;

/// array which holds the game data.
extern int gameData[9][9];

/// array which holds deleted cells to keep track of them.
extern int deletedCells[9][9];

/// array which holds the coordinates of user cells (cells to solve) to keep track of theme.
extern int userCells[9][9];

/**
 * @param array int array to search.
 * @param number is the integer to search for.
 * @return Returns the index of the number if number is found in the array, -1 otherwise.
 *
 * Checks if a number exists in an array.
 */
int isElementInArray(int array[], int number);

/**
 * @param array int array to clear.
 *
 * Sets the value of array items to zero.
 */
void resetArray(int array[]);

/**
 * \brief Function to generate the game data.
 *
 * @param array where we put the randomly generated data into.
 *
 * It generates a completed sudoku game.
 */
void generateGameData(int array[][9]);

/**
 * \brief Function to delete grid cells randomly.
 *
 * @param array to keep track of deleted grid cells.
 * @param difficulty is an integer which defines how difficult the game should be.
 *
 * It randomly deletes cells by difficulty level.
 */
void deleteCells(int array[][9], int difficulty);

/**
 * @param direction of where to navigate
 *
 * Moves the cursor in the desired direction
 */
void navigateTo(int direction);

void mark(int x,int y, int suggestion);
/**
 * @param array[][] which holds the game data.
 * @param box_start_row Where does the row start from.
 * @param box_start_col Where does the column start from.
 * @param number is the integer to search for.
 * @return Returns the x-coordinate of the number if number is found in the array, -1 otherwise.
 *
 * Checks if a number exists in an array
 */
int isElementInBox(int array[][9], int box_start_row, int box_start_col, int number);

/**
 * @return randomly generated number between 0-9
 *
 * Generates randomly generated number
 */
int generateRandomNumber();

/**
 * It handles user input
 */
void handleUserInput();

/**
 *
 * @param array[][] which holds the game data.
 * @return Return 1 if the game successfully solved, otherwise 0.
 */
int solveGame(int array[][9]);

/**
 * @param array[][] which holds the game data.
 *
 * Sets the value of array items to zero.
 */
void resetGameData(int array[][9]);

/**
 * @return Returns randomly generated number by interval
 *
 *  Generate randomly generated number by interval
 */
int generateNumberByInterval(int x, int y);

/**
 * @param array[][] which holds the game data.
 * @return Returns a integer which indicates if the grid if filled complete.
 *
 *  Checks if the grid if filled complete.
 */
int getGridStatus(int array[][9]);

/**
 * @param array[][] which holds the game data.
 * @param x is the x-coordinate of the cell to solve.
 * @param y ist the y-coordinate of the cell to solve.
 *
 *  Checks if the grid if filled complete.
 */
void solveCell(int array[][9], int x, int y);

int getGameStatus(int array[][9]);


void solveAll(int gameData[][9], int deletedCells[][9]);

int timer(int action);

void timeToString(int userTime, char stringTime[]);

int checkGameSolved();

#endif //SUDOKU_GAME_H
