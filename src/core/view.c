//
// Created by team on 24.06.19. (haben wir schon einen Teamnamen?)
//
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

int deletedCells[9][9];
int userCells[9][9];
int isGameActive;


#ifdef __WIN32__
HANDLE hConsole;
#endif

void initColors() {
#ifdef __WIN32__
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}


#define KRED  "\x1B[31m" ///< color red
#define KGRN  "\x1B[32m" ///< color green
#define KYEL  "\x1B[33m" ///< color yellow
#define KBLU  "\x1B[34m" ///< color blue
#define KMAG  "\x1B[35m" ///< color magenta
#define KCYN  "\x1B[36m" ///< color cay
#define KWHT "\x1B[37m" ///< color white


void printColoredNumber(int number, char *color, int newLine) {
    newLine ? printf("%s%d %s\n", color, number, KWHT) : printf("%s%d %s", color, number, KWHT);
}

void printColoredString(char text[], char color[], int newLine) {
    newLine ? printf("%s%s%s\n", color, text, KWHT) : printf("%s%s%s", color, text, KWHT);
}

void setPrintingColor(char *color) {
    printf("%s", color);
}

void renderUsernameDialog(char *pcUsername) {
    setPrintingColor(KCYN);

    printf(" ++============= Spieler Name ==============++\n");
    char userNameRow[100];
    sprintf(userNameRow, "Name: %s%*s ", pcUsername, 33 - strlen(pcUsername), "");
    printTableLine(userNameRow);
    printEmptyTableLine();
    printTableLine("Sie koennen diesen Schritt              ");
    printTableLine("ueberspringen, druecken Sie             ");
    printTableLine("dafuer einfach Enter.                   ");
    printTableLine("                                        ");
    printEndOfTable();
}

void renderCourt(int gameData[][9], int userCells[][9], int x_coordinate, int y_coordinate) {
    int padding = 5;
    printColoredString("     +---+---+---+---+---+---+---+---+---+", KCYN, 1);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            int number = gameData[i][j];
            if (j % 3 == 0) {
                if (j == 0) {
                    printf("%s%*s| ", KCYN, padding, "");
                } else {
                    printf("%s| ", KCYN);
                }
            } else {
                printColoredString("| ", KGRN, 0);
            }

            if (i == x_coordinate && j == y_coordinate) {
                if (number > 0) {
                    printColoredNumber(number, KRED, 0);
                } else {
                    printColoredString("| ", KRED, 0);
                }
            } else {
                if (gameData[i][j] > 0) {
                    if (userCells[i][j] == 1) {
                        printColoredNumber(number, KMAG, 0);
                    } else {
                        printColoredNumber(number, KWHT, 0);
                    }
                } else {
                    printf("%s  ", KGRN);
                }
            }
        }
        printf("%s|\n", KCYN);
        if ((i + 1) % 3 == 0) {
            printf("%s     +---+---+---+---+---+---+---+---+---+\n", KCYN);
        } else {
            printf("     ");
            for (int i = 0; i < 3; i++) {
                printf("%s+", KCYN);
                printf("%s---+---+---", KGRN);
            }
            printf("%s+\n", KCYN);
        }
    }
}

