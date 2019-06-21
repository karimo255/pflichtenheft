#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<ctype.h>

#ifdef __unix__
#include <termios.h>
#include <unistd.h>

static struct termios new_io;
static struct termios old_io;

/* Funktion schaltet das Terminal in den cbreak-Modus:        */
/* Kontrollflag ECHO und ICANON auf 0 setzen                  */
/* Steuerzeichen: Leseoperation liefert 1 Byte VMIN=1 VTIME=1 */
int cbreak(int fd) {
   /*Sichern unseres Terminals*/
   if((tcgetattr(fd, &old_io)) == -1)
      return -1;
   new_io = old_io;
   /* Wir verändern jetzt die Flags für den cbreak-Modus. */
   new_io.c_lflag = new_io.c_lflag & ~(ECHO|ICANON);
   new_io.c_cc[VMIN] = 1;
   new_io.c_cc[VTIME]= 0;

   /*Jetzt setzen wir den cbreak-Modus*/
   if((tcsetattr(fd, TCSAFLUSH, &new_io)) == -1)
      return -1;
   return 1;
}

int getch(void) {
   int c;

   if(cbreak(STDIN_FILENO) == -1) {
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

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

int isElementInArray(int array[], int ele);
void resetArray(int array[]);
void generateGameData(int array[][9]);
void deleteNumbers(int array[][9], int difficulty);
void renderCourt(int arr[][9]);
void renderInfoBox();
void renderGameMenu();
void navigateTo(int pos);
void renderMenu();
int isElementInBox(int arr[][9], int box_start_row, int box_start_col, int ele);
int generateRandomNumber();
void handleUserInput();
int solve(int arr[][9]);
void resetGameData(int arr[][9]);
int generateNumberByInterval(int x, int y);

int x = 0;
int y = 0;
int arr[9][9] = { 0 };
int exitTheGame = 0;
int isGameActive = 0;

int elementsInSomeColumn[9] = {0};

enum POSITIONS{
  MENU=0,
  INGAME=1,
  DETAILS=2
};

enum DIFFICULTY {
  EASY=4,
  MEDIUM=6,
  HARD=7
};

int currentPosition = MENU;

int main() {
  printf("%d ====>", currentPosition);
  srand(time(NULL));
  generateGameData(arr);
  deleteNumbers(arr,EASY);

  while (!exitTheGame)
  {
    // system("clear");
    switch (currentPosition)
    {
    case MENU:
      renderMenu();
      break;
    
    case INGAME:
      renderInfoBox();
      renderCourt(arr);
      renderGameMenu();
      break;

    case DETAILS:
      // renderMenu();
      break;
    }
    sleep(1);
    handleUserInput();
 }
  
  return 0;
}
void renderMenu(){
  printf("------------ menu -------------\n");
  if(isGameActive){
    printf("| r - Spiel Fortsetzen        |\n");
  } else
  {
    printf("| s - Spiel Starten           |\n");
  }
  
  printf("| b - BestenListe             |\n");
  printf("| x - Beenden                 |\n");
  printf("-------------------------------\n");
}

int generateNumberByInterval(int x, int y) {
  return x + rand() % (y - x +1);
}

void deleteNumbers(int array[][9], int difficulty) {
  for (int x = 1; x <= 3; x++)
  {
    for (int y = 1; y <= 3; y++)
    {
      int tmp = difficulty;
      while (tmp > 0)
      {
        int r = generateNumberByInterval(3 * (x -1), 3 * x -1);
        int c = generateNumberByInterval(3 * (y -1), 3 * y -1);
          array[r][c]= 0;
          tmp--;
      }
    }
  }
}


void navigateTo(int pos){
    switch(pos) { // the real value
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

    if (x > 8){ x = 8;}
    if (x < 0){ x = 0;}
    if (y > 8){ y = 8;}
    if (y < 0){ y = 0;}
}
void handleUserInput(){
  int userInput;
  int pos;

      if (( userInput = getch() ) == '\033') { // if the first value is esc
    getch(); // skip the [
    navigateTo(getch());

}  else {
    printf("current %d \n", currentPosition);
    switch (currentPosition)
    {
    case MENU:
        if(isalpha(userInput)){
        switch (userInput){
        case 's':
          currentPosition = INGAME;
          break;

        case 'b':
          printf("Bestlist\n");
          break;

        case 'q':
          exitTheGame = 1;
          break;
        }
      }

      break;
    
    case INGAME:
      if(isdigit(userInput)){
        // printf("%d", userInput - '0');
        arr[x][y] = userInput - '0';
      } else if(isalpha(userInput)){
        switch (userInput){
        case 'h':
          printf("Give a hint\n");
          break;

        case 's':
          if(solve(arr) > 0){
          printf("solved\n");
          } else{
          printf("not solved\n");
          }
          break;

        case 'b':
          currentPosition = MENU;
          break;
        }
      }
    }
    }
}

int generateRandomNumber(){
  return 1 + rand() % 9;
}

void resetArray(int array[]){
  for (int l = 0; l < 9; l++) {
    array[l] = 0;
  }
}
void resetGameData(int array[][9]){
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) {
      array[x][y] = 0;
    }
  }
}

int isElementInArray(int array[], int ele) {
  for (int x = 0; x < 9; x++) {
    if (array[x] == ele && ele != 0) {
      return x;
    }
  }
  return -1;
}

int isElementInBox(int arr[][9], int box_start_row, int box_start_col, int ele)
{
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++){

			if (arr[row + box_start_row][col + box_start_col] == ele && ele != 0) 
			{
				return 1;
			}
    }
	return -1;
}
void generateGameData(int a[][9]) {
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) {
      int number = generateRandomNumber();
      if (isElementInArray(a[x], number) >= 0 ) { // number darf nur einmal in row vorkommen.
        y--;
        continue;
      }

      resetArray(elementsInSomeColumn);
      for (int l = 0; l < sizeof(elementsInSomeColumn); l++) {
        elementsInSomeColumn[l] = a[l][y];
      }

      // number darf nur einmal in column und box vorkommen.
      if (isElementInArray(elementsInSomeColumn, number) >= 0 || isElementInBox(a, x-x%3, y-y%3, number) >= 0) { 
        resetArray(a[x]);
        x--;
        break;
      }

      a[x][y] = number;
    }
  }
}

