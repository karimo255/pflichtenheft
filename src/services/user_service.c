//
// Created by karim on 25.06.19.
//
#include "../../headers/services/user_service.h"
#include "../../headers/services/connection.h"
#include "../../headers/core/view.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char sql[2000];
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
    fflush(stdout);
    clear_output();
	int rc = sqlite3_exec(connection, sql, lastInsertIdCallBack, newUserId, &zErrMsg);
	if (!rc == SQLITE_OK) {
        return -1;
    } else {
        return 0;
    }
}

int registerUser(char username[], char password[6], int *newUserId) {
    sprintf(sql, "INSERT INTO `User` (name, password) VALUES(\"%s\", \"%s\");", username, password);
    int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);

    if (!rc == SQLITE_OK) {
        return -1;
    } else {
        return getLastInsertId(newUserId);
    }
}

int loginUserCallback(void *userID, int argc, char **argv, char **azColName) {
    int *_id = (int *) userID;
    if(argc <= 0) {
        *_id = 0;
        return 0;
    }
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "id") == 0) {
            *_id = atoi(argv[i]);
            printf("ja\n");
        }
    }
    return 0;
}

void loginUser(char username[], char password[], int *id) {
    sprintf(sql, "SELECT * FROM `User` WHERE name =\"%s\" AND password = \"%s\";", username, password);
    printf("sql: %s\n",sql);
    int rc = sqlite3_exec(connection, sql, loginUserCallback, id, &zErrMsg);
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

void getUserID(char username[30], int *userID) {
    sprintf(sql, "SELECT id FROM User WHERE name=\"%s\"  LIMIT 1", username);
    int rc = sqlite3_exec(connection, sql, getUserIdCallback, userID, &zErrMsg);
}



int createUserTable() {
    sprintf(sql, "CREATE TABLE \"User\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` TEXT NOT NULL, `password` TEXT )");
    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);
    if (!rc == SQLITE_OK) {
        return 1;
    }
    else {
        return 0;
    }
}

int createScoreTable() {
    sprintf(sql, "CREATE TABLE \"Score\" ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `userId` INTEGER NOT NULL, `difficulty` INTEGER NOT NULL, `time` INTEGER, FOREIGN KEY(`userId`) REFERENCES `User`(`id`) )");
    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);
    if (!rc == SQLITE_OK) {
        return 1;
    }
    else {
        return 0;
    }
}