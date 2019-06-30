//
// Created by karim on 25.06.19.
//
#include "../../headers/services/user_service.h"
#include "../../headers/services/connection.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char sql[200];
char *zErrMsg;
sqlite3 *connection;


int lastInsertIdCallBack(void *userID, int argc, char **argv, char **azColName) {
    int *tmp = (int *) userID;
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "last_insert_rowid()") == 0) {
            *tmp = atoi(argv[i]);
        }
    }
    return 0;
}


int getLastInsertId(int *newUserId) {
    sprintf(sql, "SELECT last_insert_rowid()");
    int rc = sqlite3_exec(connection, sql, lastInsertIdCallBack, newUserId, &zErrMsg);
    if (!rc == SQLITE_OK) {
        return -1;
    } else {
        return 0;
    }
}

int registerUser(char username[], char password[8],  int *newUserId) {
    sprintf(sql, "INSERT INTO `User` (name, password) VALUES(\"%s\", \"%s\");", username, password);
    int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);

    if (!rc == SQLITE_OK) {
        return -1;
    } else {
        return getLastInsertId(newUserId);
    }
}


int getUserIdCallback(void *userID, int argc, char **argv, char **azColName) {
    int *tmp = (int *) userID;
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "id") == 0) {
            *tmp = atoi(argv[i]);
            printf("ja\n");
        }
    }
    return 0;
}

void getUserID(char username[30], int *userID){
    sprintf(sql, "SELECT id FROM User WHERE name=\"%s\"  LIMIT 1", username);
    printf(sql);
    int rc = sqlite3_exec(connection, sql, getUserIdCallback, userID, &zErrMsg);
}
