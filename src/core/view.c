/* Autoren: Karim Echchennouf, Mohammad Kadoura, Florian Kry, Jonathan Trute
 * Klasse: FA12
 * Dateiname: game.c
 * Datum: 24.06.19
 * Beschreibung: Hier werden alle "Screens" im Spiel erstellt bzw.
 * gerendert.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../headers/services/score_service.h"
#include "../../libs/sqlite3.h"
#include "../../headers/core/game.h"
#include "../../headers/core/view.h"
#include "../../headers/shared/shared.h"
#include "../../headers/services/connection.h"

#ifdef __WIN32__
#include <Windows.h>
#endif

#ifdef __WIN32__
HANDLE hConsole;
#endif

void initColors()
{
#ifdef __WIN32__
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

#define KRED "\x1B[31m" ///< color red
#define KGRN "\x1B[32m" ///< color green
#define KYEL "\x1B[33m" ///< color yellow
#define KBLU "\x1B[34m" ///< color blue
#define KMAG "\x1B[35m" ///< color magenta
#define KCYN "\x1B[36m" ///< color cay
#define KWHT "\x1B[37m" ///< color white

int iDeletedCells[9][9];
int iUserCells[9][9];
int iIsGameActive;

void printColoredNumber(int iNumber, char *pcColor, int iNewLine)
/* Dient zur formatierten Ausgabe einer farbigen Zahl im Spielfeld
 * 1. Parameter: Zahl, die gedruckt werden soll
 * 2. Parameter: Zeiger auf die Variable mit dem Wert für die Farbe,
 * in der die Zahl gedruckt werden soll
 * 3. Parameter: Soll danach eine neue Zeile folgen? (1 = Ja, 0 = Nein)
 */
{
    iNewLine ? printf("%s%d %s\n", pcColor, iNumber, KWHT) : printf("%s%d %s", pcColor, iNumber, KWHT);
}

void printColoredString(char cText[], char cColor[], int iNewLine)
/* Dient zur formatierten Ausgabe eines vorgegeben Strings
 * 1. Parameter: String, der gedruckt werden soll
 * 2. Parameter: Farbe, in der der String gedruckt werden soll
 * 3. Parameter: Soll danach eine neue Zeile folgen? (1 = Ja, 0 = Nein)
 */
{
    iNewLine ? printf("%s%s%s\n", cColor, cText, KWHT) : printf("%s%s%s", cColor, cText, KWHT);
}

void setPrintingColor(char *pcColor)
/* Setzt die Farbe, in der ab sofort in die Konsole geschrieben werden soll
 * 1. Parameter: Zeiger auf die Variable mit dem Wert für die Farbe
 */
{
    printf("%s", pcColor);
}

void renderUsernameDialog(char *pcUsername)
/* Gibt den Dialog, in dem der Spieler nach seinem Namen gefragt wird
 * aus.
 * 1. Parameter: Zeiger auf die Variable mit dem Wert des bisherigern
 * Benutzernamens (Zeichen werden einzeln entgegengenommen, um eine
 * nutzerfreundliche Obfläche / Eingabe
 * und das Nutzen der Löschen-Taste zu ermöglichen)
 */
{
    setPrintingColor(KCYN);

    printf(" ++============= Spieler Name ==============++\n");

    char cUserNameRow[100];

    sprintf(cUserNameRow, "Name: %s%*s ", pcUsername, 33 - strlen(pcUsername), "");
    printTableLine(cUserNameRow);
    printEmptyTableLine();
    printTableLine("Sie koennen diesen Schritt              ");
    printTableLine("ueberspringen, druecken Sie             ");
    printTableLine("dafuer einfach Enter.                   ");
    printTableLine("                                        ");
    printEndOfTable();
}

