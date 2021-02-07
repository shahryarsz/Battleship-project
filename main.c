#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int map1[10][10]={0};                                     //player1 game map
int map2[10][10]={0};                                     //player2 game map

void printmap(int *arr[10][10])
{
    printf(" ");
    for (int i=0; i<10; i++)
    {
        printf(" %d" ,i);
    }
    for (int i=0; i<10; i++)
    {
        printf("\n%d" , i);
        for (int j=0; j<10; j++)
        {
            switch (*arr[i][j])
            {
                case 0:                                 //water
                    printf(" -");
                    break;
                case 1:                                 //shot
                    printf(" E");
                    break;
                case 2:
                    printf(" C");                       //comletly destroyed
                    break;
            }

        }
    }
}
int x , y;
void shooting(int *arr[10][10] , int x , int y)
{
    while (1)
    {
        scanf("%d %d" ,&x ,&y);
        if (*arr[x][y]!=1)
        {
            *arr[x][y]=1;
            return;
        }else
        {
            printf("you cant shoot here!try again!\n");
            continue;
        }
    }
}

int main()
{
    int *pshots[10][10];
    for (int i=0; i<10; i++)
    {
        for (int j=0; j<10; j++)
        {
            pshots[i][j]=&map1[i][j];
        }
    }
}

