#include "../../libs/sqlite3.h"


struct sScore {
    char name[10];
    int userId;
    int time;
    int difficulty;
    struct sScore *next;
};
typedef struct sScore score;

/**
 * \brief Inserts score in the database.
 *
 * @param username[]
 * @param iScore
 * @param _time
 * @param iDifficulty
 * @return Returns the id of the last user if insered, otherwise -1.
 */
int insertScore(int *piUserID, int iScore, int iDifficulty);


/**
 * \brief Retrieves score from the database.
 *
 * @param scores is an array of Score where the scores put into.
 */
void getScores(sScore *scores, int iDiff);


int getBestScoreByUserID(int iUserID);

int bestScoreCallback(void *pcBestScore, int iArgc, char **ppcArgv, char **ppcAzColName);


int getBestScore(int *piBestScore, int iDifficulty);