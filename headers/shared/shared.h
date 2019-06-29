//
// Created by karim on 24.06.19.
//

#ifndef SUDOKU_SHARED_H
#define SUDOKU_SHARED_H
#define MAX_MARKS 3

#ifdef __WIN32__
#include <Windows.h>
#endif

/// Difficulty enum
extern enum DIFFICULTY ///< max. deleted cells per box
{
    EASY = 5, ///< EASY
    MEDIUM = 7, ///< MEDIUM
    HARD = 8 ///< HARD
};

/// Positions enum
extern enum POSITIONS
{
    MENU = 0, ///< MENU
    IN_GAME = 1, ///< IN_GAME
    DIFFICULTY_DIALOG = 2, ///< DIFFICULTY_DIALOG
    DETAILS = 3, ///< DETAILS
    HELP = 4, ///< DETAILS
    USER_NAME = 5,
    DETAILS_DIALOG = 6,
    SOLVED_GAME = 7, ///< SOLVED_GAME
    SET_MARK = 8,
};

/// Grid-Status enum
enum GRID_STATUS
{
    FILLED, ///< DETAILS
    NOT_FILLED ///< DETAILS
};

/// Arrows enum
enum ARROWS{
    UP=72, ///< UP
    DOWN = 80, ///< DOWN
    LEFT=75, ///< LEFT
    RIGHT=77, ///< RIGHT
    UP_LINUX=65, ///< RIGHT ON UNIX
    DOWN_LINUX = 66, ///< DOWN ON UNIX
    RIGHT_LINUX = 67, ///< DOWN ON UNIX
    LEFT_LINUX = 68, ///< DOWN ON UNIX
};
#endif //SUDOKU_SHARED_H






	
