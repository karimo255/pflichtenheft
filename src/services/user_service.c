//
// Created by karim on 25.06.19.
//
#define _CRT_SECURE_NO_WARNINGS
#include "../../headers/services/user_service.h"
#include "../../headers/services/connection.h"
#include <stdio.h>



char sql[200];
char *zErrMsg;
sqlite3 *connection;


int registerUser(char username[]){
    sprintf(sql, "INSERT INTO `User` (Name) VALUES(\"%s\");", username);

    int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);
    printf("%s\n", sql);

    if(!rc == SQLITE_OK){
        return -1;
    } else{
        return 0;
    }
}