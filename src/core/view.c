//
// Created by team on 24.06.19. (haben wir schon einen Teamnamen?)
//
#include <stdio.h>
#include <string.h>

#include "../../headers/services/score_service.h"
#include "../../libs/sqlite3.h"
#include "../../headers/core/view.h"
#include "../../headers/shared/shared.h"
#include "../../headers/core/game.h"
#include "../../headers/services/connection.h"

#ifdef __WIN32__
#include <Windows.h>
#endif

int deletedCells[9][9];
int userCells[9][9];
int difficulty;
int isGameActive;


#ifdef __WIN32__
HANDLE hConsole;
#endif

void initColors(){
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


void printColoredNumber(int number, char *color, int newLine){
    newLine ? printf("%s%d %s\n", color, number, KWHT) : printf("%s%d %s", color, number, KWHT);
}

void printColoredString(char text[], char color[], int newLine){
    newLine ? printf("%s%s%s\n", color, text, KWHT) : printf("%s%s%s", color, text, KWHT);
}

void setPrintingColor(char *color){
    printf("%s", color);
}

void renderUsernameDialog(char *username) {
    setPrintingColor(KCYN);

    printf("++=============Spielner Name===============++\n");
    printf("|| Name: %s%*s ||\n", username, 33 - strlen(username), "");
    printEmptyTableLine();
    printTableLine("Sie koennen diesen Schritt überspringen,");
    printTableLine("druecken Sie dafür einfach Enter.       ");
    printTableLine("                                        ");

    printEndOfTable();
}

void renderCourt()
{

    printColoredString("+---+---+---+---+---+---+---+---+---+", KCYN, 1);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {

            int number = arr[i][j];
            if (j % 3 == 0)
            {
                printColoredString("| ", KCYN, 0);
            }
            else
            {
                printColoredString("| ", KGRN, 0);
            }

            if (i == x && j == y)
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
                if (arr[i][j] > 0)
                {
                    if (userCells[i][j] == 1)
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
                    printf("%s  ", KGRN );
                }
            }
        }
        printf("%s|\n", KCYN);
        if ((i + 1) % 3 == 0)
        {
            printColoredString("+---+---+---+---+---+---+---+---+---+", KCYN, 1);
        }
        else
        {
            printColoredString("+---+---+---+---+---+---+---+---+---+", KGRN, 1 );
        }
    }
    printf("%s \n", gameMessage);
    printf("\n");
}
void renderInfoBox(char *username, int score)
{
    char difficultyText[20] = "";
    switch (difficulty)
    {
        case EASY:
            sprintf(difficultyText, "%s", "Einfach");
            break;
        case MEDIUM:
            sprintf(difficultyText, "%s", "Mittel");
            break;
        case HARD:
            sprintf(difficultyText, "%s", "Schwer");
            break;
        default:
            break;
    }
    int difficultyBoxWith = 8;
    int userBoxWith = 10;
    int bestscoreWidth = 9;
    int remainingBoxWith = 5;
    int remaining = getRemainingCells(arr);
    printColoredString("++=================++=====================++", KCYN, 1);

    // first row
    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);
    printf( "User: %s%*s", username, userBoxWith - strlen(username), "");

    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);
    printf( "Bestscore: %d%*s", score, bestscoreWidth - lenHelper(score), "");

    setPrintingColor(KCYN);
    printf("||\n");

    // second row
    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);
    printf( "Time: %s%*s", "01:30", userBoxWith - strlen("01:30"), "");

    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);
    printf( "Difficulty: %s%*s", difficultyText, difficultyBoxWith - strlen(difficultyText), "");

    setPrintingColor(KCYN);
    printf("||\n");

    // third row
    setPrintingColor(KCYN);
    printf("|| ");

    setPrintingColor(KWHT);
    printf( "Remaining: %d%*s", remaining, remainingBoxWith - lenHelper(remaining), "");

    setPrintingColor(KCYN);
    printf("||                     ||\n");
    printf("++=================++=====================++\n\n");
}

void renderGameMenu()
{
    setPrintingColor(KCYN);
    printf("%s Movement        Commands\n\n", KCYN);

    setPrintingColor(KWHT);
    printf(" > - Right       h - Give a hint\n\n");
    printf(" < - Left        ");
    printColoredString("c - Check\n", getGameStatus(arr) == FILLED ? KWHT : KRED,1);
    printf(" ^ - Top         a - Abbrechen \n\n");
    printf(" v - Down        s - Solve All \n\n");
    printf("                 z - Zurueck \n\n");
    printf("                 k - Spielregeln \n\n");
    printf("                 q - Beenden \n\n");
}

