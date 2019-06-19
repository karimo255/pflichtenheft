#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

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
void renderCourt(int arr[][9]);
int isElementInBox(int arr[][9], int box_start_row, int box_start_col, int ele);
int generateRandomNumber();

int c = 0;
int x = 0;
int y = 5;
int tt = 0;

int main() {
  srand(time(NULL));
  int arr[9][9] = { 0 };

  generateGameData(arr);
  while (1)
  {
    renderCourt(arr);
    sleep(1);
  }
  
  return 0;
}

int generateRandomNumber(){
  return 1 + rand() % 9;
}

void resetArray(int array[]){
  for (int l = 0; l < 9; l++) {
    array[l] = 0;
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
int elementsInSomeColumn[9] = {0};
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

void renderCourt(int arr[][9]) {
  system("clear");
  printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      c++;
      if (j % 3 == 0) {
        printf("%s| ", KRED);
      } else {
        printf("%s| ", KWHT);
      }

      if (i==x && j == y) {
        if (c % 2 == 0){
          printf("%s%d ", KWHT, arr[i][j]);
        } else {
          printf("%s| ", KWHT);
        }
      } else {
        printf("%s%d ", KWHT, arr[i][j]);
      }

    }
    printf("%s|\n", KRED);
    if ((i + 1) % 3 == 0) {
      printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
    } else {
      printf("%s+---+---+---+---+---+---+---+---+---+\n", KWHT);
    }
  }
}