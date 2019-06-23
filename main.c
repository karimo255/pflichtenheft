#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#ifdef __unix__
#include <termios.h>
#include <unistd.h>

static struct termios new_io;
static struct termios old_io;

/* Funktion schaltet das Terminal in den cbreak-Modus:        */
/* Kontrollflag ECHO und ICANON auf 0 setzen                  */
/* Steuerzeichen: Leseoperation liefert 1 Byte VMIN=1 VTIME=1 */
int cbreak(int fd)
{
  /*Sichern unseres Terminals*/
  if ((tcgetattr(fd, &old_io)) == -1)
    return -1;
  new_io = old_io;
  /* Wir verändern jetzt die Flags für den cbreak-Modus. */
  new_io.c_lflag = new_io.c_lflag & ~(ECHO | ICANON);
  new_io.c_cc[VMIN] = 1;
  new_io.c_cc[VTIME] = 0;

  /*Jetzt setzen wir den cbreak-Modus*/
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
  /* alten Terminal-Modus wiederherstellen */
  tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
  return c;
}

/* ... oder wird das Programm unter MS-Windows übersetzt? */
#elif __WIN32__ || _MSC_VER || __MS_DOS__
#include <conio.h>
#endif

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

int isElementInArray(int array[], int ele);
void resetArray(int array[]);
void generateGameData(int array[][9]);
void deleteCells(int array[][9], int difficulty);
void navigateTo(int pos);
int isElementInBox(int arr[][9], int box_start_row, int box_start_col, int ele);
int generateRandomNumber();
void handleUserInput();
int solveGame(int arr[][9]);
void resetGameData(int arr[][9]);
int generateNumberByInterval(int x, int y);
int getGameStatus(int array[][9]);
void solveCell(int array[][9], int x, int y);

void renderMenu();
void renderDetails();
void renderCourt(int arr[][9]);
void renderInfoBox();
void renderGameMenu();
void renderDifficultyDialog();

struct cellKoordinaten
{
  int x;
  int y;
};

int x = 0;
int y = 0;
int exitTheGame = 0;
int isGameActive = 0;
int currentPosition;
int difficulty;
char gameMessage[200] = "";
int arr[9][9] = {0};
int deletedCells[9][9] = {0};
int userCells[9][9] = {0};
int elementsInSomeColumn[9] = {0};

enum DIFFICULTY
{
  EASY = 4,
  MEDIUM = 6,
  HARD = 7
};

enum POSITIONS
{
  MENU = 0,
  INGAME = 1,
  DIFFICULTY_DIALOG = 2,
  DETAILS = 3
};

enum GRID_STATUS
{
  FILLED,
  NOT_FILLED
};

