#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int map1[12][12];                                     //player1 game map
int map2[12][12];                                     //player2 game map


void printmap(int *arr[12][12])
{
    printf("   ");
    for (int i=0; i<10; i++)
    {
        printf(" %d" ,i);
    }
    for (int i=0; i<12; i++)
    {
        if (i==0 || i==11)
        {
            continue;
        }else
        {
        printf("\n%d->" , i-1);
        }
        for (int j=0; j<12; j++)
        {
            switch (*arr[i][j])
            {
                case 0:                                 //not shot yet
                    printf(" -");
                    break;
                case 1:                                 //shot
                    printf(" E");
                    break;
                case 2:
                    printf(" C");                       //comletly destroyed
                    break;
                case 3:
                    printf(" W");                       //safe waters
                    break;
                case 4:
                    printf("");                         //extra
                    break;
            }
        }
    }
}
int x , y;
void shooting(int *arr[12][12] , int x , int y)
{
    while (1)
    {
        scanf("%d %d" ,&x ,&y);
        if (*arr[x+1][y+1]!=1)
        {
            *arr[x+1][y+1]=1;
            return;
        }else
        {
            printf("you cant shoot here!try again!\n");
            continue;
        }
    }
}
void showWater(*arr[12][12] , int x1 , int y1 , int x2 , int y2)
{
    x1++;
    x2++;
    y1++;
    y2++;
    if (x1-x2==0)                                           //ofoghi
    {
        for (int i=y1-1; i<=y2+1; i++)
        {
            if (*arr[x1-1][i]!=4)
            {
                *arr[x1-1][i]=3;
            }
            if (*arr[x1+1][i]!=4)
            {
                *arr[x1+1][i]=3;
            }
        }
        if (*arr[x1][y1-1]!=4)
        {
            *arr[x1][y1-1]=3;
        }
        if (*arr[x1][y1+1]!=4)
        {
            *arr[x1][y2+1]=3;
        }
    }
    if (y1-y2==0)                                           //amoodi
    {
        for (int i = x1-1; i<=x2+1; i++)
        {
            if (*arr[i][y1-1]!=4)
            {
                *arr[i][y1-1]=3;
            }
            if (*arr[i][y1+1]!=4)
            {
                *arr[i][y1+1]=3;
            }
        }
        if (*arr[x1-1][y1]!=4)
        {
            *arr[x1-1][y1]=3;
        }
        if (*arr[x2+1][y1]!=4)
        {
            *arr[x2+1][y1]=3;
        }
    }
    
}
void showmenu()
{
    printf("YO!\nwelcome to my battleship game:)\n\n");
    printf("MENU\n");
    printf("1)Play with a friend\n");
    printf("2)Play with bot\n");
    printf("3)Load game\n");
    printf("4)Load last game\n");
    printf("5)Settings\n");
    printf("6)Score Board\n");
    printf("7)Exit\n");
    
}

int choice;
void entermenu()
{
    while (choice!=7)
    {
        showmenu();
        scanf("%d" , &choice);
        if (choice==1)
        {
            //PLAYER 1:
            int choice1;
            printf("player 1\n");
            printf("choose user:\n");
            printf("1)choose from available users\n");
            printf("2)new user\n");
            scanf("%d" , &choice1);
            if (choice1==1)
            {
                //BLACK BOX FOR SHOWING SAVED PLAYERS
            } else
            {
                //BLACK BOX FOR CREATING NEW USER
            }
            printf("put ships:\n");
            printf("1)auto\n");
            printf("2)manual\n");
            scanf("%d" , &choice1);
            if (choice1==1)
            {
                //BLACK BOX FOR AUTO PUTTING SHIPS
            }else
            {
                //BLACK NOX FOR MANUAL PUTTING SHIPS
            }
            //PLAYER 2:
            int choice2;
            printf("player 2\n");
            printf("choose user:\n");
            printf("1)choose from available users\n");
            printf("2)new user\n");
            scanf("%d" , &choice2);
            if (choice2==1)
            {
                //BLACK BOX FOR SHOWING SAVED PLAYERS
            } else
            {
                //BLACK BOX FOR CREATING NEW USER
            }
            printf("put ships:\n");
            printf("1)auto\n");
            printf("2)manual\n");
            scanf("%d" , &choice2);
            if (choice2==1)
            {
                //BLACK BOX FOR AUTO PUTTING SHIPS
            }else
            {
                //BLACK NOX FOR MANUAL PUTTING SHIPS
            }
            
            //BLACK BOX FOR STARTING THE GAME WITH FRIEND
        }else if (choice==2)
        {
            //BLAKC BOX FOR STARTING THE GAME WITH BOT
        }else if (choice==3)
        {
            //BLACK BOX FOR LOAD GAME
        }else if(choice==4)
        {
            //BLACK BOX FOR LOAD LAST GAME
        }else if (choice==5)
        {
            //SETTINGS
        }else if (choice==6)
        {
            //SCORE BOARD
        }
        
    }
    
    
}
int main()
{
    int *pshots[12][12];
    for (int i =1; i<12; i++)
    {
        for (int j=1; j<10; j++)
        {
            map1[i][j]=0;
        }
    }
    for (int i=0; i<12; i++)
    {
        map1[0][i]=4;
        map1[i][0]=4;
        map1[11][i]=4;
        map1[i][11]=4;
    }
    for (int i=0; i<12; i++)
    {
        for (int j=0; j<12; j++)
        {
            pshots[i][j]=&map1[i][j];
        }
    }
  
}

