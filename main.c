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

int main()
{
  int arr[9][9];
  int ele;

  int indexOf(int array[], int ele) {
    for(int x= 0; x < 9; x++){
        if(array[x] == ele){
          return x;
        }
    }
    return -1;
  }

  void generateGameData(int a[][9]){
    for(int x= 0; x < 9; x++){
      for(int y =0; y< 9; y++){
        ele = rand() % 10;
        if(indexOf(a[x], ele)>= 0){ // element darf nur einmal vorkommen.
          y--;
          continue;
        } 
          a[x][y] = ele;
      }
    }
  }

  void renderCourt(int arr[][9]){
      int x = 0;
      int y = 0;

      int i = 0;
      system("clear");
      printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
      for (i; i < 9; i++) {
        for (int j=0; j < 9; j++) {
              if(j % 3 == 0){
                printf("%s| ", KRED ,arr[i][j]);
              } else {
                printf("%s| ", KWHT ,arr[i][j]);
              }
              printf("%s%d ", KWHT ,arr[i][j]);
        }
        printf("%s|\n", KRED);
        if((i+1) % 3 == 0){
          printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
        } else{
          printf("%s+---+---+---+---+---+---+---+---+---+\n", KWHT);
        }
    }
  }

  generateGameData(arr);
  renderCourt(arr);
  return 0;
}