void renderInfoBox(char *username, int *score, int _difficulty, int remaining) {

    int difficultyBoxWith = 8;
    int userBoxWith = 10;
    int bestscoreWidth = 6;
    int remainingBoxWith = 5;
    int hilfeWidth = 12;
    int tippWidth = 8;
    char userStringTime[6];

    timeToString(timer(TIMER_STATE), userStringTime);
    printColoredString("  ++=================++=====================++", KCYN, 1);

    // first row
    setPrintingColor(KCYN);
    printf("  || ");

    setPrintingColor(KWHT);
    printf("User: %s%*s", username, userBoxWith - strlen(username), "");

    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);
    char us[6];
    timeToString(*score,us);
    printf("Bestscore: %s%*s", us, bestscoreWidth -( *score > 10 ? 2: 1), "");

    setPrintingColor(KCYN);
    printf("||\n");

    // second row
    setPrintingColor(KCYN);
    printf("  || ");

    setPrintingColor(KWHT);
    printf("Time: %s     ", userStringTime);

    setPrintingColor(KCYN);
    printf("|| ");

    char _difficultyText[40];
    switch (_difficulty) {
        case EASY:
            strcpy(_difficultyText, "Einfach");
            break;
        case MEDIUM:
            strcpy(_difficultyText, "Mittel ");
            break;
        case HARD:
            strcpy(_difficultyText, "Schwer ");
            break;
        default:
            break;
    }
    setPrintingColor(KWHT);
    printf("Difficulty: %s%*s", _difficultyText, difficultyBoxWith - strlen(_difficultyText), "");

    setPrintingColor(KCYN);
    printf("||\n");

    // third row
    setPrintingColor(KCYN);
    printf("  || ");

    setPrintingColor(KWHT);
    printf("Remaining: %d%*s", remaining, remainingBoxWith - lenHelper(remaining), "");

    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);
    printf("Hilfe: %d/%d%*s", anzahlDerHilfe, erlaubteAnzahlDerHilfe,
           hilfeWidth - lenHelper(anzahlDerHilfe) - lenHelper(erlaubteAnzahlDerHilfe), "");

    setPrintingColor(KCYN);
    printf("||\n");



    // third row
    setPrintingColor(KCYN);
    printf("  || ");

    setPrintingColor(KWHT);
    printf("Tipps: %d/%d%*s", anzahlDerTipps, erlaubteAnzahlDerTipps,
           tippWidth - lenHelper(anzahlDerTipps) - lenHelper(erlaubteAnzahlDerTipps), "");

    setPrintingColor(KCYN);
    printf("||                     ||\n");
    printf("  ++=================++=====================++\n\n");
}