void renderCourt(int iGameData[][9], int iUserCells[][9], int iX_coordinate, int iY_coordinate)
/* Gibt das "sichtbare" Spielfeld in der Konsole aus.
 * 1. Parameter: sichtbares Spielfeld
 * 2. Parameter: Array, in dem gespeichert ist, welche Zellen vom Nutzer befüllt werden
 * müssen
 * 3. Parameter: x-Koordinate, an der sich der Cursor des Spielers befindet
 * 4. Parameter: y-Koordinate, an der sich der Cursor des Spielers befindet
 */
{
    int iPadding = 5;

    printColoredString("     +---+---+---+---+---+---+---+---+---+", KCYN, 1);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {

            int number = iGameData[i][j];
            if (j % 3 == 0)
            {
                if (j == 0)
                {
                    printf("%s%*s| ", KCYN, iPadding, "");
                }
                else
                {
                    printf("%s| ", KCYN);
                }
            }
            else
            {
                printColoredString("| ", KGRN, 0);
            }

            if (i == iX_coordinate && j == iY_coordinate)
            {
                if (number > 0)
                {
                    printColoredNumber(number, KRED, 0);
                }
                else
                {
                    printColoredString("| ", KRED, 0);
                }
            }
            else
            {
                if (iGameData[i][j] > 0)
                {
                    if (iUserCells[i][j] == 1)
                    {
                        printColoredNumber(number, KMAG, 0);
                    }
                    else
                    {
                        printColoredNumber(number, KWHT, 0);
                    }
                }
                else
                {
                    printf("%s  ", KGRN);
                }
            }
        }
        printf("%s|\n", KCYN);

        if ((i + 1) % 3 == 0)
        {
            printf("%s     +---+---+---+---+---+---+---+---+---+\n", KCYN);
        }
        else
        {
            printf("     ");
            for (int i = 0; i < 3; i++)
            {
                printf("%s+", KCYN);
                printf("%s---+---+---", KGRN);
            }
            printf("%s+\n", KCYN);
        }
    }
}

void renderInfoBox(char *pcUsername, int *piScore, int _piDifficulty, int iRemaining)
/* Gibt die Infobox aus, die sich über dem Spielfeld zur Spielzeit befindet
 * und alle wichtigen Informationen, wie die aktuell schon benötigte Zeit,
 * enthält.
 * 1. Parameter: Zeiger auf die Variable, die den Nutzernamen des aktuell spielenden Spielers enthält
 * 2. Parameter: Zeiger auf die Variable, die den Highscore im aktuellen Schwierigkeitsgrad enthält
 * 3. Parameter: aktuell ausgewählter Schwierigkeitsgrad
 * 4. Parameter: Anzahl der Zellen, die der Spieler noch ausfüllen muss
 */
{

    int iDifficultyBoxWith = 8;
    int iUserBoxWith = 10;
    int iBestscoreWidth = 6;
    int iRemainingBoxWith = 5;
    int iHilfeWidth = 12;
    int iTippWidth = 8;
    char cUserStringTime[6];

    timeToString(timer(TIMER_STATE), cUserStringTime);
    printColoredString("  ++=================++=====================++", KCYN, 1);

    /* erste Zeil */
    setPrintingColor(KCYN);
    printf("  || ");

    setPrintingColor(KWHT);
    printf("User: %s%*s", pcUsername, iUserBoxWith - strlen(pcUsername), "");

    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);

    char cUs[6];

    timeToString(*piScore, cUs);
    printf("Bestscore: %s%*s", cUs, iBestscoreWidth - (*piScore > 10 ? 2 : 1), "");

    setPrintingColor(KCYN);
    printf("||\n");

    /* zweite Zeile */
    setPrintingColor(KCYN);
    printf("  || ");

    setPrintingColor(KWHT);
    printf("Time: %s     ", cUserStringTime);

    setPrintingColor(KCYN);
    printf("|| ");

    char _cDifficultyText[40];

    switch (_piDifficulty)
    {
        case EASY:
            strcpy(_cDifficultyText, "Einfach");
            break;
        case MEDIUM:
            strcpy(_cDifficultyText, "Mittel ");
            break;
        case HARD:
            strcpy(_cDifficultyText, "Schwer ");
            break;
        default:
            break;
    }
    setPrintingColor(KWHT);
    printf("Difficulty: %s%*s", _cDifficultyText, iDifficultyBoxWith - strlen(_cDifficultyText), "");

    setPrintingColor(KCYN);
    printf("||\n");

    /* dritte Reihe */
    setPrintingColor(KCYN);
    printf("  || ");

    setPrintingColor(KWHT);
    printf("Remaining: %d%*s", iRemaining, iRemainingBoxWith - lenHelper(iRemaining), "");

    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);
    printf("Hilfe: %d/%d%*s", iAnzahlDerHilfe, iErlaubteAnzahlDerHilfe,
           iHilfeWidth - lenHelper(iAnzahlDerHilfe) - lenHelper(iErlaubteAnzahlDerHilfe), "");

    setPrintingColor(KCYN);
    printf("||\n");

    /* vierte Reihe */
    setPrintingColor(KCYN);
    printf("  || ");

    setPrintingColor(KWHT);
    printf("Tipps: %d/%d%*s", iAnzahlDerTipps, iErlaubteAnzahlDerTipps,
           iTippWidth - lenHelper(iAnzahlDerTipps) - lenHelper(iErlaubteAnzahlDerTipps), "");

    setPrintingColor(KCYN);
    printf("||                     ||\n");
    printf("  ++=================++=====================++\n\n");
}

