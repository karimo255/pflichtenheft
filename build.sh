#!/usr/bin/env bash

gcc main.c src/core/game.c src/core/view.c src/services/user_service.c src/services/score_service.c src/core/inputHandler.c libs/sqlite3.c src/core/getch.c  -lpthread -DSQLITE_OMIT_LOAD_EXTENSION -o ./bin/sudoku && ./bin/sudoku
