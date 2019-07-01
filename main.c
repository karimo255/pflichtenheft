/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: main.c
 * Datum: 24.06.19
 * Beschreibung: Das ist das Hauptprogramm. Von hier aus wird der Mainloop ausgeführt,
 * der das Spiel kontrolliert. Es wird überprüft, in welchem "Screen" sich der Nutzer
 * gerade befindet und welche Eingaben er tätigt.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "headers/core/game.h"
#include "headers/services/user_service.h"
#include "headers/services/score_service.h"
#include "headers/services/connection.h"
#include "libs/sqlite3.h"
#include "headers/core/view.h"

#define TRUE 1
#define FALSE 0

#ifdef __unix__

#include <termios.h>
#include <unistd.h>

static struct termios new_io;
static struct termios old_io;

int cbreak(int fd)
{
    if ((tcgetattr(fd, &old_io)) == -1)
        return -1;
    new_io = old_io;
    new_io.c_lflag = new_io.c_lflag & ~(ECHO | ICANON);
    new_io.c_cc[VMIN] = 1;
    new_io.c_cc[VTIME] = 0;

    if ((tcsetattr(fd, TCSAFLUSH, &new_io)) == -1)
        return -1;
    return 1;
}

int getch(void)
{
    int c;

    if (cbreak(STDIN_FILENO) == -1)
    {
        printf("Fehler bei der Funktion cbreak ... \n");
        exit(EXIT_FAILURE);
    }
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
    return c;
}

#elif __WIN32__
#define HEIGHT 825
#define WIDTH 420
#include <Windows.h>
#include <conio.h>
#endif

int iWindows = 0;

void resizeWindow()
{
#ifdef __unix__
    system("resize -s 48 48");
#endif

#ifdef __WIN32__
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    MoveWindow(hwnd, 550, 5, WIDTH, HEIGHT, TRUE);
    setvbuf(stdout, NULL, _IOFBF, 10000);
    iWindows = 1;
#endif
}

sqlite3 *psqlConnection;

int iExitTheGame = 0;
int iGameData[9][9];
int iDeletedCells[9][9];
int iUserCells[9][9];
int iDifficulty;
int iIsGameActive;
int iIsSolvedAutomatic;
int iCurrentPosition;
char cUsername[50] = "Name eingeben...\0";
char cPassword[6] = {0};
int *piUserID = 0;
int *piBestScore = 0;
int iRemaining = 0;
int iMarks[9][9][MAX_MARKS];

int iB = 0;

int iAnzahlDerTipps = 0;
int iAnzahlDerHilfe = 0;


int iErlaubteAnzahlDerTipps = 0;
int iErlaubteAnzahlDerHilfe = 0;

void checkGameState();
void renderGame();

void setConfig()
/* Setzt bzw. initialisiert die erlaubte Anzahl, wie oft der Spieler
 * die Tipp- und Hilfefunktion nutzen darf. Setzt ausßerdem die bereits
 * genutzten Funktionen zurück.
 */
{
    iAnzahlDerTipps = 0;
    iAnzahlDerHilfe = 0;

    switch (iDifficulty)
    {
        case EASY:
            iErlaubteAnzahlDerHilfe = 5;
            iErlaubteAnzahlDerTipps = 8;
            break;
        case MEDIUM:
            iErlaubteAnzahlDerHilfe=4;
            iErlaubteAnzahlDerTipps=6;
            break;
        case HARD:
            iErlaubteAnzahlDerHilfe=3;
            iErlaubteAnzahlDerTipps=4;
            break;
    }
}

