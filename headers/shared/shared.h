/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden im Wesentlichen alle für das Spiel
 * nötigen Berechnungen durchgeführt. Z.B. das Generieren des Spiel-
 * feldes oder der Timer, wie lange ein Spieler für das Lösen
 * ein Spiel benötigt.
*/

#ifndef SUDOKU_SHARED_H
#define SUDOKU_SHARED_H
#define MAX_MARKS 3

#ifdef __WIN32__
#include <Windows.h>
#endif

/// Difficulty enum
extern enum DIFFICULTY ///< max. deleted cells per box
{
    EASY = 5, ///< Repräsentiert Schwierigkeitsstufe einfach
    MEDIUM = 7, ///< Repräsentiert Schwierigkeitsstufe mittel
    HARD = 8 ///< Repräsentiert Schwierigkeitsstufe schwer
};

extern enum ACTIONS {
    TIMER_STATE = 0,  /// ///< Aktion um Timer Status zu holen
    TIMER_START = 1, ///< Aktion um Timer zu starten
    TIMER_PAUSE = 2,  ///< Aktion um Timer zu pausieren
    TIPP_USED = 3,  ///< Aktion wenn  Tipp benutzt wird
    HELP_USED, ///< Aktion wenn  Hilfe benutzt wird
    RESET_TIMER, ///< Aktion um den Timmer zurückzusetzen
};

/// Positions enum
extern enum POSITIONS {
    MENU = 0, ///< MENU
    IN_GAME = 1, ///< IN_GAME
    DIFFICULTY_DIALOG = 2, ///< DIFFICULTY_DIALOG
    DETAILS = 3, ///< DETAILS
    HELP = 4, ///< HELP
    USER_NAME = 5, ///< USER_NAME
    DETAILS_DIALOG = 6, ///< DETAILS_DIALOG
    SOLVED_GAME = 7, ///< SOLVED_GAME
    SET_MARK = 8, ///< SET_MARK
    SET_PASSWORD = 9, ///< SET_PASSWORD
    ENTER_PASSWORD = 10, ///< ENTER_PASSWORD
};

/// Grid-Status enum
enum GRID_STATUS {
    FILLED = 1, ///< Beschreibt den Staus von Sudoku
    NOT_FILLED = 0 ///< Beschreibt den Staus von Sudoku
};

/// Arrows enum
enum ARROWS {
    UP = 72, ///< UP
    DOWN = 80, ///< DOWN
    LEFT = 75, ///< LEFT
    RIGHT = 77, ///< RIGHT
    UP_LINUX = 65, ///< RIGHT ON UNIX
    DOWN_LINUX = 66, ///< DOWN ON UNIX
    RIGHT_LINUX = 67, ///< DOWN ON UNIX
    LEFT_LINUX = 68, ///< DOWN ON UNIX
};
#endif //SUDOKU_SHARED_H






	