int getGameStatus(int array[][9])
{
  for (int x = 0; x < 9; x++)
  {
    for (int y = 0; y < 9; y++)
    {
      if (array[x][y] == 0)
      {
        return NOT_FILLED;
      }
    }
  }
  return FILLED;
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

int main()
{
  srand(time(NULL));

  currentPosition = MENU;
  difficulty = EASY;

  while (!exitTheGame)
  {
    system("clear");
    switch (currentPosition)
    {
    case MENU:
      renderMenu();
      break;

    case INGAME:
      if (isGameActive == 0)
      {
        resetGameData(arr);
        resetGameData(deletedCells);
        resetGameData(userCells);
        generateGameData(arr);
        deleteCells(arr, difficulty);
        isGameActive = 1;
      }

      renderInfoBox();
      renderCourt(arr);
      renderGameMenu();
      sprintf(gameMessage, "%s", "");
      break;

    case DIFFICULTY_DIALOG:
      renderDifficultyDialog();
      break;

    case DETAILS:
      renderDetails();
      break;
    }

    handleUserInput();
  }
  return 0;
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

int generateNumberByInterval(int x, int y)
{
  return x + rand() % (y - x + 1);
}

void deleteCells(int array[][9], int difficulty)
{
  for (int x = 1; x <= 3; x++)
  {
    for (int y = 1; y <= 3; y++)
    {
      int tmp = difficulty;
      while (tmp > 0)
      {
        int r = generateNumberByInterval(3 * (x - 1), 3 * x - 1);
        int c = generateNumberByInterval(3 * (y - 1), 3 * y - 1);
        if (array[r][c] > 0)
        { // not already deleted
          deletedCells[r][c] = array[r][c];
          userCells[r][c] = 1;
          array[r][c] = 0;
        }
        tmp--;
      }
    }
  }
}

void navigateTo(int pos)
{
  switch (pos)
  { // the real value
  case 'A':
    // code for arrow up
    x--;
    break;
  case 'B':
    // code for arrow down
    x++;
    break;
  case 'C':
    // code for arrow right
    y++;
    break;
  case 'D':
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
  int pos;

  if ((userInput = getch()) == '\033')
  {          // if the first value is esc
    getch(); // skip the [
    navigateTo(getch());
  }
  else
  {
    switch (currentPosition)
    {

    case DIFFICULTY_DIALOG:
      if (userInput == 10) // enter pressed
      {
        currentPosition = INGAME;
      }
      else if (isalpha(userInput))
      {
        switch (userInput)
        {
        case 'a':
          difficulty = EASY;
          currentPosition = INGAME;
          break;

        case 'b':
          difficulty = MEDIUM;
          currentPosition = INGAME;
          break;

        case 'c':
          difficulty = HARD;
          currentPosition = INGAME;
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

          currentPosition = DIFFICULTY_DIALOG;
          break;

        case 'r':
          if (isGameActive == 0)
          {
            break;
          }

          currentPosition = INGAME;
          break;

        case 'b':
          currentPosition = DETAILS;
          break;

        case 'q':
          exitTheGame = 1;
          break;
        }
      }
      break;

    case INGAME:
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

        case 'a':
          resetGameData(arr);
          isGameActive = 0;

        case 'b':
          currentPosition = MENU;
          break;
        }
      }
      break;
    case DETAILS:
      if (isalpha(userInput))
      {
        switch (userInput)
        {
        case 'b':
          currentPosition = MENU;
          printf("Bestlist\n");
          break;
        }
      }
      break;
    }
  }
}

int generateRandomNumber()
{
  return 1 + rand() % 9;
}

void resetArray(int array[])
{
  for (int l = 0; l < 9; l++)
  {
    array[l] = 0;
  }
}
void resetGameData(int array[][9])
{
  for (int x = 0; x < 9; x++)
  {
    for (int y = 0; y < 9; y++)
    {
      array[x][y] = 0;
    }
  }
}

int isElementInArray(int array[], int ele)
{
  for (int x = 0; x < 9; x++)
  {
    if (array[x] == ele && ele != 0)
    {
      return x;
    }
  }
  return -1;
}

int isElementInBox(int arr[][9], int box_start_row, int box_start_col, int ele)
{
  for (int row = 0; row < 3; row++)
    for (int col = 0; col < 3; col++)
    {

      if (arr[row + box_start_row][col + box_start_col] == ele && ele != 0)
      {
        return 1;
      }
    }
  return -1;
}
void generateGameData(int a[][9])
{
  for (int x = 0; x < 9; x++)
  {
    for (int y = 0; y < 9; y++)
    {
      int number = generateRandomNumber();
      if (isElementInArray(a[x], number) >= 0)
      { // number darf nur einmal in row vorkommen.
        y--;
        continue;
      }

      resetArray(elementsInSomeColumn);
      for (int l = 0; l < sizeof(elementsInSomeColumn); l++)
      {
        elementsInSomeColumn[l] = a[l][y];
      }

      // number darf nur einmal in column und box vorkommen.
      if (isElementInArray(elementsInSomeColumn, number) >= 0 || isElementInBox(a, x - x % 3, y - y % 3, number) >= 0)
      {
        resetArray(a[x]);
        x--;
        break;
      }

      a[x][y] = number;
    }
  }
}

void solveCell(int array[][9], int x, int y)
{
  if (deletedCells[x][y] > 0)
  {
    array[x][y] = deletedCells[x][y];
  }
}

int solveGame(int a[][9])
{
  for (int x = 0; x < 9; x++)
  {
    for (int y = 0; y < 9; y++)
    {
      int number = a[x][y];
      a[x][y] = 0;
      if (isElementInArray(a[x], number) >= 0 || number == 0)
      { // number darf nur einmal in row vorkommen.
        a[x][y] = number;
        return 0;
      }

      resetArray(elementsInSomeColumn);
      for (int l = 0; l < sizeof(elementsInSomeColumn); l++)
      {
        elementsInSomeColumn[l] = a[l][y];
      }

      // number darf nur einmal in column und box vorkommen.
      if (isElementInArray(elementsInSomeColumn, number) >= 0 || isElementInBox(a, x - x % 3, y - y % 3, number) >= 0)
      {
        a[x][y] = number;
        ;
        return 0;
      }
      a[x][y] = number;
    }
  }
  resetGameData(arr);
  return 1;
}

void renderCourt(int arr[][9])
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
  printf("%s-----------------------------------------\n", KCYN);
  printf("%s| %sUser: %s      %s| %sBestscore: %d       %s|\n", KCYN, KWHT, "Otto", KCYN, KWHT, 257, KCYN);
  printf("%s| %sTime: %s     %s| %sDifficulty: %s%s  |\n", KCYN, KWHT, "01:30", KCYN, KWHT, difficultyText, KCYN);
  printf("%s| %sRemaining: %d    %s|                      |\n", KCYN, KWHT, 5, KCYN);
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
