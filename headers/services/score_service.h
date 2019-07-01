//
// Created by karim on 25.06.19.
//
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
 * @param score
 * @param _time
 * @param difficulty
 * @return Returns the id of the last user if insered, otherwise -1.
 */
int insertScore(int *userID, int score, int difficulty);


/** Abfrage der Highscores inklusive der zugehörigen Spielernamen / UserIDs
 * 1. Parameter: Zeiger auf die Struktur, in der die abgefragten Daten ge-
 * speichert werden sollen
 */
void getScores(struct sScore *scores);

/** Erfragt den Highscore des aktuellen Nutzers aus der Datenbank.
 * 1. Parameter: UserID des aktuellen Nutzers
 */
int getBestScoreByUserID(int userID);

int bestScoreCallback(void *bestScore, int argc, char **argv, char **azColName);

/** Erfragt den Highscore aller Spieler im aktuellen Schwierigkeitsgrad aus der Datenbank.
 * 1. Parameter: Zeiger auf die Variable, in die der Highscore geschrieben werden soll
 * 2. Parameter: aktueller Schwierigkeitsgrad
 */
int getBestScore(int *bestScore, int difficulty);