void renderGameMenu() {
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

void printGameMessage(){
    printf("\n");
    printf(" %s%s\n", KYEL, gameMessage);
    printf("\n");
}

void renderSolvedGame(int solvedAutomatic, int anzahlDerTipps, int anzahlDerHilfe)
{
    char stringTime[5];

    printf("%s ++============== Spielende ================++%s\n",KCYN,KWHT);
    printEmptyTableLine();

    if (solvedAutomatic == 1)
    {
        printTableLine("Du hast das Sudoku leider nicht selbst- ");
        printTableLine("staendig gelöst. Mehr Erfolg beim       ");
        printTableLine("naechsten Versuch!                      ");
    }
    else
    {
        timeToString(timer(TIMER_STATE), stringTime);
        printTableLine("Herzlichen Glueckwunsch!!!              ");
        printTableLine("Du hast das Sudoku in einer Zeit von    ");
        printStartOfLine();
        setPrintingColor(KWHT);
        printf("%s geloest und hast dabei %d Tipps und", stringTime, anzahlDerTipps);
        printEndOfLine();printStartOfLine();
        setPrintingColor(KWHT);
        printf("%d Zellloesungen verwendet.              ", anzahlDerHilfe);
        printEndOfLine();
        printTableLine("Deine Zeit wird automatisch gespeichert.");
    }
    printEmptyTableLine();
    printTableLine("z - Zurueck zum Menue                   ");
    printEndOfTable();
    printTableLine("Das fertige Spielbrett:                 ");
    printEndOfTable();

}

void renderSetPassword(){
    printf(" ++=========== Password setzen =============++\n");
    printEmptyTableLine();
    printTableLine(" Password setzen:                       ");
    printEmptyTableLine();
    printEndOfTable();
}

void renderEnterPassword(){
    printf(" ++=========== Password eingeben ===========++\n");
    printEmptyTableLine();
    printTableLine(" Password eingeben:                     ");
    printEmptyTableLine();

    char m[100];
    sprintf(m, " %s%s                    %s", KRED, gameMessage, KWHT);
    if(strlen(gameMessage) > 0 ){
        printTableLine(m);
    }
    printEmptyTableLine();
    printEndOfTable();
}

void renderMenu() {
    setPrintingColor(KCYN);
    printf(" ++================== Menu =================++\n");
    printEmptyTableLine();
    if (isGameActive > 0) {
        printTableLine("          r - Spiel fortsetzen          ");

    } else {
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

void print_list(struct sScore *head, int iDifficulty) {
    struct sScore *current = head;
    setPrintingColor(KCYN);
    char screenTitle[100];
    sprintf(screenTitle, "%s%*s| %s%*s ", "Spieler", 19 - strlen("Spieler"), "", "Score", 18 - strlen("Score"), "");
    printTableLine(screenTitle);
    printEmptyTableLine();
    while (current != NULL) {
        if(current->difficulty == iDifficulty) {
            if (current->userId == 2) {
                char scoreRow[100];
                char us[10];
                timeToString(current->time,us);
                sprintf(scoreRow, "%s%*s | %s%*s", current->name, 18 - strlen(current->name), "", us,
                        19 - strlen(us), "");
                setPrintingColor(KYEL);
                printTableLine(scoreRow);
                setPrintingColor(KWHT);
            } else {
                char scoreRow[100];
                char us[10];
                timeToString(current->time,us);
                sprintf(scoreRow, "%s%*s | %s%*s", current->name, 18 - strlen(current->name), "", us,
                        19 - strlen(us), "");
                printTableLine(scoreRow);
            }
        }


        current = current->next;
    }
}

void renderDBestScoreDialog() {
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

void renderDetails(struct sScore *scores, int difficulty) {
    char difficultyText[20];
    switch (difficulty) {
        case EASY:
            strcpy(difficultyText, "Einfach");
            break;
        case MEDIUM:
            strcpy(difficultyText, "Mittel ");
            break;
        case HARD:
            strcpy(difficultyText, "Schwer ");
            break;
    }

    setPrintingColor(KCYN);
    printf(" ++======== Bestenliste ( %s) =========++\n", difficultyText);

    print_list(scores, difficulty);

    printEmptyTableLine();
    printEmptyTableLine();

    printTableLine("z - Zurueck                             ");
    printEmptyTableLine();
    printEndOfTable();
}

void renderDifficultyDialog() {
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


void renderHelpDialog() {

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

    if (isGameActive > 0) {
        printTableLine("z - Zurueck zum Spiel                   ");
    } else {
        printTableLine("z - Zurueck zum Menue                   ");
    }

    printEndOfTable();
}

void renderMarkModeMessage() {
    printf(" ++============= Markieren-Modus ============++\n");
    printTableLine("    Sie sind im Markieren-Modus!         ");
    printTableLine("    Nun koennen Sie moegliche            ");
    printTableLine("    Zahlen setzen. Druecken Sie < m >,   ");
    printTableLine("    um diesen Modus zu verlassen.        ");
    printTableLine("    < d > um Notizen zu loeschen.        ");
    printEndOfTable();
}

int getRemainingCells(int array[][9]) {
    int counter = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (array[i][j] == 0) {
                counter++;
            }
        }
    }
    return counter;
}

int lenHelper(int x) {
    if (x >= 50000) return 6;
    if (x >= 5000) return 5;
    if (x >= 500) return 4;
    if (x >= 50) return 3;
    if (x >= 10) return 2;
    return 1;
}

void printStartOfLine() {
    setPrintingColor(KCYN);
    printf(" || ");
}

void printEndOfLine() {
    setPrintingColor(KCYN);
    printf("||\n");
}

void printTableLine(char text[]) {
    printStartOfLine();

    setPrintingColor(KWHT);
    printf(text);

    printEndOfLine();
}

void renderNotesBox(int x, int y) {
    printf("     "); // padding-left
    for (int j = 0; j < y_coordinate - y_coordinate % 3; ++j) {
        printf("    ");
    }
    int shouldDisplay = 0;
    for (int i = 0; i < MAX_MARKS; ++i) {
        if (marks[x_coordinate][y_coordinate][i] != 0) {
            shouldDisplay++;
            break;
        }
    }
    if (shouldDisplay) {
        for (int i = 0; i < MAX_MARKS; ++i) {
            if (marks[x_coordinate][y_coordinate][i] != 0) {
                printf("| %d ", marks[x_coordinate][y_coordinate][i]);
            } else {
                printf("|   ", marks[x_coordinate][y_coordinate][i]);
            }
        }
        printf("|\n");
    } else {
        printf("\n");
    }

}

void printEndOfTable() {
    setPrintingColor(KCYN);
    printf(" ++=========================================++\n");
}

void printEmptyTableLine() {
    printStartOfLine();
    printf("                                        ");
    printEndOfLine();
}

void clear_output() {
#ifdef __unix__
    system("clear");
#endif

#ifdef __WIN32__
    system("cls");
#endif
}
