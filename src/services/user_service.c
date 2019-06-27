//
// Created by karim on 25.06.19.
//
#include "../../headers/services/user_service.h"
#include "../../headers/services/connection.h"
#include <stdio.h>



char sql[200];
char *zErrMsg;
sqlite3 *connection;


int id = 0;

int callback2(void *userID, int argc, char **argv, char **azColName) {

	for (int i = 0; i < argc; i++) {
		if (strcmp(azColName[i], "last_insert_rowid()") == 0) {
			id = atoi(argv[i]);
		}
	}

	return 0;
}


int getLastUserID() {
	sprintf(sql, "SELECT last_insert_rowid()");
	int rc = sqlite3_exec(connection, sql, callback2, NULL, &zErrMsg);
	if (!rc == SQLITE_OK) {
		return -1;
	}
	else {
		return id;
	}
}

int registerUser(char username[]) {
	sprintf(sql, "INSERT INTO `User` (Name) VALUES(\"%s\");", username);
	int rc = sqlite3_exec(connection, sql, NULL, NULL, &zErrMsg);



	if (!rc == SQLITE_OK) {
		id = 0;
		return -1;
	}
	else {
		return getLastUserID();
	}
}
