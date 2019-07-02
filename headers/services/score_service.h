/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden im Wesentlichen alle für das Spiel
 * nötigen Berechnungen durchgeführt. Z.B. das Generieren des Spiel-
 * feldes oder der Timer, wie lange ein Spieler für das Lösen
 * ein Spiel benötigt.
*/

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


/** Abfrage der Highscores inklusive der zugehörigen Spielernamen / UserIDs
 * 1. Parameter: Zeiger auf die Struktur, in der die abgefragten Daten ge-
 * speichert werden sollen
 */
void getScores(struct sScore *scores);

/** Erfragt den Highscore des aktuellen Nutzers aus der Datenbank.
 * 1. Parameter: UserID des aktuellen Nutzers
 */
int getBestScoreByUserID(int userID);

int bestScoreCallback(void *pvBestScore, int iArgc, char **ppcArgv, char **ppcAzColName);

/** Erfragt den Highscore aller Spieler im aktuellen Schwierigkeitsgrad aus der Datenbank.
 * 1. Parameter: Zeiger auf die Variable, in die der Highscore geschrieben werden soll
 * 2. Parameter: aktueller Schwierigkeitsgrad
 */
int getBestScore(int *piBestScore, int iDifficulty);