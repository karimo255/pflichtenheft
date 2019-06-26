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

int deletedCells[9][9];
int userCells[9][9];
int difficulty;
int isGameActive;


void renderCourt()
{
    printf("%s+---+---+---+---+---+---+---+---+---+\n", KCYN);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
			
            int number = arr[i][j];
            if (j % 3 == 0)
            {
                printf("%s| ", KCYN);
            }
            else
            {
                printf("%s| ", KGRN);
            }

            if (i == x && j == y)
            {
                if (number > 0)
                {
                    printf("%s%d ", KRED, number);
                }
                else
                {
                    printf("%s| ", KRED);
                }
            }
            else
            {
                if (arr[i][j] > 0)
                {
                    if (userCells[i][j] == 1)
                    {
                        printf("%s%d ", KMAG, number);
                    }
                    else
                    {
                        printf("%s%d ", KWHT, number);
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
            printf("%s+---+---+---+---+---+---+---+---+---+\n", KCYN);
        }
        else
        {
            printf("%s+---+---+---+---+---+---+---+---+---+\n", KGRN );
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
    printf("%s++=================++=====================++\n", KCYN);
    printf("%s|| %sUser: %s%*s%s|| %sBestscore: %d      %s||\n", KCYN, KWHT, "Otto", userBoxWith - strlen("Otto"), "", KCYN, KWHT, 257, KCYN);
    printf("%s|| %sTime: %s     %s|| %sDifficulty: %s%*s%s||\n", KCYN, KWHT, "01:30", KCYN, KWHT, difficultyText, difficultyBoxWith - strlen(difficultyText), "", KCYN);
    printf("%s|| %sRemaining: %d%*s%s||                     ||\n", KCYN, KWHT, remaining, remainingBoxWith - lenHelper(remaining), "", KCYN);
    printf("%s++=================++=====================++\n\n", KCYN);
}

void renderGameMenu()
{
    printf("%s Movement        Commands\n\n", KCYN);
    printf("%s %c - Right       h - Give a hint\n\n", KWHT,'>');
    printf("%s %c - Left        %ss - Solve \n\n", KWHT, '<', getGameStatus(arr) == FILLED ? KWHT : KRED);
    printf("%s %c - Top         a - Abbrechen \n\n", KWHT, '^');
    printf("%s %c - Down        z - Zurueck \n\n", KWHT, 'v');
    printf("%s                 k - Spielregeln \n\n", KWHT, 'v');
	printf("                 q - Beenden \n\n", KWHT);
}

void renderMenu()
{
    printf("%s++============= Menu =============++\n",KCYN);
    if (isGameActive > 0)
    {
		printf("%s||                                ||\n",KCYN);
        printf("||      %sr - Spiel Fortsetzen      %s||\n",KWHT,KCYN);
    }
    else
    {
		printf("%s||                                ||\n", KCYN);
        printf("||      %ss - Spiel Starten         %s||\n",KWHT,KCYN);
    }
	printf("%s||                                ||\n", KCYN);
    printf("||      %sb - BestenListe           %s||\n",KWHT,KCYN);
	printf("%s||                                ||\n", KCYN);
    printf("||      %sk - Spielregeln           %s||\n",KWHT,KCYN);
	printf("%s||                                ||\n", KCYN);
    printf("||      %sq - Beenden               %s||\n",KWHT,KCYN);
	printf("%s||                                ||\n", KCYN);
    printf("%s++================================++\n",KCYN);
	printf("%s||                                ||\n",KCYN);
    printf("||      %sb - BestenListe           %s||\n",KWHT,KCYN);
    printf("||      %sq - Beenden               %s||\n",KWHT,KCYN);
	printf("%s||                                ||\n",KCYN);
    printf("%s++================================++\n",KCYN);
}

void print_list(struct score *head){
    struct score * current = head;
    printf("|| ScoreID  | Score  | UserID  | Difficulty||\n", current->scoreID);

    while (current != NULL) {
        if(current->userID == 2) {
            printf("|| %s%d       | %d     | %d       | %d         %s||\n",KYEL, current->scoreID, current->score, current->userID, current->difficulty, KCYN);
        } else{
            printf("|| %d       | %d     | %d       | %d         ||\n", current->scoreID, current->score, current->userID, current->difficulty);
        }

        current = current->next;
    }
}
void renderDetails(struct score *scores)
{
    printf("%s++===============  Details  ===============++\n",KCYN);
    print_list(scores);
    printf("%s||                                         ||\n",KCYN);
    printf("%s||                                         ||\n",KCYN);
    printf("|| %sz - Zurueck                             %s||\n",KWHT,KCYN);
	printf("%s||                                         ||\n",KCYN);
    printf("++=========================================++\n");
}

void renderDifficultyDialog()
{
    printf("%s++====== Schwierigkeiteinstellungen ======++%s\n",KCYN,KWHT);
	printf("%s||                                        ||\n",KCYN);
    printf("%s||             %sa - Einfach                %s||\n",KCYN,KWHT,KCYN);
	printf("%s||                                        ||\n",KCYN);
    printf("%s||             %sb - Mittel                 %s||\n",KCYN,KWHT,KCYN);
	printf("%s||                                        ||\n",KCYN);
    printf("%s||             %sc - Schwer                 %s||\n",KCYN,KWHT,KCYN);
	printf("%s||                                        ||\n",KCYN);
	printf("%s||                                        ||\n",KCYN);
    printf("%s||         %sWaehle die gewuenschte         %s||\n",KCYN,KWHT,KCYN);
    printf("%s||        %sSchwierigkeitsstufe aus.        %s||\n",KCYN,KWHT,KCYN);
	printf("%s||                                        ||\n",KCYN);
    printf("%s++========================================++\n",KCYN);
}


void renderHelpDialog()
{
  printf("%s++=========== Die Spielregeln ============++%s\n",KCYN,KWHT);
  printf("%s|| %sSudoku ist ein Zahlenpuzzle. Das        %s||\n",KCYN,KWHT,KCYN);
	printf("%s|| %sPuzzlefeld besteht aus einem Quadrat,   %s||\n",KCYN,KWHT,KCYN);
	printf("%s|| %sdas in 3x3 Unterquadrate bzw. Bloecke   %s||\n",KCYN,KWHT,KCYN);
	printf("%s|| %seingeteilt ist. Jedes Unterquadrat ist  %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %swieder in 3x3 Felder eingeteilt. Das    %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %sGesamtquadrat enthaelt also 81 Felder   %s||\n",KCYN,KWHT,KCYN);
	printf("%s|| %sin 9 Reihen und 9 Spalten.              %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %s                                        %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %sIn einigen dieser Felder sind schon     %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %szu Beginn Ziffern (1-9) eingetragen.    %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %sTypischerweise sind 22 bis 36 Felder    %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %svon 81 moeglichen vorgegeben. Das       %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %sPuzzle muss nun so vervollstaendigt     %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %swerden, dass:                           %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %s- in jeder Zeile,                       %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %s- in jeder Spalte und                   %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %s- in jedem der neun Bloecke jede Ziffer %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %s von 1 bis 9 genau einmal auftritt.     %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %s                                        %s||\n",KCYN,KWHT,KCYN);
  printf("%s|| %s----------------------------------------%s||\n",KCYN,KWHT,KCYN);

  if (isGameActive > 0)
  {
    printf("%s|| %sz - Zurueck zum Spiel                    %s||\n",KCYN,KWHT,KCYN);
  }
  else
  {
    printf("%s|| %sz - Zurueck zum Menue                   %s||\n",KCYN,KWHT,KCYN);
  }
  printf("%s++=========================================++\n",KCYN);
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
