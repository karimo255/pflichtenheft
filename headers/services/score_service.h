//
// Created by karim on 25.06.19.
//
#include "../../libs/sqlite3.h"


struct score {
    char name[30];
    int userId;
    int time;
    int difficulty;
    struct score *next;
};
typedef struct score score;

/**
 * \brief Inserts score in the database.
 *
 * @param username[]
 * @param score
 * @param _time
 * @param difficulty
 * @return Returns the id of the last user if insered, otherwise -1.
 */
int insertScore(int *userID, int score, int difficulty);


/**
 * \brief Retrieves score from the database.
 *
 * @param scores is an array of Score where the scores put into.
 */
void getScores(score *scores);


int getBestScoreByUserID(int userID);