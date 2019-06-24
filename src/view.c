//
// Created by karim on 24.06.19.
//
#include <stdio.h>
#include <string.h>
#include "../headers/view.h"
#include "../headers/shared.h"
#include "../headers/game.h"

int deletedCells[9][9];
int userCells[9][9];
int difficulty;
int isGameActive;

void renderCourt()
{
    printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int number = arr[i][j];
            if (j % 3 == 0)
            {
                printf("%s| ", KRED);
            }
            else
            {
                printf("%s| ", KWHT);
            }

            if (i == x && j == y)
            {
                if (number > 0)
                {
                    printf("%s%d ", KBLU, number);
                }
                else
                {
                    printf("%s| ", KBLU);
                }
            }
            else
            {
                if (arr[i][j] > 0)
                {
                    if (userCells[i][j] == 1)
                    {
                        printf("%s%d ", KCYN, number);
                    }
                    else
                    {
                        printf("%s%d ", KWHT, number);
                    }
                }
                else
                {
                    printf("%s  ", KWHT);
                }
            }
        }
        printf("%s|\n", KRED);
        if ((i + 1) % 3 == 0)
        {
            printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
        }
        else
        {
            printf("%s+---+---+---+---+---+---+---+---+---+\n", KWHT);
        }
    }
    printf("%s \n", gameMessage);
    printf("\n");
}
void renderInfoBox()
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
    int remainingBoxWith = 5;
    int remaining = getRemainingCells(arr);
    printf("%s-----------------------------------------\n", KCYN);
    printf("%s| %sUser: %s%*s%s| %sBestscore: %d      %s|\n", KCYN, KWHT, "Otto", userBoxWith - strlen("Otto"), "", KCYN, KWHT, 257, KCYN);
    printf("%s| %sTime: %s     %s| %sDifficulty: %s%*s%s|\n", KCYN, KWHT, "01:30", KCYN, KWHT, difficultyText, difficultyBoxWith - strlen(difficultyText), "", KCYN);
    printf("%s| %sRemaining: %d%*s%s|                     |\n", KCYN, KWHT, remaining, remainingBoxWith - lenHelper(remaining), "", KCYN);
    printf("%s-----------------------------------------\n\n", KCYN);
}

void renderGameMenu()
{
    printf("%s Movement        Commands\n\n", KCYN);
    printf("%s %c%c%c - Right       h - Give a hint\n\n", KWHT, '\xE2', '\x86', '\x92');
    printf("%s %c%c%c - Left        %ss - Solve \n\n", KWHT, '\xE2', '\x86', '\x90', getGameStatus(arr) == FILLED ? KWHT : KRED);
    printf("%s %c%c%c - Top         a - Abbrechen \n\n", KWHT, '\xE2', '\x86', '\x91');
    printf("%s %c%c%c - Down        b - Zurück \n\n", KWHT, '\xE2', '\x86', '\x93');
}

void renderMenu()
{
    printf("------------ Menu -------------\n");
    if (isGameActive > 0)
    {
        printf("| r - Spiel Fortsetzen        |\n");
    }
    else
    {
        printf("| s - Spiel Starten           |\n");
    }

    printf("| b - BestenListe             |\n");
    printf("| q - Beenden                 |\n");
    printf("-------------------------------\n");
}

void renderDetails()
{
    printf("----------- Details  -----------\n");
    printf("| b - Zurück                   |\n");
    printf("--------------------------------\n");
}

void renderDifficultyDialog()
{
    printf("------ Schwierigkeiteinstellungen -------\n");
    printf("| a - Einfach                           |\n");
    printf("| b - Mittel                            |\n");
    printf("| c - Schwer                            |\n\n");
    printf("| Wähle die gewünschte Schwierigkeits-  |\n");
    printf("| stufe aus.                            |\n");
    printf("-----------------------------------------\n");
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
    if (x >= 100000)     return 6;
    if (x >= 10000)      return 5;
    if (x >= 1000)       return 4;
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}