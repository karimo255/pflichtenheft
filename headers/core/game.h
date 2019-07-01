#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H


#include "../shared/shared.h"


extern int iMarks[9][9][MAX_MARKS];

void fillNotesForCell(int iX_coordinate, int iY_coordinate);

/// x-coordinate of the cursor.
extern int iX_coordinate;

/// y-coordinate of the cursor.
extern int iY_coordinate;

/// game difficulty.
extern int iDifficulty;

/// game message
extern char cGameMessage[200];

/// is a session active.
extern int iIsGameActive;

/// current active menu item.
extern int iCurrentPosition;

/// array which holds the game data.
extern int iGameData[9][9];

/// array which holds deleted cells to keep track of them.
extern int iDeletedCells[9][9];

/// array which holds the coordinates of user cells (cells to solve) to keep track of theme.
extern int iUserCells[9][9];

extern int iAnzahlDerTipps;
extern int iAnzahlDerHilfe;


extern int iErlaubteAnzahlDerTipps;
extern int iErlaubteAnzahlDerHilfe;

void fillNotesForCell(int iX_coordinate, int iY_coordinate);


/**
 * @param iArray int array to search.
 * @param iElement is the integer to search for.
 * @return Returns the index of the number if number is found in the array, -1 otherwise.
 *
 * Checks if a number exists in an array.
 */
int isElementInArray(int iArray[], int iElement, int iSize);

/**
 * @param iArray int array to clear.
 *
 * Sets the value of array items to zero.
 */
void resetArray(int iArray[], int iSize);

/**
 * \brief Function to generate the game data.
 *
 * @param iGameData where we put the randomly generated data into.
 *
 * It generates a completed sudoku game.
 */
void generateGameData(int iGameData[][9]);

/**
 * \brief Function to delete grid cells randomly.
 *
 * @param iGameData to keep track of deleted grid cells.
 * @param iDifficulty is an integer which defines how difficult the game should be.
 *
 * It randomly deletes cells by difficulty level.
 */
void deleteCells(int iGameData[][9], int iDifficulty);

/**
 * @param iPos of where to navigate
 *
 * Moves the cursor in the desired direction
 */
void navigateTo(int iPos);

void makeNote(int iX, int iY, int iSuggestion);
/**
 * @param iGameData[][] which holds the game data.
 * @param iBoxStartRow Where does the row start from.
 * @param iBox_Start_Col Where does the column start from.
 * @param iElement is the integer to search for.
 * @return Returns the x-coordinate of the number if number is found in the array, -1 otherwise.
 *
 * Checks if a number exists in an array
 */
int isElementInBox(int iGameData[][9], int iBoxStartRow, int iBox_Start_Col, int iElement);

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
 * @param iGameData[][] which holds the game data.
 * @return Return 1 if the game successfully solved, otherwise 0.
 */
int solveGame(int iGameData[][9]);

/**
 * @param iGameData[][] which holds the game data.
 *
 * Sets the value of array items to zero.
 */
void resetGameData(int iGameData[][9]);

/**
 * @return Returns randomly generated number by interval
 *
 *  Generate randomly generated number by interval
 */
int generateNumberByInterval(int iX, int iY);

/**
 * @param array[][] which holds the game data.
 * @return Returns a integer which indicates if the grid if filled complete.
 *
 *  Checks if the grid if filled complete.
 */
int getGridStatus(int array[][9]);

/**
 * @param iGameData[][] which holds the game data.
 * @param iX is the x-coordinate of the cell to solve.
 * @param iY ist the y-coordinate of the cell to solve.
 *
 *  Checks if the grid if filled complete.
 */
void solveCell(int iGameData[][9], int iX, int iY);

int getGameStatus(int iGameData[][9]);


void solveAll(int iGameData[][9], int iDeletedCells[][9]);

int timer(int iAction);

void timeToString(int iUserTime, char cStringTime[]);

int checkGameSolved();

#endif //SUDOKU_GAME_H
