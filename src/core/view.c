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

void renderUsernameDialog() {
	printf("Bitte Name eingeben: ");
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
    printColoredString("s - Solve\n", getGameStatus(arr) == FILLED ? KWHT : KRED,1);
    printf(" ^ - Top         a - Abbrechen \n\n");
    printf(" v - Down        z - Zurueck \n\n");
    printf("                 k - Spielregeln \n\n");
    printf("                 q - Beenden \n\n");
}

void renderMenu()
{
    setPrintingColor(KCYN);
    printf("++============= Menu =============++\n");
    printf("||                                ||\n");
    printf("||      ");

    setPrintingColor(KWHT);

    if (isGameActive > 0)
    {
        printf("r - Spiel fortsetzen");
        setPrintingColor(KCYN);
        printf("      ||\n");

    } else {
        printf("s - Spiel starten");
        setPrintingColor(KCYN);
        printf("         ||\n");
    }


    printf("||      ");
    setPrintingColor(KWHT);
    printf("b - BestenListe");
    setPrintingColor(KCYN);
    printf("           ||\n");

    printf("||      ");
    setPrintingColor(KWHT);
    printf("k - Spielregeln");
    setPrintingColor(KCYN);
    printf("           ||\n");

    printf("||      ");
    setPrintingColor(KWHT);
    printf("q - Beenden");
    setPrintingColor(KCYN);
    printf("               ||\n");

    printf("++================================++\n");
}

void print_list(struct score *head){
    struct score * current = head;
    setPrintingColor(KCYN);
    printf("|| ScoreID  | Score  | UserID  | Difficulty||\n");

    while (current != NULL) {
        if(current->userID == 2) {
            setPrintingColor(KCYN);
            printf("|| ");
            setPrintingColor(KYEL);
            printf("%d       | %d     | %d       | %d         ", current->scoreID, current->score, current->userID, current->difficulty);
            setPrintingColor(KCYN);
            printf("||\n");
        } else{
            setPrintingColor(KCYN);
            printf("|| ");
            setPrintingColor(KWHT);
            printf("%d       | %d     | %d       | %d         ", current->scoreID, current->score, current->userID, current->difficulty);
            setPrintingColor(KCYN);
            printf("||\n");
        }

        current = current->next;
    }
}
void renderDetails(struct score *scores)
{
    setPrintingColor(KCYN);
    printf("++===============  Details  ===============++\n");

    print_list(scores);

    setPrintingColor(KCYN);
    printf("||                                         ||\n");
    printf("||                                         ||\n");

    setPrintingColor(KCYN);
    printf("|| ");
    setPrintingColor(KWHT);
    printf("z - Zurueck                             ");
    setPrintingColor(KCYN);
    printf("||\n");

    setPrintingColor(KCYN);
    printf("||                                         ||\n");
    printf("++=========================================++\n");
}

void renderDifficultyDialog()
{
    setPrintingColor(KCYN);
    printf("++====== Schwierigkeiteinstellungen ======++\n");
    printf("||                                        ||\n");

    setPrintingColor(KCYN);
    printf("||             ");
    setPrintingColor(KWHT);
    printf("a - Einfach                ");
    setPrintingColor(KCYN);
    printf("||\n");

    setPrintingColor(KCYN);
    printf("||                                        ||\n");

    setPrintingColor(KCYN);
    printf("||             ");
    setPrintingColor(KWHT);
    printf("b - Mittel                 ");
    setPrintingColor(KCYN);
    printf("||\n");


    setPrintingColor(KCYN);
    printf("||                                        ||\n");

    setPrintingColor(KCYN);
    printf("||             ");
    setPrintingColor(KWHT);
    printf("c - Schwer                 ");
    setPrintingColor(KCYN);
    printf("||\n");

    setPrintingColor(KCYN);
    printf("||                                        ||\n");
    printf("||                                        ||\n");


    setPrintingColor(KCYN);
    printf("||         ");
    setPrintingColor(KWHT);
    printf("Waehle die gewuenschte         ");
    setPrintingColor(KCYN);
    printf("||\n");

    setPrintingColor(KCYN);
    printf("||         ");
    setPrintingColor(KWHT);
    printf("Schwierigkeitsstufe aus.       ");
    setPrintingColor(KCYN);
    printf("||\n");

    printf("||                                        ||\n");
    printf("++========================================++\n");
}


void renderHelpDialog()
{
    setPrintingColor(KCYN);
    printf("+------------ Die Spielregeln -------------+\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("Sudoku ist ein Zahlenpuzzle. Das         ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("Puzzlefeld besteht aus einem Quadrat,    ");
    setPrintingColor(KCYN);

    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("das in 3×3 Unterquadrate bzw. Bloecke    ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("eingeteilt ist. Jedes Unterquadrat ist   ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("wieder in 3×3 Felder eingeteilt. Das     ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("Gesamtquadrat enthält also 81 Felder     ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("in 9 Reihen und 9 Spalten.               ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("|                                          |\n");


    printf("| ");
    setPrintingColor(KWHT);
    printf("In einigen dieser Felder sind schon      ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("zu Beginn Ziffern (1-9) eingetragen.     ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("Typischerweise sind 22 bis 36 Felder von ");
    setPrintingColor(KCYN);
    printf("|\n");


    printf("| ");
    setPrintingColor(KWHT);
    printf("81 moeglichen vorgegeben. Das Puzzle     ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("muss nun so vervollstaendigt werden,     ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("| ");
    setPrintingColor(KWHT);
    printf("dass:                                    ");
    setPrintingColor(KCYN);
    printf("|\n");

    printf("|                                          |\n");
    printf("|                                          |\n");
    printf("|                                          |\n");


    if (isGameActive > 0)
    {
        printf("| ");
        setPrintingColor(KWHT);
        printf("z - Zurueck zum Spiel                    ");
        setPrintingColor(KCYN);
        printf("|\n");
    }
    else
    {
        printf("| ");
        setPrintingColor(KWHT);
        printf("z - Zurueck zum Menu                     ");
        setPrintingColor(KCYN);
        printf("|\n");
    }
    printf("+------------------------------------------+\n");
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