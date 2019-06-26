cmake_minimum_required(VERSION 3.14)
project(sudoku C)

set(CMAKE_C_STANDARD 99)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(src)


SET(CMAKE_C_FLAGS  -pthread)
SET(CMAKE_C_FLAGS -DSQLITE_OMIT_LOAD_EXTENSION)



add_executable(sudoku src/core/game.c src/core/view.c src/services/user_service.c src/services/score_service.c libs/sqlite3.c  main.c)

target_link_libraries(${PROJECT_NAME} pthread)