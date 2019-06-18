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

int indexOf(int array[], int ele);
void generateGameData(int array[][9]);
void renderCourt(int arr[][9]);

int check = 1;

int main() {
  srand(time(NULL));
  int arr[9][9];

  generateGameData(arr);
  renderCourt(arr);
  return 0;
}

int indexOf(int array[], int ele) {
  for (int x = 0; x < 9; x++) {
    if (array[x] == ele) {
      return x;
    }
  }
  return -1;
}

void generateGameData(int a[][9]) {

  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) {
      int ele = 1 + rand() % 9;
      int elementsInSomeRow[x];
      if (indexOf(a[x], ele) >= 0 && check) { // element darf nur einmal in rows vorkommen.
        y--;
        continue;
      }
      for (int l = 0; l < x + 1; l++) {
        elementsInSomeRow[l] = a[l][y];
      }
      if (indexOf(elementsInSomeRow, ele) >= 0) { // element darf nur einmal in columns vorkommen.
        // printf("duplicat at %d,%d \n", x + 1, y + 1);
        check = 0;
        y--;
        continue;
      }
      check = 1;
      a[x][y] = ele;
    }
  }
}

void renderCourt(int arr[][9]) {
  int x = 0;
  int y = 0;

  int i = 0;
  system("clear");
  printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
  for (i; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (j % 3 == 0) {
        printf("%s| ", KRED);
      } else {
        printf("%s| ", KWHT);
      }
      printf("%s%d ", KWHT, arr[i][j]);
    }
    printf("%s|\n", KRED);
    if ((i + 1) % 3 == 0) {
      printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
    } else {
      printf("%s+---+---+---+---+---+---+---+---+---+\n", KWHT);
    }
  }
}