void renderGameMenu()
/* Gibt die Legende aus, die dem Spieler aufzeigt, welche Tasten er drücken kann, um
 * mit dem Spiel zu interagieren.
 */
{
    setPrintingColor(KCYN);
    printf("    %s Navigation      Befehle\n\n", KCYN);

    setPrintingColor(KWHT);
    printf("     > - Rechts      0 - Zelle loeschen\n");
    printf("     < - Links       m - Notiz eintragen\n");
    printf("     ^ - Oben        t - Tipp (+15 sec)\n");
    printf("     v - Unten       h - Zelle loesen (+30 sec)\n");
    printf("                     s - Loesung anzeigen\n");
    printf("                     k - Spielregeln\n");
    printf("                     p - Pause\n");
    printf("                     a - Abbrechen\n");
    printf("                     q - Beenden\n");
    //printColoredString("c - Check\n", getGameStatus(arr) == FILLED ? KWHT : KRED,1);
}

void printGameMessage()
/* Gibt die kontextbezogene Nachricht an den Spieler aus (z.B. "Passwort falsch")
 */
{
    printf("\n");
    printf(" %s%s\n", KYEL, cGameMessage);
    printf("\n");
}

void renderSolvedGame(int iSolvedAutomatic, int iAnzahlDerTipps, int iAnzahlDerHilfe)
/* Gibt den "Winscreen" in der Konsole aus, wenn das Sudoku vollständig gelöst wurde.
 * 1. Parameter: Wurde das Spiel aufgelöst oder hat der Spieler es selbst gelöst
 * (1 = es wurde aufglöst, 0 = Spieler hat es gelöst)
 * 2. Parameter: Anzahl der benutzten Tipps
 * 3. Parameter: Anzahl der benutzten Zelllösugen
 */
{
    char cStringTime[5];

    printf("%s ++============== Spielende ================++%s\n", KCYN, KWHT);
    printEmptyTableLine();

    if (iSolvedAutomatic == 1)
    {
        printTableLine("Du hast das Sudoku leider nicht selbst- ");
        printTableLine("staendig gelöst. Mehr Erfolg beim       ");
        printTableLine("naechsten Versuch!                      ");
    }
    else
    {
        timeToString(timer(TIMER_STATE), cStringTime);
        printTableLine("Herzlichen Glueckwunsch!!!              ");
        printTableLine("Du hast das Sudoku in einer Zeit von    ");
        printStartOfLine();
        setPrintingColor(KWHT);
        printf("%s geloest und hast dabei %d Tipps und", cStringTime, iAnzahlDerTipps);
        printEndOfLine();
        printStartOfLine();
        setPrintingColor(KWHT);
        printf("%d Zellloesungen verwendet.              ", iAnzahlDerHilfe);
        printEndOfLine();
        printTableLine("Deine Zeit wird automatisch gespeichert.");
    }
    printEmptyTableLine();
    printTableLine("z - Zurueck zum Menue                   ");
    printEndOfTable();
    printTableLine("Das fertige Spielbrett:                 ");
    printEndOfTable();
}

