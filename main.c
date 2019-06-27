#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


#include "headers/core/view.h"
#include "headers/core/game.h"
#include "headers/shared/shared.h"
#include "headers/services/user_service.h"
#include "headers/services/score_service.h"
#include "headers/services/connection.h"
#include "libs/sqlite3.h"


#define HEIGHT 720
#define WIDTH 400

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
#include <Windows.h>
#include <conio.h>
#endif

int exitTheGame = 0;

sqlite3 *connection;

void resizeWindow() {
#ifdef __unix__
    system("resize -s 45 48");
#endif

#ifdef __WIN32__
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    MoveWindow(hwnd, 550, 50, WIDTH, HEIGHT, TRUE);
#endif
}

void clear_output(){
#ifdef __unix__
  //  system("clear");
#endif

#ifdef __WIN32__
    system("cls");
#endif
}

int arr[9][9];
int deletedCells[9][9];
int userCells[9][9];
int difficulty;
int isGameActive;
int currentPosition;
int os;
char username[50];
int *userID = 0;


int main()
{
    resizeWindow();
    initColors();

    userID = malloc(sizeof(int));

    int rc = sqlite3_open("./sudoku.db", &connection);

    if (rc != SQLITE_OK)
    {
        printf("Failed to open the sudoku.db\n");
        return 1;
    }

    srand(time(NULL));

    struct score *scores;
    scores = malloc(sizeof(score));
    scores->next = NULL;
    getScores(scores);

    currentPosition = MENU;
    difficulty = EASY;

    while (!exitTheGame)
    {
        clear_output();
        switch (currentPosition)
        {
            case MENU:
                renderMenu();
                break;

			case USER_NAME:
				renderUsernameDialog("user:w"
                         "");
				break;

            case IN_GAME:
                if (isGameActive == 0)
                {
                    resetGameData(arr);
                    resetGameData(deletedCells);
                    resetGameData(userCells);
                    generateGameData(arr);
                    deleteCells(arr, difficulty);
                    isGameActive = 1;
                }
                int *p;
                // int s = getBestScoreByUserID(userID, p);
                renderInfoBox(username, 5);
                renderCourt(arr, userCells, x, y, gameMessage);
                renderGameMenu();
                sprintf(gameMessage, "%s", "");
                break;

            case DIFFICULTY_DIALOG:
                renderDifficultyDialog();
                break;

            case DETAILS:
                renderDetails(scores);
                break;
            case HELP:
                renderHelpDialog();
                break;

        }

        handleUserInput();
    }
    printf("Ciao");
    return 0;
}



void navigateTo(int pos)
{
    switch (pos)
    { // the real value
        case UP:
            // code for arrow up
            x--;
            break;
        case DOWN:
            // code for arrow down
            x++;
            break;
        case RIGHT:
            // code for arrow right
            y++;
            break;
        case LEFT:
            // code for arrow left
            y--;
            break;
        case UP_LINUX:
            // code for arrow up
            x--;
            break;
        case DOWN_LINUX:
            // code for arrow down
            x++;
            break;
        case RIGHT_LINUX:
            // code for arrow right
            y++;
            break;
        case LEFT_LINUX:
            // code for arrow left
            y--;
            break;
    }

    if (x > 8)
    {
        x = 8;
    }
    if (x < 0)
    {
        x = 0;
    }
    if (y > 8)
    {
        y = 8;
    }
    if (y < 0)
    {
        y = 0;
    }
}
void handleUserInput()
{
    int userInput;
    char ch;

	if (currentPosition == USER_NAME) {

            int b = 0;
            while ((ch = getch()) != '\n'  && ch != EOF){
                username[b] = ch;
                b++;
            }

            registerUser(username, userID);
            insertScore(userID, 0, difficulty);
            currentPosition = DIFFICULTY_DIALOG;
	}
	else {

		if ((userInput = getch()) == 224) {
			navigateTo(getch()); // windows
		}
		else
		{
			navigateTo(userInput); // linux
			switch (currentPosition)
			{
			case DIFFICULTY_DIALOG:
//				if (userInput == 10) // enter pressed
//				{
//					currentPosition = IN_GAME;
//				}
				if (isalpha(userInput))
				{
					switch (userInput)
					{
					case 'a':
						difficulty = EASY;
						currentPosition = IN_GAME;
						break;

					case 'b':
						difficulty = MEDIUM;
						currentPosition = IN_GAME;
						break;

					case 'c':
						difficulty = HARD;
						currentPosition = IN_GAME;
						break;
					}
				}
				break;

			case MENU:
				if (isalpha(userInput))
				{
					switch (userInput)
					{
					case 's':
						if (isGameActive > 0)
						{
							break;
						}

						currentPosition = USER_NAME;
						break;

					case 'r':
						if (isGameActive == 0)
						{
							break;
						}

						currentPosition = IN_GAME;
						break;

					case 'b':
						currentPosition = DETAILS;
						break;

					case 'k':
						currentPosition = HELP;
						break;

					case 'q':
						exitTheGame = 1;
						break;
					}
				}
				break;

			case IN_GAME:
				if (isdigit(userInput))
				{
					if (userCells[x][y] == 1)
					{
						arr[x][y] = userInput - '0';
					}
				}
				else if (isalpha(userInput))
				{
					switch (userInput)
					{
					case 'h':
						solveCell(arr, x, y);
						break;
					case 's':
						if (getGameStatus(arr) == NOT_FILLED)
						{
							sprintf(gameMessage, "%s", "Sudoku Noch nicht voll.");
							break;
						}

						if (solveGame(arr) == 1)
						{
							sprintf(gameMessage, "%s", "Geloest");
						}
						else
						{
							sprintf(gameMessage, "%s", "Nicht geloest");
						}
						break;

					case 'q':
						exitTheGame = 1;
					case 'a':
						resetGameData(arr);
						isGameActive = 0;

					case 'z':
						currentPosition = MENU;
						break;
					case 'k':
						currentPosition = HELP;
						break;
					}
				}
				break;
			case DETAILS:
				if (isalpha(userInput))
				{
					switch (userInput)
					{
					case 'z':
						currentPosition = MENU;

						break;
					}
				}
				break;
			case HELP:
				if (isalpha(userInput))
				{
					switch (userInput)
					{
					case 'z':
						if (isGameActive > 0)
						{
							currentPosition = IN_GAME;
						}
						else
						{
							currentPosition = MENU;
						}

						break;
					}
				}
				break;
			}
		}
	}
}


