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

  for(int x= 0; x < 9; x++){
    for(int y =0; y< 9; y++){
    arr[x][y] = rand() % 10;
    }
  }

  int x = 0;
  int y = 0;
	int c;

    printf("1\n");
    i = 0;
    system("clear");
    printf("%s+---+---+---+---+---+---+---+---+---+\n", KRED);
    for (i; i < 9; i++)
    {


    for (int j=0; j < 9; j++) {
          // test(i-1,0); printf("| %d ", arr[i][j]); test(8-i, 1);// 0
          if(j % 3 == 0){
          printf("%s| ", KRED ,arr[i][j]);

          } else
          {
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
