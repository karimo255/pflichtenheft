#include "../../libs/sqlite3.h"

/**
 * \brief Inserts user in the database.
 *
 * @param username to insert.
 * @return Returns zero if insered, otherwise -1.
 */

int registerUser(char username[], char password[], int *newUserId);

void loginUser(char username[], char password[], int*id);
int createUserTable();

int createScoreTable();

int getUserIdCallback(void *userID, int argc, char **argv, char **azColName);

void getUserID(char username[30], int *userID);