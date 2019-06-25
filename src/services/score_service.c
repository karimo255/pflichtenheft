//
// Created by karim on 25.06.19.
//



#include "../../headers/services/score_service.h"
#include "../../headers/services/connection.h"
#include <stdio.h>



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

int callback(null, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
//   *(int*) userId = 1;
    return 0;
}

void getScores(){
    sprintf(sql, "SELECT * FROM `Score` LIMIT 10;");

    int rc = sqlite3_exec(connection, sql, callback, NULL, &zErrMsg);
    printf("%s\n", sql);

    if(!rc == SQLITE_OK){
        return -1;
    } else{
        return 0;
    }
}



