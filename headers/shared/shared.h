//
// Created by karim on 24.06.19.
//

#ifndef SUDOKU_SHARED_H
#define SUDOKU_SHARED_H

#endif //SUDOKU_SHARED_H

/// Difficulty enum
enum DIFFICULTY
{
    EASY = 4, ///< EASY
    MEDIUM = 6, ///< MEDIUM
    HARD = 7 ///< HARD
};

/// Positions enum
enum POSITIONS
{
    MENU = 0, ///< MENU
    IN_GAME = 1, ///< IN_GAME
    DIFFICULTY_DIALOG = 2, ///< DIFFICULTY_DIALOG
    DETAILS = 3, ///< DETAILS
    HELP = 4 ///< DETAILS
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
	
