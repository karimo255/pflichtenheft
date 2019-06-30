//
// Created by team-name on 25.06.19.
//

#include "../../headers/services/score_service.h"
#include "../../headers/services/connection.h"
#include "../../headers/shared/shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char sql[200];
char *zErrMsg;
sqlite3 *connection;
int user_id = 0;


int insertScore(int *userID, int score, int difficulty) {
    sprintf(sql, "INSERT INTO `Score` (time, userId, difficulty) VALUES(\"%d\", \"%d\", \"%d\");", score,
            *userID, difficulty);

    int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);
    printf("%s\n", sql);

    if (!rc == SQLITE_OK) {
        return -1;
    } else {
        return 0;
    }
}

delete_whole_list(struct score *scores) {
    score *temp;
    while (scores->next != NULL) // skip first element
    {
        temp = scores->next;
        scores = scores->next;
        free(temp);
    }
}


int getScoresCallback(void *scores, int argc, char **argv, char **azColName) {
    score *a_head = (score *) scores;

    /**
    * We go to end of the list
    */
    while (a_head->next != NULL) {
        a_head = a_head->next;
    }

    /**
    * now we can add a new item
    */
    a_head->next = malloc(sizeof(score));

    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "time") == 0) {
            a_head->next->time = atoi(argv[i]);
        }
        if (strcmp(azColName[i], "name") == 0) {
            strcpy(a_head->next->name, argv[i]);
        } else if (strcmp(azColName[i], "userId") == 0) {
            a_head->next->userId = atoi(argv[i]);
        } else if (strcmp(azColName[i], "difficulty") == 0) {
            a_head->next->difficulty = atoi(argv[i]);
        }
    }

    a_head->next->next = NULL;
    return 0;
}

void getScores(score *scores, int diff) {
    delete_whole_list(scores);
    sprintf(sql,
            "SELECT `Score`.`userId`, `User`.`name`, `Score`.`time` FROM `Score` INNER JOIN `User` ON `Score`.`userId` = `User`.`id` WHERE `Score`.`difficulty` = %d ORDER BY `Score`.`time` DESC LIMIT 10;",
            diff);
    int rc = sqlite3_exec(connection, sql, getScoresCallback, scores, &zErrMsg);
    if (rc == SQLITE_OK) {
        printf("OK\n");
    } else {
        printf("NO\n");
    }
}

int bestScoresCallBack(void *scores, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "time") == 0) {
            user_id = atoi(argv[i]);
        }
    }
    return 0;
}

int getBestScoreByUserID(int userID) {
    sprintf(sql, "SELECT time FROM `Score` WHERE userId = %d LIMIT 1 SORT BY time DESC;", userID);
    int rc = sqlite3_exec(connection, sql, bestScoresCallBack, NULL, &zErrMsg);
    printf("%s\n", sql);
    return user_id;
}

int bestScoreCallback(void *bestScore, int argc, char **argv, char **azColName) {
    int *tmp = (int *) bestScore;
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "time") == 0) {
            *tmp = atoi(argv[i]);
        }
    }
    return 0;
}


int getBestScore(int *bestScore,int difficulty) {
    sprintf(sql, "SELECT time FROM `Score`  WHERE  difficulty=%d ORDER by time desc  LIMIT 1", difficulty);
    int rc = sqlite3_exec(connection, sql, bestScoreCallback, bestScore, &zErrMsg);
    if (!rc == SQLITE_OK) {
        return -1;
    } else {
        return 0;
    }
}