void renderSetPassword()
/* Gibt den Dialog aus, in dem ein neuer Spieler sein Passwort
 * setzen kann.
 */
{
    printf(" ++=========== Password setzen =============++\n");
    printEmptyTableLine();
    printTableLine(" Password setzen:                       ");
    printEmptyTableLine();
    printEndOfTable();
}

void renderEnterPassword()
/* Gibt den Dialog aus, in dem ein bereits registrierter Spieler
 * sein Passwort eingeben kann.
 */
{
    printf(" ++=========== Password eingeben ===========++\n");
    printEmptyTableLine();
    printTableLine(" Password eingeben:                     ");
    printEmptyTableLine();

    char cM[100];

    sprintf(cM, " %s%s                    %s", KRED, cGameMessage, KWHT);

    if (strlen(cGameMessage) > 0)
    {
        printTableLine(cM);
    }
    printEmptyTableLine();
    printEndOfTable();
}

void renderMenu()
/* Druckt das Hauptmenü in die Konsole. Überprüft, ob Spiel noch läuft oder
 * ob ein neues gestartet werden kann (nur visueller Einfluss).
 */
{
    setPrintingColor(KCYN);
    printf(" ++================== Menu =================++\n");

    printEmptyTableLine();

    if (iIsGameActive > 0)
    {
        printTableLine("          r - Spiel fortsetzen          ");
    }
    else
    {
        printTableLine("          s - Spiel starten             ");
    }

    printEmptyTableLine();
    printTableLine("          b - Bestenliste               ");
    printEmptyTableLine();
    printTableLine("          k - Spielregeln               ");
    printEmptyTableLine();
    printTableLine("          q - Beenden                   ");
    printEmptyTableLine();
    printEndOfTable();
}

void print_list(struct sScore *head, int iDifficulty)
/* Gibt die Bestenliste (TOP 10) in Abhängigkeit von der Zeit und dem ausgewählten Schwierigkeitsgrad
* aus.
* 1. Parameter: Zeiger auf Struktur, in der die Bestscores gespeichert sind
* 2. Parameter: ausgewählter Schwierigkeitsgrad
*/
{
    struct sScore *current = head;
    setPrintingColor(KCYN);

    char cScreenTitle[100];

    sprintf(cScreenTitle, "%s%*s| %s%*s ", "Spieler", 19 - strlen("Spieler"), "", "Score", 18 - strlen("Score"), "");
    printTableLine(cScreenTitle);
    printEmptyTableLine();

    while (current != NULL)
    {
        if (current->difficulty == iDifficulty)
        {
            if (current->userId == 2)
            {
                char cScoreRow[100];
                char cUs[10];

                timeToString(current->time, cUs);
                sprintf(cScoreRow, "%s%*s | %s%*s", current->name, 18 - strlen(current->name), "", cUs,
                        19 - strlen(cUs), "");
                setPrintingColor(KYEL);
                printTableLine(cScoreRow);
                setPrintingColor(KWHT);
            }
            else
            {
                char cScoreRow[100];
                char cUs[10];

                timeToString(current->time, cUs);
                sprintf(cScoreRow, "%s%*s | %s%*s", current->name, 18 - strlen(current->name), "", cUs,
                        19 - strlen(cUs), "");
                printTableLine(cScoreRow);
            }
        }

        current = current->next;
    }
}

