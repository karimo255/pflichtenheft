/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: inputHandler.c
 * Datum: 28.06.19
 * Beschreibung: Stellt alle Funktionen zur Verarbeitung der Nutzereingaben
 * bereit.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#include "../../headers/core/inputHandler.h"
#include "../../headers/core/game.h"
#include "../../headers/services/user_service.h"
#include "../../headers/services/score_service.h"
#include "../../headers/services/connection.h"
#include "../../libs/sqlite3.h"
#include "../../headers/core/view.h"

#ifdef __unix__

#include <termios.h>
#include <unistd.h>
#include "../../headers/getch.h"

#elif __WIN32__
#include <Windows.h>
#include <conio.h>
#endif

int *piIsUserLoggedIn = 0;
char cPassword[6] = {0};
int iCurrentPosition;
int iB = 0;
int *piUserID;
int iDifficulty;

void navigateTo(int iPos)
/* Sorgt dafür, dass der Cursor des Spielers seine Position
 * ändert.
 * 1. Parameter: Richtung, in die der Spieler seinen Cursor
 * bewegen möchte
 */
{
    switch (iPos) { // the real value
        /* Code für Pfeil nach oben */
        case UP:
            iX_coordinate--;
            break;

            /* Code für Pfeil nach unten */
        case DOWN:
            iX_coordinate++;
            break;

            /* Code für Pfeil nach rechts */
        case RIGHT:
            iY_coordinate++;
            break;

            /* Code für Pfeil nach links */
        case LEFT:
            iY_coordinate--;
            break;

            /* Code für Pfeil nach oben unter Linux */
        case UP_LINUX:
            iX_coordinate--;
            break;

            /* Code für Pfeil nach unten unter Linux */
        case DOWN_LINUX:
            iX_coordinate++;
            break;

            /* Code für Pfeil nach rechts unter Linux */
        case RIGHT_LINUX:
            iY_coordinate++;
            break;

            /* Code für Pfeil nach links unter Linux */
        case LEFT_LINUX:
            iY_coordinate--;
            break;
    }

    if (iX_coordinate > 8) {
        iX_coordinate = 0;
    }
    if (iX_coordinate < 0) {
        iX_coordinate = 8;
    }
    if (iY_coordinate > 8) {
        iY_coordinate = 0;
    }
    if (iY_coordinate < 0) {
        iY_coordinate = 8;
    }
}

void handleUserNameInput()
/* Wird aufgerufen, wenn der Nutzer sich im Dialog befindet, in dem er
 * seinen Namen angeben soll. Ermöglicht zudem das Überspringen dieses
 * Schrittes. Der Spieler ist dann unter "anonym" angemeldet.
 * Es werden einzelne Buchstaben eingelesen, um den Dialog dynamisch zu
 * gestalten und das nutzen der Löschen-Taste zu ermöglichen, um einzelne
 * Buchstaben löschen zu können. So wird ein möglichst nutzerfreundliches
 * Erlebnis garantiert.
 */
{
    char cCh = getch();

    if (cCh != 13 && cCh != '\n' && cCh != EOF) { // no enter
        if (cCh == 27) { // escape
            strcpy(cUusername, "anonym");
            iCurrentPosition = DIFFICULTY_DIALOG;
        } else if (cCh == 127 || cCh == 8) {
            iB--;
            if (iB < 0) {
                iB = 0;
            }
            cUusername[iB] = 0;
        } else if (iB < 8) {
            if (isalpha(cCh) || isdigit(cCh)) {
                cUusername[iB] = cCh;
                iB++;
            }
        }
    } else if (strlen(cUusername) > 0) { // enter
        cUusername[iB] = '\0';

        iB = 0;
        if (cUusername[0] == 0) {
            strcpy(cUusername, "anonym");
        } else {
            piUserID = malloc(sizeof(int));
            *piUserID = 0;
            getUserID(cUusername, piUserID);

            if (*piUserID > 0) {
                iCurrentPosition = ENTER_PASSWORD;
            } else {
                iCurrentPosition = SET_PASSWORD;
            }
        }
    }
}

