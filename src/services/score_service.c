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


int insertScore(int userID, int score, int difficulty) {
    sprintf(sql, "INSERT INTO `Score` (Score, UserID, Schwierigkeitsgrad) VALUES(\"%d\", \"%d\", \"%d\");", score,
            userID, difficulty);

    int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);
    printf("%s\n", sql);

    if (!rc == SQLITE_OK) {
        return -1;
    } else {
        return 0;
    }
}

void deleteNode(score *node) {
    score *temp = node->next;
    node->userID = node->next->userID;
    printf("drei\n");
    strcpy(node->name, node->next->name);
    printf("%s\n", node->name);
    node->time = node->next->time;
    printf("f<C3><BC>nf\n");
    node->next = temp->next;
    free(temp);
}

int user_id = 0;
int callback3(void *scores, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
<<<<<<< HEAD
        if (strcmp(azColName[i], "Score") == 0) {
           user_id = atoi(argv[i]);
        } 
=======
        if (strcmp(azColName[i], "UserID") == 0) {
            a_head->next->userID = atoi(argv[i]);
        } else if (strcmp(azColName[i], "Name") == 0) {
            printf("%s", argv[i]);
            strcpy(a_head->next->name, argv[i]);
        } else if (strcmp(azColName[i], "Score") == 0) {
            a_head->next->time = atoi(argv[i]);
        }
>>>>>>> clion
    }
    return 0;
}

int callback(void *scores, int argc, char **argv, char **azColName) {
	score *a_head = (score *)scores;

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
		if (strcmp(azColName[i], "ScoreID") == 0) {
			a_head->next->scoreID = atoi(argv[i]);
		}
		else if (strcmp(azColName[i], "Score") == 0) {
			a_head->next->score = atoi(argv[i]);
		}
		else if (strcmp(azColName[i], "UserID") == 0) {
			a_head->next->userID = atoi(argv[i]);
		}
		else if (strcmp(azColName[i], "Schwierigkeitsgrad") == 0) {
			a_head->next->difficulty = atoi(argv[i]);
		}
	}

	a_head->next->next = NULL;
	return 0;
}

void getScores(score *scores) {
    sprintf(sql, "SELECT `Score`.`UserID`, `User`.`Name`, `Score`.`Score` FROM `Score` INNER JOIN `User` ON `Score`.`UserID` = `User`.`UserID` WHERE `Score`.`Schwierigkeitsgrad` = %i ORDER BY `Score`.`Score` DESC LIMIT 10;", EASY);
    int rc = sqlite3_exec(connection, sql, callback, scores, &zErrMsg);
    deleteNode(scores);
    printf("%s\n", sql);
}


int getScoreByUserID(int userID) {
	sprintf(sql, "SELECT * FROM `Score` where UserID=%d;", userID);
	int rc = sqlite3_exec(connection, sql, callback3, NULL, &zErrMsg);
	printf("%s\n", sql);
	return user_id;
}