void renderDBestScoreDialog()
/* Ausgabe des Dialoges, in dem der Spieler den Schwierigkeitsgrad zum Anzeigen der
 * Bestenliste auswählen kann
 */
{
    printf(" ++============= Bestenliste ===============++\n");
    printEmptyTableLine();
    printTableLine("             e - Einfach                ");
    printEmptyTableLine();
    printTableLine("             m - Mittel                 ");
    printEmptyTableLine();
    printTableLine("             s - Schwer                 ");
    printEmptyTableLine();
    printEmptyTableLine();
    printTableLine("Bitte Waehlen Sie                       ");
    printEmptyTableLine();
    printTableLine("z- Zurueck                              ");
    printEndOfTable();
}

void renderDetails(struct sScore *scores, int iDifficulty)
/* Stellt das Grundgerüst für die Ausgabe der Bestenliste bereit bzw.
* initialisiert diese Ausgabe
* 1. Parameter: Zeiger auf Struktur, in der die Bestscores gespeichert sind
* 2. Parameter: ausgewählter Schwierigkeitsgrad
*/
{
    char cDifficultyText[20];

    switch (iDifficulty)
    {
        case EASY:
            strcpy(cDifficultyText, "Einfach");
            break;
        case MEDIUM:
            strcpy(cDifficultyText, "Mittel ");
            break;
        case HARD:
            strcpy(cDifficultyText, "Schwer ");
            break;
    }

    setPrintingColor(KCYN);
    printf(" ++======== Bestenliste ( %s) =========++\n", cDifficultyText);

    print_list(scores, iDifficulty);

    printEmptyTableLine();
    printEmptyTableLine();

    printTableLine("z - Zurueck                             ");
    printEmptyTableLine();
    printEndOfTable();
}

void renderDifficultyDialog()
/* Ausgabe des Dialoges, in dem der Spieler den Schwierigkeitsgrad
 * für sein Spiel wählen kann.
 */
{
    setPrintingColor(KCYN);

    printf(" ++====== Schwierigkeitseinstellungen ======++\n");

    printEmptyTableLine();
    printTableLine("            a - Einfach                 ");

    printEmptyTableLine();
    printTableLine("            b - Mittel                  ");

    printEmptyTableLine();
    printTableLine("            c - Schwer                  ");

    printEmptyTableLine();
    printEmptyTableLine();

    printTableLine("        Waehlen Sie die gewuenschte     ");
    printTableLine("        Schwierigkeitsstufe aus.        ");
    printEmptyTableLine();
    printEndOfTable();
}

void renderHelpDialog()
/* Gibt die Spielregeln in der Konsole aus.
 */
{
    printf("%s ++=========== Die Spielregeln =============++%s\n", KCYN, KWHT);
    printEmptyTableLine();
    printTableLine("Sudoku ist ein Zahlenpuzzle. Das        ");
    printTableLine("Puzzlefeld besteht aus einem Quadrat,   ");
    printTableLine("das in 3x3 Unterquadrate bzw. Bloecke   ");
    printTableLine("eingeteilt ist. Jedes Unterquadrat ist  ");
    printTableLine("wieder in 3x3 Felder eingeteilt. Das    ");
    printTableLine("Gesamtquadrat enthaelt also 81 Felder   ");
    printTableLine("in 9 Reihen und 9 Spalten.              ");
    printTableLine("                                        ");
    printTableLine("In einigen dieser Felder sind schon     ");
    printTableLine("zu Beginn Ziffern (1-9) eingetragen.    ");
    printEmptyTableLine();
    printTableLine("Typischerweise sind 22 bis 36 Felder    ");
    printTableLine("von 81 moeglichen vorgegeben. Das       ");
    printTableLine("Puzzle muss nun so vervollstaendigt     ");
    printTableLine("werden, dass:                           ");
    printTableLine("- in jeder Zeile,                       ");
    printTableLine("- in jeder Spalte und                   ");
    printTableLine("- in jedem der neun Bloecke jede Ziffer ");
    printTableLine("  von 1 bis 9 genau einmal auftritt.    ");
    printTableLine("                                        ");
    printEmptyTableLine();

    if (iIsGameActive > 0)
    {
        printTableLine("z - Zurueck zum Spiel                   ");
    }
    else
    {
        printTableLine("z - Zurueck zum Menue                   ");
    }

    printEndOfTable();
}

