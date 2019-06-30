#include "../../libs/sqlite3.h"

/**
 * \brief Inserts user in the database.
 *
 * @param username to insert.
 * @return Returns zero if insered, otherwise -1.
 */


registerUser(char username[], int *newUserId);

int createDatabaseIfNotExist();

int createTables();