int solve(int a[][9]){
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) {
      int number = a[x][y];
      a[x][y] = 0;
      if (isElementInArray(a[x], number) >= 0 || number == 0 ) { // number darf nur einmal in row vorkommen.
        a[x][y]= number;
        return 0;
      }

      resetArray(elementsInSomeColumn);
      for (int l = 0; l < sizeof(elementsInSomeColumn); l++) {
        elementsInSomeColumn[l] = a[l][y];
      }

      // number darf nur einmal in column und box vorkommen.
      if (isElementInArray(elementsInSomeColumn, number) >= 0 || isElementInBox(a, x-x%3, y-y%3, number) >= 0) {
        a[x][y]= number; 
        return 0;
      }
      a[x][y]= number;
    }
  }
  resetGameData(arr);
  return 1;
}

void renderCourt(int arr[][9]) {
  printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int number = arr[i][j];
      if (j % 3 == 0) {
        printf("%s| ", KRED);
      } else {
        printf("%s| ", KWHT);
      }

      if (i==x && j == y) {
          if(number > 0){
            printf("%s%d ", KBLU, number);
          } else
          {
            printf("%s| ", KBLU);
          }
          
      } else {
        if(arr[i][j] > 0){
          printf("%s%d ", KWHT, number);
        } else
        {
         printf("%s  ", KWHT);
        }
        
      }

    }
    printf("%s|\n", KRED);
    if ((i + 1) % 3 == 0) {
      printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
    } else {
      printf("%s+---+---+---+---+---+---+---+---+---+\n", KWHT);
    }
  }
  printf("\n");
}
  void renderInfoBox(){
    printf("%s-------------------------------------\n", KCYN);
    printf("%s| %sUser: %s      %s| %sBestscore: %d  %s|\n", KCYN, KWHT, "Otto", KCYN, KWHT, 257, KCYN);
    printf("%s| %sTime: %s     %s| %sDifficulty: %s%s|\n", KCYN, KWHT, "01:30", KCYN, KWHT, "Easy", KCYN);
    printf("%s| %sRemaining: %d    %s|                 |\n", KCYN, KWHT, 5, KCYN);
    printf("%s-------------------------------------\n\n", KCYN);
  }

  void renderGameMenu(){
    printf("%s Movement        Commands\n\n", KCYN);
    printf("%s %c%c%c - Right       h - Give a hint\n\n", KWHT, '\xE2', '\x86', '\x92' );
    printf("%s %c%c%c - Left        s - Solve \n\n", KWHT, '\xE2', '\x86', '\x90' );
    printf("%s %c%c%c - Top         b - Back\n\n", KWHT, '\xE2', '\x86', '\x91' );
    printf("%s %c%c%c - Down \n\n", KWHT, '\xE2', '\x86', '\x93' );
  }