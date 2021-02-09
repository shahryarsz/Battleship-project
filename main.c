#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int map1[12][12];                                     //player1 game map
int map2[12][12];                                     //player2 game map
int attackmap1[12][12];                                //map of player 2 for player 1
int attackmap2[12][12];                                //map of player 1 for player 2
int *pshots1[12][12];
int *pshots2[12][12];
int *attackenemy1[12][12];
int *attackenemy2[12][12];

void syncingMaps(int *map[12][12] ,int *attackmap[12][12])
{
    for (int i=0; i<12; i++)
    {
        for (int j=0; j<12; j++)
        {
            *attackmap[i][j]=*map[i][j]+10;
        }
    }
}

struct node
{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int len;
    struct node *next;
};
struct node * createnode(int x1 , int y1 ,int x2,int y2 ,int len)                 //ships bigger than 1
{
    struct node * newnode;
    newnode = (struct node *)malloc(sizeof(struct node));
    if (newnode==NULL)
    {
        return NULL;
    }
    newnode->start_x =x1;
    newnode->start_y =y1;
    newnode->end_x =x2;
    newnode->end_y =y2;
    newnode->len = len;
    newnode->next=NULL;
    return newnode;
}
struct node * createnode1(int x1 , int y1 , int len)                            //ships with size of 1
{
    struct node * newnode;
    newnode = (struct node *)malloc(sizeof(struct node));
    if (newnode==NULL)
    {
        return NULL;
    }
    newnode->start_x=x1;
    newnode->start_y=y1;
    newnode->len=len;
    newnode->next=NULL;
    return newnode;
}
void addfront(struct node **list , int x1 , int y1 , int x2 , int y2 , int len)   //addfront for more than 1
{
    struct node * newnode = createnode(x1, y1, x2, y2, len);
    newnode->next = *list;
    *list = newnode;
}
void addfront1(struct node **list , int x1 , int y1 ,int len)                     //add front size 1
{
    struct node * newnode = createnode1(x1, y1, len);
    newnode->next= *list;
    *list = newnode;
}
void printships(struct node *list)                                            //printing the list of ships
{
    struct node * cur = list;
    while (cur!=NULL) {
        if (cur->len==1)
        {
            printf("\nx1 is : %d\n" , cur->start_x);
            printf("y1 is : %d\n" , cur->start_y);
            printf("\nfucking len is : %d\n" , cur->len);
            cur = cur->next;
            printf("\n");
            continue;
        }
        printf("x1 is : %d\n" , cur->start_x);
        printf("y1 is : %d\n" , cur->start_y);
        printf("x2 is : %d\n" , cur->end_x);
        printf("y2 is : %d\n" , cur->end_y);
        printf("\nfucking len is : %d\n" , cur->len);
        cur = cur->next;
        printf("\n");
    }
}
/*void deleteship(struct node **list , int zero , int *arr[12][12])
{
    struct node *cur = *list , *prev=NULL;
    if (cur!=NULL && cur->len==zero)
    {
        if (cur->start_x==cur->end_x)                                   //ofoghi
        {
            for (int i = cur->start_y; i<=cur->end_y; i++)
            {
                *arr[cur->start_x+1][i]=2;
            }
        }else                                                           //amoodi
        {
            for (int i = cur->start_x; i<=cur->end_x; i++)
            {
                *arr[i][cur->start_x]=2;
            }
        }
        *list = cur->next;
        free(cur);
        return;
    }
    
    while (cur!=NULL && cur->len!=zero)
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur->start_x==cur->end_x)                                   //ofoghi
    {
        for (int i = cur->start_y; i<=cur->end_y; i++)
        {
            *arr[cur->start_x+1][i]=2;
        }
    }else                                                           //amoodi
    {
        for (int i = cur->start_x; i<=cur->end_x; i++)
        {
            *arr[i][cur->start_x]=2;
        }
    }

    prev->next = cur->next;
    free(cur);
    return;
}*/
void completekill(int *arr[12][12] , int x1 , int y1 , int x2 , int y2 , int len)
{
    if (x1==x2)
    {
        for (int i= y1; i<=y2; i++)
        {
            *arr[x1][i]=2;
        }
    }
    
}
void searchships(int x , int y , struct node **list )
{
    struct node *cur = *list;
    struct node *prev = NULL;
    while (cur!=NULL)
    {
        if (cur->len==1)
        {
            if (x==cur->start_x && y==cur->start_y )
            {
                cur->len--;
                if ((*list)->len==0)
                {
                    struct node * temp = *list;
                    *list = (*list)->next;
                    free(temp);
                    return;
                }
                while (cur!=NULL)
                {
                    if (cur->len==0)
                    {
                        prev->next = cur->next;
                        free(cur);
                        return;
                    }
                    prev = cur;
                    cur = cur->next;
                }
                return;
            }
            cur=cur->next;
        }else
        {
            for (int i =cur->start_x; i<=cur->end_x; i++)
            {
                if (i==x && (y>=cur->start_y && y<=cur->end_y))
                {
                    cur->len--;
                    if (cur->len==0)
                    {
                        if ((*list)->len==0)
                        {
                            struct node * temp = *list;
                            *list = (*list)->next;
                            free(temp);
                            return;
                        }
                        while (cur!=NULL)
                        {
                            if (cur->len==0)
                            {
                                prev->next = cur->next;
                                free(cur);
                                return;
                            }
                            prev = cur;
                            cur = cur->next;
                        }
                        return;
                    }
                }
            }
            cur=cur->next;
        }
        
    }
}
void getship(int *map[12][12],struct node **list,int n , int size)              //getting all ships
{
    while (1)
    {
        printf("\nplease choose %d ship with length of %d:\n",n ,size);
        int x1 ,y1 , x2 ,y2;
        for (int i=0; i<n; i++)
        {
            if (size==1)
            {
                scanf("%d %d" , &x1 , &y1);
                if (*map[x1+1][y1+1]==7  || *map[x1+1][y1+1]==5)
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }else
                {
                    x1++;
                    y1++;
                    for (int i = y1-1; i<=y1+1; i++)
                    {
                        if (*map[x1-1][i]!=4)
                        {
                            *map[x1-1][i]=5;
                        }
                        if (*map[x1+1][i]!=4)
                        {
                            *map[x1+1][i]=5;
                        }
                    }
                    if (*map[x1][y1-1]!=4)
                    {
                        *map[x1][y1-1]=5;
                    }
                    if (*map[x1][y1+1]!=4)
                    {
                        *map[x1][y1+1]=5;
                    }
                    x1--;
                    y1--;
                    *map[x1+1][y1+1]=7;
                    addfront1(list, x1, y1, size);
                    continue;
                }
            }
            scanf("%d %d %d %d" ,&x1 , &y1 ,&x2 ,&y2);                    //conditions are:
            if (x1==x2 && y1==y2)                                        //not same
            {
                printf("wrong input!try again!\n");
                i--;
                continue;
            }else if (x1!=x2 && y1!=y2)                                  //not orib
            {
                printf("wrong input!try again!\n");
                i--;
                continue;
            }else if(x1>9 || y1>9 || x2>9 || y2>9)                       //less than max
            {
                printf("wrong input!try again!\n");
                i--;
                continue;
            }else if(x1<0 || y1<0 || x2<0 || y2<0)                       //more than min
            {
                printf("wrong input!try again!\n");
                i--;
                continue;
            }else if (x1==x2)                                           //checking size ofoghi
            {
                if ((abs(y2-y1)+1)>size)
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }else if((abs(y2-y1)+1)<size)
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }
            }else if (y1==y2)                                           //checking size amoodi
            {
                if ((abs(x2-x1)+1)>size)
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }else if((abs(x2-x1)+1)<size)
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }
            }
            if (x1==x2)
            {
               if (*map[x1+1][y1+1]==7  || *map[x1+1][y1+1]==5)         //checking other ships or borders
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }
                for (int i=y1; i<=y2; i++)                              //inserting ships on map
                {
                    *map[x1+1][i+1]=7;
                }
            }else
            {
                if (*map[x1+1][y1+1]==7 || *map[x1+1][y1+1]==5)         //checking other ships or borders
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }
                for (int i=x1; i<=x2; i++)                              //inserting ships on map
                {
                    *map[i+1][y1+1]=7;
                }
            }
            if (x1-x2==0)                                           //making border for the ship(ofoghi)
            {
                x1++;
                y1++;
                x2++;
                y2++;
                for (int i=y1-1; i<=y2+1; i++)
                {
                    if (*map[x1-1][i]!=4)
                    {
                        *map[x1-1][i]=5;
                    }
                    if (*map[x1+1][i]!=4)
                    {
                        *map[x1+1][i]=5;
                    }
                }
                if (*map[x1][y1-1]!=4)
                {
                    *map[x1][y1-1]=5;
                }
                if (*map[x1][y2+1]!=4)
                {
                    *map[x1][y2+1]=5;
                }
            }
            if (y1-y2==0)                                         //making border for the ship(amoodi)
            {
                x1++;
                y1++;
                x2++;
                y2++;
                for (int i = x1-1; i<=x2+1; i++)
                {
                    if (*map[i][y1-1]!=4)
                    {
                        *map[i][y1-1]=5;
                    }
                    if (*map[i][y1+1]!=4)
                    {
                        *map[i][y1+1]=5;
                    }
                }
                if (*map[x1-1][y1]!=4)
                {
                    *map[x1-1][y1]=5;
                }
                if (*map[x2+1][y1]!=4)
                {
                    *map[x2+1][y1]=5;
                }
            }
            x1--;
            y1--;
            x2--;
            y2--;
            addfront(list, x1, y1, x2, y2, size);
        }
        break;
    }
    
}
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
                case 0: case 10:case 11:case 12:case 17:
                    printf(" -");
                    break;
                case 1:                         //shot
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
                case 7:                               //ships
                    printf(" S");
                    break;
                case 5:case 15:                               //ships border
                    printf(" -");
                    break;
            }
        }
    }
}
int x , y;      //shooting choises
void shooting(int *arr[12][12] , int x , int y , struct node *list)
{
    while (1)
    {
        printf("\nchoose your shot\n");
        scanf("%d %d" ,&x ,&y);
        if (x>9 || x<0 ||y>9 ||y<0 )
        {
            printf("wrong input! try again!\n");
            continue;
        }
        switch (*arr[x+1][y+1])
        {
            case 17:
                *arr[x+1][y+1]=1;
                //searchships(x, y, &list);
                printf("\nwow!nice shot :O !\nas a gift shot again!:)\n");
                printf("\nnow the map is :\n");
                printmap(attackenemy1);
                printships(list);
                continue;
                break;
            case 10:case 15:
                *arr[x+1][y+1]=3;
                printf("\noops,you missed it!\n");
                printf("\nthe map now is: \n\n");
                return;
                break;
            case 7:case 3:case 1:
                printf("\nyou cant shoot here!try again!\n");
                continue;
                break;
        }
    }
}
void showWater(int *arr[12][12] , int x1 , int y1 , int x2 , int y2)
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
        if (*arr[x1][y2+1]!=4)
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
void gameSTART()
{
    while (1)
    {
        //player 1 turn:
        while (1)
        {
            printf("\nenemy map:\n");
            printmap(attackenemy1);
            
            
        }
    }
}
int choice;
void entermenu()
{
    struct node *ships1 = NULL;
    struct node *ships2 = NULL;
    
    while (choice!=7)
    {
        showmenu();
        scanf("%d" , &choice);
        if (choice==1)
        {
            //PLAYER 1:
            int choice1;
            while (1)
            {
                
                printf("player 1\n");
                printf("choose user:\n");
                printf("1)choose from available users\n");
                printf("2)new user\n");
                scanf("%d" , &choice1);
                if (choice1==1)
                {
                    //BLACK BOX FOR SHOWING SAVED PLAYERS
                } else if(choice1==2)
                {
                    //BLACK BOX FOR CREATING NEW USER
                }else{
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            while (1)
            {
                printf("put ships:\n");
                printf("1)auto\n");
                printf("2)manual\n");
                scanf("%d" , &choice1);
                if (choice1==1)
                {
                    //BLACK BOX FOR AUTO PUTTING SHIPS
                }else if(choice1==2)
                {
                    printmap(pshots1);
                    getship(pshots1,&ships1, 1, 5);
                    printmap(pshots1);
                    getship(pshots1,&ships1, 2, 3);
                    printmap(pshots1);
                    getship(pshots1,&ships1, 3, 2);
                    printmap(pshots1);
                    getship(pshots1,&ships1, 4, 1);
                    printmap(pshots1);
                    syncingMaps(pshots1, attackenemy1);
                    for (int i = 0; i<4; i++)
                    {
                        printf("\n");
                        printmap(attackenemy1);
                        shooting(attackenemy1, x, y, ships1);
                    }
                    
                }else
                {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            
            //PLAYER 2:
            int choice2;
            while (1)
            {
                printf("\nplayer 2\n");
                printf("choose user:\n");
                printf("1)choose from available users\n");
                printf("2)new user\n");
                scanf("%d" , &choice2);
                if (choice2==1)
                {
                    //BLACK BOX FOR SHOWING SAVED PLAYERS
                } else if(choice2==2)
                {
                    //BLACK BOX FOR CREATING NEW USER
                }else
                {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            while (1)
            {
                printf("put ships:\n");
                printf("1)auto\n");
                printf("2)manual\n");
                scanf("%d" , &choice2);
                if (choice2==1)
                {
                    //BLACK BOX FOR AUTO PUTTING SHIPS
                }else if(choice2==2)
                {
                    printmap(pshots2);
                    getship(pshots2,&ships2, 1, 5);
                    printmap(pshots2);
                    getship(pshots2,&ships2, 2, 3);
                    printmap(pshots2);
                    getship(pshots2,&ships2, 3, 2);
                    printmap(pshots2);
                    getship(pshots2,&ships2, 4, 1);
                    printmap(pshots2);
                    syncingMaps(pshots2, attackenemy2);
                    
                    
                }else
                {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            //BLACK BOX FOR STATING THE GAME
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
        }else
        {
            printf("wrong input!try again!\n---------------------\n");
            continue;

        }
        
    }
    
}

int main()
{
    //initialization of map1 & map2 & attackmap1 & attackmap2
    for (int i =1; i<12; i++)
    {
        for (int j=1; j<10; j++)
        {
            map1[i][j]=0;
            map2[i][j]=0;
            attackmap1[i][j]=10;
            attackmap2[i][j]=10;
        }
    }
    for (int i=0; i<12; i++)
    {
        //map1
        map1[0][i]=4;
        map1[i][0]=4;
        map1[11][i]=4;
        map1[i][11]=4;
        //map2
        map2[0][i]=4;
        map2[i][0]=4;
        map2[11][i]=4;
        map2[i][11]=4;
        //attackmap1
        attackmap1[0][i]=4;
        attackmap1[i][0]=4;
        attackmap1[11][i]=4;
        attackmap1[i][11]=4;
        //attackmap2
        attackmap2[0][i]=4;
        attackmap2[i][0]=4;
        attackmap2[11][i]=4;
        attackmap2[i][11]=4;
    }
    //initializing pointer to map1
    for (int i=0; i<12; i++)
    {
        for (int j=0; j<12; j++)
        {
            pshots1[i][j]=&map1[i][j];
        }
    }
    //initializing pointer to map2
    for (int i=0; i<12; i++)
    {
        for (int j=0; j<12; j++)
        {
            pshots2[i][j]=&map2[i][j];
        }
    }
    //initializing pointer to attackmap1
    for (int i=0; i<12; i++)
    {
        for (int j=0; j<12; j++)
        {
            attackenemy1[i][j]=&attackmap1[i][j];
        }
    }
    //initializing pointer to attackmap2
    for (int i=0; i<12; i++)
    {
        for (int j=0; j<12; j++)
        {
            attackenemy2[i][j]=&attackmap2[i][j];
        }
    }
    entermenu();
  
}

