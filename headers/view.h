//
// Created by karim on 24.06.19.
//

#ifndef SUDOKU_VIEW_H
#define SUDOKU_VIEW_H

#endif //SUDOKU_VIEW_H

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT "\x1B[37m"

void renderMenu();
void renderDetails();
void renderCourt();
void renderInfoBox();
void renderGameMenu();
void renderDifficultyDialog();
int lenHelper(int x);

/**
* \brief Get count of not filled cells
*
* \param array gameData
* \return count of not filled cells
*/
int getRemainingCells(int array[][9]);
