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
  int i = 0;
  int arr[9][9];

int indexOf(int array[], int ele) {
  for(int x= 0; x < 9; x++){
      if(array[x] == ele){
        return 1;
      }
  }
  return 0;
}

  for(int x= 0; x < 9; x++){
    for(int y =0; y< 9; y++){
      int ele = rand() % 10;
      if(indexOf(arr[x], ele)> 0){
        y--;
      } else{
        arr[x][y] = ele;
      }
    }
  }

  int x = 0;
  int y = 0;

    i = 0;
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
    sleep(1);
  return 0;
}