void handleSetPasswordInput()
/* Wird aufgerufen, wenn sich ein neuer Nutzer anmeldet und ein
 * Passwort setzen möchte. Dieses wird nach einer Bestätigung durch
 * das Drücken der Entertaste mit dem Nutzernamen und der UserID in
 * der Datenbank hinterlegt (bzw. die Funktion, die dies erledigt wird
 * aufgerufen).
 * Es werden einzelne Buchstaben eingelesen, um den Dialog dynamisch zu
 * gestalten und das nutzen der Löschen-Taste zu ermöglichen, um einzelne
 * Buchstaben löschen zu können. So wird ein möglichst nutzerfreundliches
 * Erlebnis garantiert.
 */
{
    char cCh = getch();

    printf("%d\n", cCh);
    if (cCh != 13 && cCh != '\n' && cCh != EOF) { // not enter
        if (cCh == 27) { // escape
            resetArray(cPassword, 8);
            iCurrentPosition = USER_NAME;
        }
        //   if (password[0] == 0) resetArray(password, 8);
        if (cCh == 127 || cCh == 8) { // delete backspace
            iB--;
            if (iB < 0) {
                iB = 0;
            }
            cPassword[iB] = 0;
        } else if (iB < 6) {
            if (isalpha(cCh) || isdigit(cCh)) {
                cPassword[iB] = cCh;
                iB++;
            }
        }
    }
        /* Enter */
    else {
        registerUser(cUusername, cPassword, piUserID);

        clear_output();
        fflush(stdout);

        iCurrentPosition = DIFFICULTY_DIALOG;
    }
}

void handleEnterPasswordInput()
/* Wird aufgerufen, wenn sich ein bereits registrierter Nutzer anmelden
 * möchte. Dieser kann nun sein Passwort eingeben. Das eingegebene Passwort
 * wird dem in der Datenbank hinterlegtem Passwort verglichen und auf
 * Übereinstimmung geprüft.
 * Es werden einzelne Buchstaben eingelesen, um den Dialog dynamisch zu
 * gestalten und das nutzen der Löschen-Taste zu ermöglichen, um einzelne
 * Buchstaben löschen zu können. So wird ein möglichst nutzerfreundliches
 * Erlebnis garantiert.
 */
{
    char cCh = getch();

    if (cCh != 13 && cCh != '\n' && cCh != EOF) { // not enter
        if (cCh == 27) { // escape
            resetArray(cPassword, 8);
            iCurrentPosition = USER_NAME;
        } else if (cCh == 127 || cCh == 8) { // delete backspace
            iB--;
            if (iB < 0) {
                iB = 0;
            }
            cPassword[iB] = 0;
        } else if (iB < 6) {
            if (isalpha(cCh) || isdigit(cCh)) {
                cPassword[iB] = cCh;
                iB++;
            }
        }
    }
        /* Enter */
    else {
        piIsUserLoggedIn = malloc(sizeof(int));
        *piIsUserLoggedIn = 0;

        loginUser(cUusername, cPassword, piIsUserLoggedIn);
        if (*piIsUserLoggedIn > 0) {
            iCurrentPosition = DIFFICULTY_DIALOG;
        } else {
            strcpy(cGameMessage, "Passwort ist falsch");
            resetArray(cPassword, 6);
            iB = 0;
        }
    }
}

void handleDifficultyDialogInput(int iUserInput)
/* Wird aufgerufen, wenn der Nutzer aufgefordert wird den Schwierigkeits-
 * grad für sein Spiel zu wählen und eine Eingabe tätigt. Setzt den Schwierigkeitsgrad
 * und die Position des Spielers auf "im Spiel".
 * 1. Parameter: Eingabe des Nutzers
 */
{
    if (isalpha(iUserInput)) {
        switch (iUserInput) {
            case 'a':
                iDifficulty = EASY;
                iCurrentPosition = IN_GAME;
                setConfig();
                break;

            case 'b':
                iDifficulty = MEDIUM;
                iCurrentPosition = IN_GAME;
                setConfig();
                break;

            case 'c':
                iDifficulty = HARD;
                iCurrentPosition = IN_GAME;
                setConfig();
                break;
        }
    }
}

