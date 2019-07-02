/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: user_service.c
 * Datum: 25.06.19
 * Beschreibung: In dieser Datei befinden sich alle Funktionen, die
 * etwas mit den Abfragen an die Datenbank zu tun haben, wenn es
 * um die Spieler und deren Daten geht.
*/

#include "../../headers/services/user_service.h"
#include "../../headers/services/connection.h"
#include "../../headers/core/view.h"
#include "../../headers/core/game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char cSql[2000];
char *pcZErrMsg;

sqlite3 *psqlConnection;

int lastInsertIdCallBack(void *pvUserID, int iArgc, char **ppcArgv, char **ppcAzColName)
{
    int *piTmp = (int *)pvUserID;

    for (int i = 0; i < iArgc; i++)
    {
        if (strcmp(ppcAzColName[i], "last_insert_rowid()") == 0)
        {
            *piTmp = atoi(ppcArgv[i]);
        }
    }

    return 0;
}

int getLastInsertId(int *piNewUserId)
{
    sprintf(cSql, "SELECT last_insert_rowid()");

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, lastInsertIdCallBack, piNewUserId, &pcZErrMsg);
    if (!rc == SQLITE_OK)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int registerUser(char cUsername[], char cPassword[6], int *piNewUserId)
/* Registriert einen neuen Nutzer mit seinem von ihm gesetzten Passwort in
 * der Datenbank.
 * 1. Parameter: eingegebener Nutzername
 * 2. Parameter: eingegebenes Passwort
 * 3. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
{
    sprintf(cSql, "INSERT INTO `User` (name, password) VALUES(\"%s\", \"%s\");", cUsername, cPassword);

    int rc = sqlite3_exec(psqlConnection, cSql, NULL, NULL, &pcZErrMsg);
    if (!rc == SQLITE_OK)
    {
        return -1;
    }
    else
    {
        return getLastInsertId(piNewUserId);
    }
}

int loginUserCallback(void *pvUserID, int iArgc, char **ppcArgv, char **ppcAzColName)
{
    int *_piId = (int *)pvUserID;

    if (iArgc <= 0)
    {
        *_piId = 0;
        return 0;
    }
    for (int i = 0; i < iArgc; i++)
    {
        if (strcmp(ppcAzColName[i], "id") == 0)
        {
            *_piId = atoi(ppcArgv[i]);
        }
    }

    return 0;
}

void loginUser(char cUsername[], char cPassword[], int *piId)
/* Überprüft mit Hilfe der Daten aus der Datenbank, ob das eingegebene
 * Passwort richtig ist.
 * 1. Parameter: eingegebener Nutzername
 * 2. Parameter: eingegebenes Passwort
 * 3. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
{
    sprintf(cSql, "SELECT * FROM `User` WHERE name =\"%s\" AND password = \"%s\";", cUsername, cPassword);

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, loginUserCallback, piId, &pcZErrMsg);
}

int getUserIdCallback(void *userID, int argc, char **argv, char **azColName)
{
    int *piTmp = (int *)userID;

    if (argc <= 0)
    {
        *piTmp = 0;
        return 0;
    }
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(azColName[i], "id") == 0)
        {
            *piTmp = atoi(argv[i]);
        }
    }

    return 0;
}

void getUserID(char cUsername[8], int *piUserID)
/* Erfragt die zugehörige UserID zu einem bestimmten Nutzernamen aus der Datenbank.
 * 1. Parameter: Nutzername des Spielers, dessen UserID erfragt werden soll
 * 2. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
{
    sprintf(cSql, "SELECT id FROM User WHERE name=\"%s\"  LIMIT 1", cUsername);

    int rc = sqlite3_exec(psqlConnection, cSql, getUserIdCallback, piUserID, &pcZErrMsg);
    if (!rc == SQLITE_OK)
    {
        // strcpy(gameMessage, "Tabellen konnten nicht erstellt werden.");  // zum schnell debuggen
    }
}

int createUserTable()
/* Erstellt die Tabelle für die Daten des Nutzers (NutzerID, Name und Passwort).
 */
{
    sprintf(cSql,
            "CREATE TABLE \"User\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` TEXT NOT NULL, `password` TEXT )");

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, NULL, NULL, &pcZErrMsg);
    if (!rc == SQLITE_OK)
    {
        // strcpy(gameMessage, "Tabellen konnten nicht erstellt werden."); // zum schnell debuggen
        return 1;
    }
    else
    {
        return 0;
    }
}

int createScoreTable()
/* Erstellt die Tabelle für die Daten rund um die benötigten Zeiten
 * der Spieler (NutzerID, benötigte Zeit und Schwierigkeitsgrad).
 */
{
    sprintf(cSql,
            "CREATE TABLE \"Score\" ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `userId` INTEGER NOT NULL, `difficulty` INTEGER NOT NULL, `time` INTEGER, FOREIGN KEY(`userId`) REFERENCES `User`(`id`) )");

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, NULL, NULL, &pcZErrMsg);
    if (!rc == SQLITE_OK)
    {
        // strcpy(gameMessage, "Datenbank existiert bereits."); // zum debuggen
        return 1;
    }
    else
    {
        return 0;
    }
}