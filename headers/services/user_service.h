/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden im Wesentlichen alle für das Spiel
 * nötigen Berechnungen durchgeführt. Z.B. das Generieren des Spiel-
 * feldes oder der Timer, wie lange ein Spieler für das Lösen
 * ein Spiel benötigt.
*/

#include "../../libs/sqlite3.h"

/** Registriert einen neuen Nutzer mit seinem von ihm gesetzten Passwort in
 * der Datenbank.
 * 1. Parameter: eingegebener Nutzername
 * 2. Parameter: eingegebenes Passwort
 * 3. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
int registerUser(char cUsername[], char cPassword[], int *piNewUserId);

/** Überprüft mit Hilfe der Daten aus der Datenbank, ob das eingegebene
 * Passwort richtig ist.
 * 1. Parameter: eingegebener Nutzername
 * 2. Parameter: eingegebenes Passwort
 * 3. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
void loginUser(char cUsername[], char cPassword[], int*piId);

/** Erstellt die Tabelle für die Daten des Scores (id, time, userId, difficulty).
 */
int createUserTable();

/** Erstellt die Tabelle für die Daten des Nutzers (NutzerID, Name und Passwort).
 */
int createScoreTable();

int getUserIdCallback(void *userID, int argc, char **argv, char **azColName);

/** Erfragt die zugehörige UserID zu einem bestimmten Nutzernamen aus der Datenbank.
 * 1. Parameter: Nutzername des Spielers, dessen UserID erfragt werden soll
 * 2. Parameter: Zeiger auf die zu befüllende Variable UserID
 */
void getUserID(char cUsername[8], int *piUserID);