int main()
/* Hauptfunktion. "Grundfunktionalitäten" werden bereitgestellt.
 * Hierzu zählen das Setzen der Fenstergröße, das Initialisieren der
 * Farben, die Herstellung einer Verbindung zur Datenbank ( + eventuelles
 * Erstellen dieser) und das Starten des Hauptspielzykluses.
 */
{
    resizeWindow();
    initColors();

    piUserID = malloc(sizeof(int));
    piBestScore = malloc(sizeof(int));

    int rc = sqlite3_open("./sudoku.db", &psqlConnection);
    if (rc != SQLITE_OK)
    {
        printf("Failed to open the sudoku.db\n");
        return 1;
    }

    createUserTable();
    createScoreTable();

    fflush(stdout);

    srand(time(NULL));

    struct sScore *psScores;
    psScores = malloc(sizeof(struct sScore);
    psScores->next = NULL;
    getScores(psScores, 5);

    iCurrentPosition = MENU;
    iDifficulty = EASY;

    renderGame(psScores);

    printf("Ciao");
    return 0;
}

void checkGameState()
/* Überprüft, ob alle Sudoku-Felder befüllt sind und ob der Spieler
 * sie richtig ausgefüllt hat. Wenn dem so ist, wird er zum "Winscreen"
 * weitergeleitet, andernfalls wird er benachrichtigt, dass er das
 * Spiel nicht richtig gelöst hat.
 */
{
    if (iIsGameActive && getGameStatus(iGameData) == FILLED)
    {
        int iSolveState = solveGame(iGameData);
        if (iSolveState == 1)
        {
            if (*piUserID != 0 && iIsSolvedAutomatic == 0 && strcmp(cUsername, "anonym") != 0)
            {
                int _iScore = timer(TIMER_STATE);
                insertScore(piUserID, _iScore, iDifficulty);
            }
            iCurrentPosition = SOLVED_GAME;
            iIsGameActive = 0;
        }
        else
        {

            strcpy(cGameMessage, "Das Spiel ist nicht korrekt geloest.");
        }
    }
    
    // int score = timer(TIMER_STATE);

    // char t[6];
    // timeToString(sScore, t);
}

void navigateTo(int iPos)
/* Setzt die neue Position des Cursors, also die Position des Spielers
 * im Spielfeld.
 * 1. Parameter: Richtung, in die der Cursor sich bewegen soll
 */
{
    switch (iPos)
    { // the real value

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

    if (iX_coordinate > 8)
    {
        iX_coordinate = 0;
    }
    if (iX_coordinate < 0)
    {
        iX_coordinate = 8;
    }
    if (iY_coordinate > 8)
    {
        iY_coordinate = 0;
    }
    if (iY_coordinate < 0)
    {
        iY_coordinate = 8;
    }
}

void handleUserInput()
/* Überprüft, welche Taste der Nutzer gedrückt hat und ruft die nötigen
 * Funktionen für die Verarbeitung der entsprechenden Eingabe auf.
 */
{
    int iUserInput;
    char cCh;

    if (iCurrentPosition == USER_NAME)
    {
        cCh = getch();

        if (cCh != 13 && cCh != '\n' && cCh != EOF)
        { // no enter
            if (strcmp(cUsername, "Name eingeben...") == 0 ) resetArray(cUsername, 30);
            if (cCh == 27)
            { // escape
                strcpy(cUsername, "anonym");
                iCurrentPosition = DIFFICULTY_DIALOG;
            } else if (cCh == 127 || cCh == 8)
            {
                iB--;
                if (iB < 0)
                {
                    iB = 0;
                }
                cUsername[iB] = 0;

            } else if (iB < 30)
            {
                cUsername[iB] = cCh;
                iB++;
            }
        } else if (strlen(cUsername) > 0)
        { // enter
            cUsername[iB] = '\0';

            iB = 0;
            if (strcmp(cUsername, "Name eingeben...") == 0 || strcmp(cUsername, "") == 0)
            {
                strcpy(cUsername, "anonym");
            } else{
                getUserID(cUsername, piUserID);
                if (*piUserID > 0)
                {
                    iCurrentPosition = ENTER_PASSWORD;
                }
                else
                {
                    iCurrentPosition = SET_PASSWORD;
                }
            }
        }
    } else if (iCurrentPosition == SET_PASSWORD)
    {
        cCh = getch();

        printf("%d\n", cCh);
        if (cCh != 13 && cCh != '\n' && cCh != EOF)
        { // not enter
         //   if (cPassword[0] == 0) resetArray(cPassword, 8);
            if (cCh == 127 || cCh == 8)
            { // delete backspace
                iB--;
                if (iB < 0)
                {
                    iB = 0;
                }
                cPassword[iB] = 0;

            } else if (iB < 6)
            {
                cPassword[iB] = cCh;
                iB++;
            }
        }
        else
        { // enter
            registerUser(cUsername, cPassword, piUserID);
            fflush(stdout);
            clear_output();
            iCurrentPosition = DIFFICULTY_DIALOG;
        }
    }else if (iCurrentPosition == ENTER_PASSWORD)
    {
        cCh = getch();

        if (cCh != 13 && cCh != '\n' && cCh != EOF)
        { // not enter
            if (cCh == 127 || cCh == 8)
            { // delete backspace
                iB--;
                if (iB < 0)
                {
                    iB = 0;
                }
                cPassword[iB] = 0;

            } else if (iB < 6)
            {
                cPassword[iB] = cCh;
                iB++;
            }
        }
        else
        { // enter
            int *id=0;

            id = malloc(sizeof(int));

            loginUser(cUsername,cPassword, id);
            if (*id > 0){
                iCurrentPosition = DIFFICULTY_DIALOG;
             //   strcpy(cGameMessage, "Passwort ist falsch");
            } else{
                char loginMessage[50];
                strcpy(cGameMessage, "Passwort ist falsch");
                resetArray(cPassword,6);
                iB =0;
            }
        }
    }
    else
    {

        if ((iUserInput = getch()) == 224)
        {
            navigateTo(getch()); // iWindows
        }
        else
        {
            navigateTo(iUserInput); // linux
            switch (iCurrentPosition)
            {
                case DIFFICULTY_DIALOG:
//				if (iUserInput == 10) // enter pressed
//				{
//					iCurrentPosition = IN_GAME;
//				}
                    if (isalpha(iUserInput))
                    {
                        switch (iUserInput)
                        {
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
                      //  getScores(scores, 5);

                    }
                    break;

                case MENU:
                    if (isalpha(iUserInput))
                    {
                        switch (iUserInput)
                        {
                            case 's':
                                if (iIsGameActive > 0)
                                {
                                    break;
                                }

                                iCurrentPosition = USER_NAME;
                                break;

                            case 'r':
                                if (iIsGameActive == 0)
                                {
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
                    break;

                case IN_GAME:
                    if (isdigit(iUserInput))
                    {
                        if (iUserCells[iX_coordinate][iY_coordinate] == 1)
                        {
                            iGameData[iX_coordinate][iY_coordinate] = iUserInput - '0';
                        }
                    } else if (isalpha(iUserInput))
                    {
                        switch (iUserInput)
                        {
                            case 'h':
                                if (iDeletedCells[iX_coordinate][iY_coordinate] > 0)
                                {
                                    if (iAnzahlDerHilfe == iErlaubteAnzahlDerHilfe)
                                    {
                                        strcpy(cGameMessage, "Anzahl der Hilfen verbraucht.");
                                        break;
                                    }
                                    iAnzahlDerHilfe++;
                                    solveCell(iGameData, iX_coordinate, iY_coordinate);
                                    timer(HELP_USED);
                                }
                                else
                                {
                                    strcpy(cGameMessage, "Zelle ist nicht leer.");
                                }


                                break;
                            case 't':
                                if (iDeletedCells[iX_coordinate][iY_coordinate] > 0)
                                {
                                    if (iAnzahlDerTipps == iErlaubteAnzahlDerTipps)
                                    {
                                        strcpy(cGameMessage, "Anzahl der Tipps verbraucht.");
                                        break;
                                    }
                                    iAnzahlDerTipps++;
                                    fillNotesForCell(iX_coordinate, iY_coordinate);
                                    timer(TIPP_USED);
                                }
                                else
                                {
                                    strcpy(cGameMessage, "Tipp ist nicht verfuegbar");
                                }

                                break;
                            case 'q':
                                iExitTheGame = 1;
                            case 'a':
                                resetGameData(iGameData);
                                iIsGameActive = 0;
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
                                iIsSolvedAutomatic = 1;
                                break;
                            case 'm':
                                if (iGameData[iX_coordinate][iY_coordinate] == 0)
                                {
                                    iCurrentPosition = SET_MARK;
                                }
                                else
                                {
                                    strcpy(cGameMessage, "Markiere-Mous nicht verfuegbar.");
                                }
                                break;
                        }
                    }
                    break;
                case SOLVED_GAME:
                    if (isalpha(iUserInput))
                    {
                        switch (iUserInput)
                        {
                            case 'z':
                                iCurrentPosition = MENU;
                                break;
                            case 'q':
                                iExitTheGame = 1;
                                break;
                        }
                    }
                    break;
                case SET_MARK :
                    if (isdigit(iUserInput))
                    {
                        makeNote(iX_coordinate, iY_coordinate, iUserInput - '0');
                    }
                    if (isalpha(iUserInput))
                    {
                        switch (iUserInput)
                        {
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
                    break;
                case DETAILS:
                    if (isalpha(iUserInput))
                    {
                        switch (iUserInput)
                        {
                            case 'z':
                                iCurrentPosition = MENU;

                                break;
                        }
                    }
                    break;
                case DETAILS_DIALOG:
                    if (isalpha(iUserInput))
                    {
                        switch (iUserInput)
                        {
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
                    break;
                case HELP:
                    if (isalpha(iUserInput))
                    {
                        switch (iUserInput)
                        {
                            case 'z':
                                if (iIsGameActive > 0)
                                {
                                    timer(TIMER_PAUSE);
                                    iCurrentPosition = IN_GAME;
                                }
                                else
                                {
                                    iCurrentPosition = MENU;
                                }
                                break;
                        }
                    }
                    break;
            }
        }
    }
}


void renderGame(struct sScore *pSScores)
/* Hauptspielzyklus. Er hält das Spiel am laufen, indem er überpüft, in
 * welchem "Screen" sich der Spieler befindet und je nachdem die entsprechenden
 * Funktionen ausführt.
 */
{
    while (!iExitTheGame)
    {
        clear_output();

        switch (iCurrentPosition)
        {
            case SET_PASSWORD:
                renderSetPassword();
                fflush(stdout);
                break;
            case ENTER_PASSWORD:
                renderEnterPassword();
                fflush(stdout);
                break;
            case MENU:
                renderMenu();
                fflush(stdout);
                break;

            case USER_NAME:
                renderUsernameDialog(cUsername);
                fflush(stdout);
                break;

            case IN_GAME:

                if (iIsGameActive == 0)
                {
                    resetGameData(iGameData);
                    resetGameData(iDeletedCells);
                    resetGameData(iUserCells);
                    generateGameData(iGameData);
                    deleteCells(iGameData, iDifficulty);
                    resetArray(iMarks[iX_coordinate][iY_coordinate], MAX_MARKS);
                    iIsGameActive = 1;
                    timer(TIMER_START);
                    iIsSolvedAutomatic = 0;
                }

                getBestScore(piBestScore, iDifficulty);
                iRemaining = getRemainingCells(iGameData);
                renderInfoBox(cUsername, piBestScore, iDifficulty, iRemaining);
                renderNotesBox(iX_coordinate, iY_coordinate);
                renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
                renderGameMenu();

                sprintf(cGameMessage, "%s", "");
                fflush(stdout);
                break;

            case SET_MARK:
                getBestScore(piBestScore, iDifficulty);
                iRemaining = getRemainingCells(iGameData);
                renderInfoBox(cUsername, piBestScore, iDifficulty, iRemaining);
                renderMarkModeMessage();
                renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
                renderGameMenu();
                sprintf(cGameMessage, "%s", "");
                fflush(stdout);
                break;

            case DIFFICULTY_DIALOG:
                renderDifficultyDialog();
                fflush(stdout);
                break;

            case DETAILS_DIALOG:
                renderDBestScoreDialog();
                fflush(stdout);
                break;

            case DETAILS:
                renderDetails(pSScores, iDifficulty);
                fflush(stdout);
                break;
            case HELP:
                renderHelpDialog();
                fflush(stdout);
                break;
            case SOLVED_GAME:
                renderSolvedGame(iIsSolvedAutomatic, iAnzahlDerTipps, iAnzahlDerHilfe);
                renderCourt(iGameData, iUserCells, iX_coordinate, iY_coordinate);
                fflush(stdout);
                break;
        }

        if (iCurrentPosition == IN_GAME && iWindows == 1)
        {
            int iEnde = 0;
            time_t now, notNow;
            now = time(NULL);

            while ((time(&notNow) - now) == 0 && !iEnde)
            {
                if (kbhit())
                {
                    handleUserInput();
                    checkGameState();
                    iEnde = 1;
                }
                Sleep(1);
            }
        }
        else
        {
            handleUserInput();
            checkGameState();
        }
    }
}