void handleMenuInput(int iUserInput)
/* Wird aufgerufen, wenn der Spieler eine Eingabe im Hauptmenü tätigt.
 * Verarbeiet die Eingabe des Nutzers und ruft die nötigen Funtkionen
 * auf, um den Nutzer zu den nächsten "Screens" weiterzuleiten.
 * 1. Parameter: Eingabe des Nutzers
 */
{
    if (isalpha(iUserInput)) {
        switch (iUserInput) {
            case 's':
                if (iIsGameActive > 0) {
                    break;
                }
                iB = strlen(cUusername);
                iCurrentPosition = USER_NAME;
                break;

            case 'r':
                if (iIsGameActive == 0) {
                    break;
                }
                timer(TIMER_PAUSE);
                iCurrentPosition = IN_GAME;
                break;

            case 'b':
                iCurrentPosition = DETAILS_DIALOG;
                break;

            case 'k':
                iCurrentPosition = HELP;
                break;

            case 'q':
                iExitTheGame = 1;
                break;
        }
    }
}

void handleInGameInput(int iUserInput)
/* Wird aufgerufen, wenn der Nutzer eine Eingabe tätigt, wenn er sich gerade
 * im laufenden Spiel befindet. Ruft nötige Funktionen auf, um die Eingabe
 * des Nutzers der Legende entsprechend zu verarbeiten.
 * 1. Parameter: Eingabe des Nutzers
 */
{
    if (isdigit(iUserInput)) {
        if (iUserCells[iX_coordinate][iY_coordinate] == 1) {
            iGameData[iX_coordinate][iY_coordinate] = iUserInput - '0';
        }
    } else if (isalpha(iUserInput)) {
        switch (iUserInput) {
            case 'h':
                if (iDeletedCells[iX_coordinate][iY_coordinate] > 0) {
                    if (iAnzahlDerHilfe == iErlaubteAnzahlDerHilfe) {
                        strcpy(cGameMessage, "Anzahl der Hilfen verbraucht.");
                        break;
                    }
                    iAnzahlDerHilfe++;
                    solveCell(iGameData, iX_coordinate, iY_coordinate);
                    timer(HELP_USED);
                } else {
                    strcpy(cGameMessage, "Zelle ist nicht leer.");
                }
                break;
            case 't':
                if (iDeletedCells[iX_coordinate][iY_coordinate] > 0) {
                    if (iAnzahlDerTipps == iErlaubteAnzahlDerTipps) {
                        strcpy(cGameMessage, "Anzahl der Tipps verbraucht.");
                        break;
                    }
                    iAnzahlDerTipps++;
                    fillNotesForCell(iX_coordinate, iY_coordinate);
                    timer(TIPP_USED);
                } else {
                    strcpy(cGameMessage, "Tipp ist nicht verfuegbar");
                }

                break;
            case 'q':
                iExitTheGame = 1;
            case 'a':
                resetGameData(iGameData);
                iIsGameActive = 0;
                resetArray(cUusername, 8);
                timer(RESET_TIMER);

            case 'p':
                timer(TIMER_PAUSE);
                iCurrentPosition = MENU;
                break;
            case 'k':
                timer(TIMER_PAUSE);
                iCurrentPosition = HELP;
                break;
            case 's':
                solveAll(iGameData, iDeletedCells);
                isSolvedAutomatic = 1;
                iCurrentPosition = SOLVED_GAME;
                break;
            case 'm':
                if (iGameData[iX_coordinate][iY_coordinate] == 0) {
                    iCurrentPosition = SET_MARK;
                } else {
                    strcpy(cGameMessage, "Markiere-Mous nicht verfuegbar.");
                }
                break;
        }
    }
}

void handleSolvedGameInput(int iUserInput)
/* Wird aufgerufen, wenn der Nutzer eine Eingabe im "Winscreen" tätigt.
 * Eingabe wird sachgerecht behandelt.
 * 1. Parameter: Eingabe des Nutzers
 */
{
    if (isalpha(iUserInput)) {
        switch (iUserInput) {
            case 'z':
                iCurrentPosition = MENU;
                break;
            case 'q':
                iExitTheGame = 1;
                break;
        }
    }
}

