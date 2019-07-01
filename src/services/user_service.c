/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 25.06.19
 * Beschreibung: In dieser Datei befinden sich alle Funktionen, die
 * etwas mit den Abfragen an die Datenbank zu tun haben, wenn es
 * um die Spieler und deren Daten geht.
*/

#include "../../headers/services/user_service.h"
#include "../../headers/services/connection.h"
#include "../../headers/core/view.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char cSql[2000];
char *cpZErrMsg;

sqlite3 *psqlConnection;

int lastInsertIdCallBack(void *pvUserID, int iArgc, char **ppcArgv, char **ppcAzColName)
{
    int *piTmp = (int *) pvUserID;

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

	int rc = sqlite3_exec(psqlConnection, cSql, lastInsertIdCallBack, piNewUserId, &cpZErrMsg);
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
    sprintf(cSql, "INSERT INTO `User` (name, cPassword) VALUES(\"%s\", \"%s\");", cUsername, cPassword);

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, NULL, NULL, &cpZErrMsg);
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
    int *_piDd = (int *) pvUserID;

    for (int i = 0; i < iArgc; i++)
    {
        if (strcmp(ppcAzColName[i], "id") == 0)
        {
            *_piDd = atoi(ppcArgv[i]);
            printf("ja\n");
        }
    }

    return 0;
}

void loginUser(char cUsername[], char cPassword[], int *pId)
/* Überprüft mit Hilfe der Daten aus der Datenbank, ob das eingegebene
 * Passwort richtig ist.
 * 1. Parameter: eingegebener Nutzername
 * 2. Parameter: eingegebenes Passwort
 * 3. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
{
    sprintf(cSql, "SELECT * FROM `User` WHERE name =\"%s\" AND cPassword = \"%s\";", cUsername, cPassword);
    printf("cSql: %s\n",cSql);

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, loginUserCallback, pId, &cpZErrMsg);
}


int getUserIdCallback(void *pvUserID, int iArgc, char **ppcArgv, char **ppcAzColName)
{
    int *piTmp = (int *) pvUserID;

    for (int i = 0; i < iArgc; i++)
    {
        if (strcmp(ppcAzColName[i], "id") == 0)
        {
            *piTmp = atoi(ppcArgv[i]);
            printf("ja\n");
        }
    }

    return 0;
}

void getUserID(char cUsername[30], int *piUserID)
/* Erfragt die zugehörige UserID zu einem bestimmten Nutzernamen aus der Datenbank.
 * 1. Parameter: Nutzername des Spielers, dessen UserID erfragt werden soll
 * 2. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
{
    sprintf(cSql, "SELECT id FROM User WHERE name=\"%s\"  LIMIT 1", cUsername);

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, getUserIdCallback, piUserID, &cpZErrMsg);
}



int createUserTable()
/* Erstellt die Tabelle für die Daten des Nutzers (NutzerID, Name und Passwort).
 */
{
    sprintf(cSql, "CREATE TABLE \"User\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` TEXT NOT NULL, `cPassword` TEXT )");

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, NULL, NULL, &cpZErrMsg);
    if (!rc == SQLITE_OK)
    {
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
    sprintf(cSql, "CREATE TABLE \"Score\" ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `userId` INTEGER NOT NULL, `iDifficulty` INTEGER NOT NULL, `time` INTEGER, FOREIGN KEY(`userId`) REFERENCES `User`(`id`) )");

    fflush(stdout);
    clear_output();

    int rc = sqlite3_exec(psqlConnection, cSql, NULL, NULL, &cpZErrMsg);
    if (!rc == SQLITE_OK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}