#include "../../libs/sqlite3.h"

/**
 * \brief Inserts user in the database.
 *
 * @param cUsername to insert.
 * @return Returns zero if insered, otherwise -1.
 */

int registerUser(char cUsername[], char cPassword[], int *piNewUserId);

void loginUser(char cUsername[], char cPassword[], int*pId);
int createUserTable();

int createScoreTable();

int getUserIdCallback(void *pvUserID, int iArgc, char **ppcArgv, char **ppcAzColName);

void getUserID(char cUsername[30], int *piUserID);