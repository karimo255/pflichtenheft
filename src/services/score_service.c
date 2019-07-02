//
// Created by team-name on 25.06.19.
//

#include "../../headers/services/score_service.h"
#include "../../headers/services/connection.h"
#include "../../headers/shared/shared.h"
#include "../../headers/core/view.h"
#include "../../headers/core/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char sql[200];
char *zErrMsg;
sqlite3 *psqlConnection;
int user_id = 0;

int insertScore(int *userID, int score, int difficulty)
{
    sprintf(sql, "INSERT INTO `Score` (time, userId, difficulty) VALUES(\"%d\", \"%d\", \"%d\");", score,
            *userID, difficulty);

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, sql, NULL, NULL, &zErrMsg);
    printf("%s\n", sql);

    if (!rc == SQLITE_OK)
    {
        strcpy(cGameMessage, "Score konnte nicht gespeichert werden.");
        return 1;
    }
    else
    {
        return 0;
    }
}

int getScoresCallback(void *scores, int argc, char **argv, char **azColName)
{
    struct sScore *a_head = (struct sScore *)scores;
    if (argc <= 0)
    {
        return 0;
    }

    /**
    * We go to end of the list
    */
    while (a_head->next != NULL)
    {
        a_head = a_head->next;
    }

    /**
    * now we can add a new item
    */
    a_head->next = malloc(sizeof(struct sScore));

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(azColName[i], "time") == 0)
        {
            a_head->next->time = atoi(argv[i]);
        }
        if (strcmp(azColName[i], "name") == 0)
        {
            strcpy(a_head->next->name, argv[i]);
        }
        else if (strcmp(azColName[i], "userId") == 0)
        {
            a_head->next->userId = atoi(argv[i]);
        }
        else if (strcmp(azColName[i], "difficulty") == 0)
        {
            a_head->next->difficulty = atoi(argv[i]);
        }
    }

    a_head->next->next = NULL;
    return 0;
}

void getScores(struct sScore *scores)
{
    sprintf(sql,
            "SELECT `Score`.`userId`, `User`.`name`, `Score`.`time`, `Score`.`difficulty` FROM `Score` INNER JOIN `User` ON `Score`.`userId` = `User`.`id` ORDER BY `Score`.`time` ASC LIMIT 10;");
    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, sql, getScoresCallback, scores, &zErrMsg);

    if (scores->next != NULL)
    {
        // deleteNode(scores);
    }
    if (rc != SQLITE_OK)
    {
        strcpy(cGameMessage, "Bestscores Abfrage fehlgeschlagen.");
    }
}

int bestScoresCallBack(void *scores, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(azColName[i], "time") == 0)
        {
            user_id = atoi(argv[i]);
        }
    }
    return 0;
}

int getBestScoreByUserID(int userID)
{
    sprintf(sql, "SELECT time FROM `Score` where userId = %d limit 1 sort by time;", userID);
    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, sql, bestScoresCallBack, NULL, &zErrMsg);
    return user_id;
}

int bestScoreCallback(void *bestScore, int argc, char **argv, char **azColName)
{
    int *tmp = (int *)bestScore;
    if (argc <= 0)
    {
        *tmp = 0;
        return 0;
    }
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(azColName[i], "time") == 0)
        {
            *tmp = atoi(argv[i]);
        }
    }
    return 0;
}

int getBestScore(int *bestScore, int difficulty)
{
    sprintf(sql, "SELECT time FROM `Score`  WHERE  difficulty=%d ORDER by time desc  LIMIT 1", difficulty);
    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, sql, bestScoreCallback, bestScore, &zErrMsg);

    if (!rc == SQLITE_OK)
    {
        printf(sql);
        strcpy(cGameMessage, "Bestscore Abfrage fehlgeschlagen2.");
        return 1;
    }
    else
    {
        return 0;
    }
}
