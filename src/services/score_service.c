//
// Created by karim on 25.06.19.
//



#include "../../headers/services/score_service.h"
#include "../../headers/services/connection.h"
#include <stdio.h>
#include <stdlib.h>


char sql[200];
char *zErrMsg;
sqlite3 *connection;


int insertScore(int userID, int score, int difficulty){
    sprintf(sql, "INSERT INTO `Score` (Score, UserID, Schwierigkeitsgrad) VALUES(\"%d\", \"%d\", \"%d\");", score, userID, difficulty );

    int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);
    printf("%s\n", sql);

    if(!rc == SQLITE_OK){
        return -1;
    } else{
        return 0;
    }
}
void print_list(struct score * head) {
    struct score * current = head;

    while (current != NULL) {
        printf("fff %d\n", current->score);
        current = current->next;
    }
}

int callback(void *scores, int argc, char **argv, char **azColName) {
    score * a_tail = NULL;
    score * a_head = (score *)scores ;
    int i;

    a_head->score = 1;

    printf("\n");
//   *(int*) userId = 1;
    return 0;
}

void getScores(score *scores){
    sprintf(sql, "SELECT * FROM `Score` LIMIT 9;");

    int rc = sqlite3_exec(connection, sql, callback, scores, &zErrMsg);
    printf("ddd %d\n", scores->score);

    printf("%s\n", sql);

    if(!rc == SQLITE_OK){


    } else{


    }
}