void handleSetMarkInput(int iUserInput)
/* Wird aufgerufen, wenn der Nutzer etwas eingibt, wenn er sich im
 * "Makier-Modus" befindet. Bei der Eingabe wird überprüft, ob es
 * sich um eine Zahl(dann wird sie in die zugehörige Notiz
 * eingetragen) oder einen Buchstaben (dann wird entsprechend der
 * Legende die nötigen Funktionen zur Umsetzung ausgeführt) handelt.
 * 1. Parameter: Eingabe des Nutzers
 */
{
    if (isdigit(iUserInput)) {
        makeNote(iX_coordinate, iY_coordinate, iUserInput - '0');
    }
    if (isalpha(iUserInput)) {
        switch (iUserInput) {
            case 'm':
                strcpy(cGameMessage, "Notizen erstellt");
                iCurrentPosition = IN_GAME;
                break;
            case 'd':
                resetArray(iMarks[iX_coordinate][iY_coordinate], MAX_MARKS);
                strcpy(cGameMessage, "Notizen geloescht");
                iCurrentPosition = IN_GAME;
                break;
            case 'q':
                iExitTheGame = 1;
        }
    }
}

void handleDetailsInput(int iUserInput)
/* Wird aufgerufen, sobald der Nutzer versucht etwas einzugeben, während
 * er sich in der Bestenliste befindet. "z" bringt den Spieler zurück zum
 * Menü.
 * 1. Parameter: Eingabe des Nutzers
 */
{
    if (isalpha(iUserInput)) {
        switch (iUserInput) {
            case 'z':
                iCurrentPosition = MENU;
                break;
        }
    }
}

void handleDetailsDialogInput(int iUserInput)
/* Wenn der Spieler eine Eingabe im Dialog, in dem er den Schwierigkeitsgrad
 * für die Bestenliste auswählen soll, tätigt, wird diese Funktion aufgerufen
 * und seine Eingabe wird verarbeitet.
 * 1. Parameter: Eingabe des Nutzers
 */
{
    if (isalpha(iUserInput)) {
        switch (iUserInput) {
            case 'e':
                iDifficulty = EASY;
                iCurrentPosition = DETAILS;
                break;
            case 'm':
                iDifficulty = MEDIUM;
                iCurrentPosition = DETAILS;
                break;
            case 's':
                iDifficulty = HARD;
                iCurrentPosition = DETAILS;
                break;
            case 'z':
                iCurrentPosition = MENU;
                break;
        }
    }
}

void handleHelpInput(int iUserInput)
/* Wird aufgerufen, sobald der Nutzer versucht etwas einzugeben, während
 * er sich in den Spielregeln befindet. "z" bringt den Spieler zurück zum
 * Menü oder zum laufenden Spiel; ausgehend davon, ob es noch ein ungelöstes
 * Sudokufeld gibt.
 * 1. Parameter: Eingabe des Nutzers
 */
{
    if (isalpha(iUserInput)) {
        switch (iUserInput) {
            case 'z':
                if (iIsGameActive > 0) {
                    timer(TIMER_PAUSE);
                    iCurrentPosition = IN_GAME;
                } else {
                    iCurrentPosition = MENU;
                }
                break;
        }
    }
}

void setConfig()
/* Setzt bzw. initialisiert die erlaubte Anzahl, wie oft der Spieler
 * die Tipp- und Hilfefunktion nutzen darf. Setzt ausßerdem die bereits
 * genutzten Funktionen zurück.
 */
{
    iAnzahlDerTipps = 0;
    iAnzahlDerHilfe = 0;

    switch (iDifficulty) {
        case EASY:
            iErlaubteAnzahlDerTipps = 8;
            iErlaubteAnzahlDerHilfe = 5;
            break;
        case MEDIUM:
            iErlaubteAnzahlDerTipps = 6;
            iErlaubteAnzahlDerHilfe = 4;
            break;
        case HARD:
            iErlaubteAnzahlDerTipps = 4;
            iErlaubteAnzahlDerHilfe = 3;
            break;
    }
}
