//
// Created by karim on 24.06.19.
//

#ifndef SUDOKU_SHARED_H
#define SUDOKU_SHARED_H

#endif //SUDOKU_SHARED_H

#ifdef __WIN32__ || _MSC_VER || __MS_DOS__
#include <Windows.h>
#endif

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

#ifdef __WIN32__ || _MSC_VER || __MS_DOS__
s#define KRED  1///< color red
#define KGRN  20 ///< color green
#define KYEL  22 ///< color yellow
#define KBLU  44 ///< color blue
#define KMAG  55 ///< color magenta
#define KCYN  89 ///< color cay
#define KWHT  255 ///< color white
#endif

#ifdef __unix__
#define KRED  "\x1B[31m" ///< color red
#define KGRN  "\x1B[32m" ///< color green
#define KYEL  "\x1B[33m" ///< color yellow
#define KBLU  "\x1B[34m" ///< color blue
#define KMAG  "\x1B[35m" ///< color magenta
#define KCYN  "\x1B[36m" ///< color cay
#define KWHT "\x1B[37m" ///< color white
#endif
	
