cmake_minimum_required(VERSION 3.14)
project(sudoku C)

set(CMAKE_C_STANDARD 99)

add_executable(sudoku main.c headers/view.h src/view.c src/game.c headers/game.h headers/shared.h)