#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
int main()
{
  int i = 0;


  void test(int counter, int newLine){
    for (int i = 0;  i <= counter;  i++, printf("%s", "|   " ));
    if(newLine) printf("\n");
  }

  int x = 0;
  int y = 0;

  for (x; x < 10 ; x++)	{
    printf("1\n");
    i = 0;
    for (i; i < 9; i++)

    {

    system("clear");
    for (int j=0; j < 10; j++) {
        if(x == j ) {
          test(i-1,0); printf("%s", "| * "); test(8-i, 1);// 0
        }else {
          test(9, 1);// 1
        }
    }
    sleep(1);
    }
  }
  return 0;
}