void renderMenu()
{
    setPrintingColor(KCYN);
    printf("++================= Menu  =================++\n");
    printEmptyTableLine();
    if (isGameActive > 0)
    {
        printTableLine("          r - Spiel fortsetzen          ");

    } else {
        printTableLine("          s - Spiel starten             ");
    }

    printEmptyTableLine();
    printTableLine("          b - BestenListe               ");
    printEmptyTableLine();
    printTableLine("          k - Spielregeln               ");
    printEmptyTableLine();
    printTableLine("          q - Beenden                   ");
    printEmptyTableLine();
    printEndOfTable();
}

void print_list(struct score *head){
    struct score * current = head->next; // note: head->next instead of head to skip the first empty element
    setPrintingColor(KCYN);
    printf("|| %s%*s| %s%*s ||\n", "Spieler", 19 - strlen("Spieler"),"", "Score", 18 - strlen("Score"),"");
    printEmptyTableLine();
    while (current->next != NULL) { // note: current->next instead of current to skip the first empty element
        if(current->userId == 2) {
            setPrintingColor(KCYN);
            printf("|| ");
            setPrintingColor(KYEL);
            printf("%s%*s | %d%*s", current->name, 18 - strlen(current->name),"", current->time, 19 - lenHelper(current->time),"");
            setPrintingColor(KCYN);
            printf("||\n");
        } else{
            setPrintingColor(KCYN);
            printf("|| ");
            setPrintingColor(KWHT);
            printf("%s%*s | %d%*s", current->name, 18 - strlen(current->name),"", current->time, 19 - lenHelper(current->time),"");
            setPrintingColor(KCYN);
            printf("||\n");
        }

        current = current->next;
    }
}

void renderDetails(struct score *scores)
{
    setPrintingColor(KCYN);
    printf("++======== Bestenliste ( Einfach) =========++\n");

    print_list(scores);

    printEmptyTableLine();
    printEmptyTableLine();

    printTableLine("z - Zurueck                             ");
    printEmptyTableLine();
    printEndOfTable();
}

void renderDifficultyDialog()
{
    setPrintingColor(KCYN);

    printf("++====== Schwierigkeiteinstellungen  ======++\n");


    printEmptyTableLine();
    printTableLine("            a - Einfach                 ");

    printEmptyTableLine();
    printTableLine("            b - Mittel                  ");


    printEmptyTableLine();
    printTableLine("            c - Schwer                  ");


    printEmptyTableLine();
    printEmptyTableLine();


    printTableLine("        Waehle die gewuenschte          ");
    printTableLine("        Schwierigkeitsstufe aus.        ");
    printEmptyTableLine();
    printEndOfTable();

    printf("||                                        ||\n");
    printf("++========================================++\n");
}


void renderHelpDialog()
{

    printf("%s++=========== Die Spielregeln  ============++%s\n",KCYN,KWHT);

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
    printEmptyTableLine();

    if (isGameActive > 0)
    {
        printTableLine("z - Zurueck zum Spiel                   ");
    }
    else {
        printTableLine("z - Zurueck zum Menue                   ");
    }

    printEndOfTable();
}


int getRemainingCells(int array[][9])
{
    int counter = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (array[i][j] == 0)
            {
                counter++;
            }
        }
    }
    return counter;
}

int lenHelper(int x) {
    if (x >= 50000)     return 6;
    if (x >= 5000)      return 5;
    if (x >= 500)       return 4;
    if (x >= 50)        return 3;
    if (x >= 10)         return 2;
    return 1;
}

void printStartOfLine()
{
    setPrintingColor(KCYN);
    printf("|| ");
}

void printEndOfLine()
{
    setPrintingColor(KCYN);
    printf("||\n");
}

void printTableLine(char text[])
{
    printStartOfLine();

    setPrintingColor(KWHT);
    printf(text);

    printEndOfLine();
}

void printEndOfTable()
{
    setPrintingColor(KCYN);
    printf("++=========================================++\n");
}

void printEmptyTableLine()
{
    printStartOfLine();
    printf("                                        ");
    printEndOfLine();
}