void renderMarkModeMessage()
/* Ausgabe des Hinweises auf den "Markieren-Modus"
 */
{
    printf(" ++============= Markieren-Modus ============++\n");
    printTableLine("    Sie sind im Markieren-Modus!         ");
    printTableLine("    Nun koennen Sie moegliche            ");
    printTableLine("    Zahlen setzen. Druecken Sie < m >,   ");
    printTableLine("    um diesen Modus zu verlassen.        ");
    printTableLine("    < d > um Notizen zu loeschen.        ");
    printEndOfTable();
}

int getRemainingCells(int iGameData[][9])
/* Ermittelt die Anzahl der Zellen, die noch befüllt werden müssen.
 * 1. Parameter: sichtbares Spielfeld
 */
{
    int iCounter = 0;

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (iGameData[i][j] == 0)
            {
                iCounter++;
            }
        }
    }
    return iCounter;
}

int lenHelper(int iX)
{
    if (iX >= 50000)
        return 6;
    if (iX >= 5000)
        return 5;
    if (iX >= 500)
        return 4;
    if (iX >= 50)
        return 3;
    if (iX >= 10)
        return 2;
    return 1;
}

void printStartOfLine()
/* Ausgabe des Starts einer Zeile, die zu einer Box mit Informationen
 * gehört (Beispiel: Spielregeln).
 */
{
    setPrintingColor(KCYN);
    printf(" || ");
}

void printEndOfLine()
/* Ausgabe des Endes einer Zeile, die zu einer Box mit Informationen
 * gehört (Beispiel: Spielregeln).
 */
{
    setPrintingColor(KCYN);
    printf("||\n");
}

void printTableLine(char cText[])
/* Ausgabe einer Zeile, die zu einer Box mit Informationen gehört
 * (Beispiel: Spielregeln).
 * 1. Parameter: Text, der gedruckt werden sollen
 */
{
    printStartOfLine();

    setPrintingColor(KWHT);
    printf(cText);

    printEndOfLine();
}

void renderNotesBox(int iX, int iY)
/* Augabe - sofern vorhanden - der Notizen des Spielers (bzw. Tipps).
 */
{
    printf("     "); // padding-left
    for (int j = 0; j < iY_coordinate - iY_coordinate % 3; ++j)
    {
        printf("    ");
    }

    int iShouldDisplay = 0;

    for (int i = 0; i < MAX_MARKS; ++i)
    {
        if (iMarks[iX_coordinate][iY_coordinate][i] != 0)
        {
            iShouldDisplay++;
            break;
        }
    }

    if (iShouldDisplay)
    {
        for (int i = 0; i < MAX_MARKS; ++i)
        {
            if (iMarks[iX_coordinate][iY_coordinate][i] != 0)
            {
                printf("| %d ", iMarks[iX_coordinate][iY_coordinate][i]);
            }
            else
            {
                printf("|   ", iMarks[iX_coordinate][iY_coordinate][i]);
            }
        }
        printf("|\n");
    }
    else
    {
        printf("\n");
    }
}

void printEndOfTable()
/* Ausgabe der Zeile, die das Ende einer Box mit Informationen darstellt
 * (Beispiel: Spielregeln).
 */
{
    setPrintingColor(KCYN);
    printf(" ++=========================================++\n");
}

void printEmptyTableLine()
/* Ausgabe einer leeren Zeile.
 */
{
    printStartOfLine();
    printf("                                        ");
    printEndOfLine();
}

void clear_output()
/* Löscht den aktuellen Konsoleninhalt. Überprüft zunächst das
 * laufende Betriebssystem, um einen ordnungsgemäßen Ablauf zu
 * gewährleisten.
 */
{
#ifdef __unix__
    system("clear");
#endif

#ifdef __WIN32__
    system("cls");
#endif
}
