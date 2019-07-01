//
// Created by karim on 24.06.19.
//

#include "../shared/shared.h"

#ifndef SUDOKU_VIEW_H
#define SUDOKU_VIEW_H

#endif //SUDOKU_VIEW_H

void renderMarkModeMessage();
void renderUsernameDialog(char *pcUsername);

/**
 * \brief function to render menu
 *
 * It show the game menu in the console
 */
void renderMenu();


void renderDBestScoreDialog();
/**
 * \brief Function to render best-score scrren
 *
 * It shows infos about users and scores
 */
void renderDetails(struct sScore *scores, int iDifficulty);

/**
 * \brief Function to render the game grid
 *
 * It shows the game grid
 */
void renderCourt(int iGameData[][9],int iUserCells[][9], int iX_coordinate, int iY_coordinate);
/**
 * \brief Function to render infos about current session
 *
 * It shows the current session information
 */
void renderInfoBox(char *pcUsername, int *piScore, int _piDifficulty, int iRemaining);

/**
 * \brief Function to render the game menu
 *
 * It shows the game menu
 */
void renderGameMenu();

/**
 * \brief Function to render the dialog after solved game
 *
 * It shows the game menu
 */
void renderSolvedGame(int iSolvedAutomatic, int iAnzahlDerTipps, int iAnzahlDerHilfe);

/**
 * \brief Function to render the difficult settings dialog
 *
 * It shows difficult dialog
 */
void renderDifficultyDialog();

/**
 * \brief Function to render the help dialog
 *
 * It shows help dialog
 */
void renderHelpDialog();

/**
 * \brief Function to get the length of integers (aka strlen for strings)
 *
 * @param iX an integer to get length of
 * @return the length of the given integer
 *
 * It return the length of integer number (for optimal printing)
 */
int lenHelper(int iX);

/**
* \brief Get count of not filled cells
*
* \param iGameData gameData
* \return count of not filled cells
*/

int getRemainingCells(int iGameData[][9]);

void print_list(struct sScore *head,int iDifficulty);


void initColors();





void printColoredString(char cText[], char cColor[], int iNewLine);
void printColoredNumber(int iNumber, char *pcColor, int iNewLine);
void setPrintingColor(char *pcColor);

void printStartOfLine();
void printEndOfLine();
void printTableLine();
void printEndOfTable();
void printEmptyTableLine();

void renderLoadingBar(int x);

void clear_output();


void renderNotesBox(int iX,int iY);

void renderSetPassword();

void renderEnterPassword();