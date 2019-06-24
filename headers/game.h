//
// Created by karim on 24.06.19.
//


extern int x;
extern int y;
extern int difficulty;
extern char gameMessage[200];
extern int isGameActive;
extern int currentPosition;


extern int arr[9][9];
extern int deletedCells[9][9];
extern int userCells[9][9];


int isElementInArray(int array[], int ele);
void resetArray(int array[]);
void generateGameData(int array[][9]);
void deleteCells(int array[][9], int difficulty);
void navigateTo(int pos);
int isElementInBox(int arr[][9], int box_start_row, int box_start_col, int ele);
int generateRandomNumber();
void handleUserInput();
int solveGame(int arr[][9]);
void resetGameData(int arr[][9]);
int generateNumberByInterval(int x, int y);
int getGameStatus(int array[][9]);
void solveCell(int array[][9], int x, int y);