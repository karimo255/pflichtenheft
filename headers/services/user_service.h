#include "../../libs/sqlite3.h"

/** Registriert einen neuen Nutzer mit seinem von ihm gesetzten Passwort in
 * der Datenbank.
 * 1. Parameter: eingegebener Nutzername
 * 2. Parameter: eingegebenes Passwort
 * 3. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
int registerUser(char username[], char password[], int *newUserId);

/** Überprüft mit Hilfe der Daten aus der Datenbank, ob das eingegebene
 * Passwort richtig ist.
 * 1. Parameter: eingegebener Nutzername
 * 2. Parameter: eingegebenes Passwort
 * 3. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
void loginUser(char username[], char password[], int*id);
int createUserTable();

/** Erstellt die Tabelle für die Daten des Nutzers (NutzerID, Name und Passwort).
 */
int createScoreTable();

int getUserIdCallback(void *userID, int argc, char **argv, char **azColName);

/** Erfragt die zugehörige UserID zu einem bestimmten Nutzernamen aus der Datenbank.
 * 1. Parameter: Nutzername des Spielers, dessen UserID erfragt werden soll
 * 2. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
void getUserID(char username[30], int *userID);