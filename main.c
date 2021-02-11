#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int map1[12][12];                                     //player1 game map
int map2[12][12];                                     //player2 game map
int map3[12][12];
int attackmap1[12][12];                                //map of player 2 for player 1
int attackmap2[12][12];                                //map of player 1 for player 2
int attackmap3[12][12];
int *pshots1[12][12];
int *pshots2[12][12];
int *pshots3[12][12];
int *attackenemy1[12][12];
int *attackenemy2[12][12];
int *attackenemy3[12][12];
int ID = 101;
struct node *ships1 = NULL;
struct node *ships2 = NULL;
struct mode *ships3 = NULL;

void syncingMaps(int *map[12][12], int *attackmap[12][12])                //syncing main map and enemy map
{
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            *attackmap[i][j] = *map[i][j] + 20;
        }
    }
}

void showWater(int *arr[12][12], int x1, int y1, int x2, int y2);

struct node
{
    int id;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int len;
    struct node *next;
};

struct node *createnode(int id, int x1, int y1, int x2, int y2, int len)        //ships bigger than 1
{
    struct node *newnode;
    newnode = (struct node *) malloc(sizeof(struct node));
    if (newnode == NULL)
    {
        return NULL;
    }
    newnode->id = id;
    newnode->start_x = x1;
    newnode->start_y = y1;
    newnode->end_x = x2;
    newnode->end_y = y2;
    newnode->len = len;
    newnode->next = NULL;
    return newnode;
}

struct node *createnode1(int id, int x1, int y1, int len)                    //ships with size of 1
{
    struct node *newnode;
    newnode = (struct node *) malloc(sizeof(struct node));
    if (newnode == NULL)
    {
        return NULL;
    }
    newnode->id = id;
    newnode->start_x = x1;
    newnode->start_y = y1;
    newnode->end_x = x1;
    newnode->end_y = y1;
    newnode->len = len;
    newnode->next = NULL;
    return newnode;
}

void addfront(struct node **list, int id, int x1, int y1, int x2, int y2, int len) //addfront for more than 1
{
    struct node *newnode = createnode(id, x1, y1, x2, y2, len);
    newnode->next = *list;
    *list = newnode;
}

void addfront1(struct node **list, int id, int x1, int y1, int len)                   //add front size 1
{
    struct node *newnode = createnode1(id, x1, y1, len);
    newnode->next = *list;
    *list = newnode;
}

void printships(struct node *list)                                            //printing the list of ships
{
    struct node *cur = list;
    while (cur != NULL)
    {
        if (cur->len == 0)
        {
            printf("x is %d y is %d\n\n", cur->start_x, cur->start_y);
            cur = cur->next;
        } else
        {
            printf("x1 is : %d\n", cur->start_x);
            printf("y1 is : %d\n", cur->start_y);
            printf("x2 is : %d\n", cur->end_x);
            printf("y2 is : %d\n", cur->end_y);
            cur = cur->next;
            printf("\n");
        }
    }
}

void searchships(int id, struct node **list, int turn)              //searching and destroying ships
{
    struct node *cur = *list;
    struct node *prev = NULL;
    while (cur->id != id)
    {
        prev = cur;
        cur = cur->next;
    }
    cur->len--;
    if (cur->len == 0)
    {
        if (turn == 1)
        {
            showWater(attackenemy2, cur->start_x, cur->start_y, cur->end_x, cur->end_y);
        } else if (turn==2)
        {
            showWater(attackenemy1, cur->start_x, cur->start_y, cur->end_x, cur->end_y);
        }else if(turn==3)
        {
            showWater(attackenemy3, cur->start_x, cur->start_y, cur->end_x, cur->end_y);
        }
        if (prev == NULL)
        {
            (*list) = (*list)->next;
            free(cur);
        } else
        {
            prev->next = cur->next;
            free(cur);
        }
    }

}
void mapbot(int *arr[12][12] ,struct node **list)
{
    for (int i =1; i<=5; i++)                           //ship size 5
    {
        *arr[1][i]=101;
    }
    for (int i =1; i<=3; i++)                           //ship size 3
    {
        *arr[3][i]=102;
    }
    for (int i=5; i<=7; i++)                            //ship size 3
    {
        *arr[5][i]=103;
    }
    for (int i=1; i<=2; i++)
    {
        *arr[i][10]=104;
    }
    for (int i=8; i<=9; i++)
    {
        *arr[i][1]=105;
    }
    for (int i = 8; i<=9; i++)
    {
        *arr[i][6]=106;
    }
    *arr[2][7]=107;
    *arr[6][1]=108;
    *arr[7][10]=109;
    *arr[10][10]=110;
    addfront(list, 101, 0 , 0 , 0, 4, 5);
    addfront(list, 102, 2, 0, 2, 2, 3);
    addfront(list, 103, 4 , 4, 4 , 6, 3);
    addfront(list, 104, 0, 9, 1, 9, 2);
    addfront(list, 105, 7, 0, 8, 0, 2);
    addfront(list, 106, 7, 5, 8, 5, 2);
    addfront1(list, 107, 1, 6, 1);
    addfront1(list, 108, 5, 0, 1);
    addfront1(list, 109, 6, 9, 1);
    addfront1(list, 110, 9, 9, 1);
}
/*void randmap(int *map[12][12] , struct node **list)
{
    srand(time(NULL));
    int x1 , y1 , m , x2 , y2;
    m = rand()%2;
    if (m==0)                                                       //ofoghi
    {
        x1 = rand()%10;
        y1 = rand()%10;
        if (x1+4<9)
        {
            x2 = x1+4;
            y2 = y1;
        }else
        {
            x2 = x1-4;
            y2 = y1;
        }
        //conditions
        while (1)
        {
            if ((*map[x1 + 1][y1 + 1] > 100 && *map[x1 + 1][y1 + 1] < 115) || *map[x1 + 1][y1 + 1] == 5)
            {
                continue;
            }
            for (int i = y1; i <= y2; i++)                              //inserting ships on map
            {
                *map[x1 + 1][i + 1] = ID;
            }
            x1++;
            y1++;
            x2++;
            y2++;
            for (int i = y1 - 1; i <= y2 + 1; i++)                      //making border for the ship
            {
                if (*map[x1 - 1][i] != 4)
                {
                    *map[x1 - 1][i] = 5;
                }
                if (*map[x1 + 1][i] != 4)
                {
                    *map[x1 + 1][i] = 5;
                }
            }
            if (*map[x1][y1 - 1] != 4)
            {
                *map[x1][y1 - 1] = 5;
            }
            if (*map[x1][y2 + 1] != 4)
            {
                *map[x1][y2 + 1] = 5;
            }
            x1--;
            y1--;
            x2--;
            y2--;
            addfront(list, ID, x1, y1, x2, y2, 5);
            ID++;
        }
    }
}*/
void getship(int *map[12][12], struct node **list, int n, int size)              //getting all ships
{
    while (1)
    {
        printf("\nplease choose %d ship with length of %d:\n", n, size);
        int x1, y1, x2, y2;
        for (int i = 0; i < n; i++)
        {
            if (size == 1)
            {
                scanf("%d %d", &x1, &y1);
                if ((*map[x1 + 1][y1 + 1] > 100 && *map[x1 + 1][y1 + 1] < 115) || *map[x1 + 1][y1 + 1] == 5)
                {
                    printf("wrong input!try again!\n");
                    i--;
                } else
                {
                    x1++;
                    y1++;
                    for (int i = y1 - 1; i <= y1 + 1; i++)
                    {
                        if (*map[x1 - 1][i] != 4)
                        {
                            *map[x1 - 1][i] = 5;
                        }
                        if (*map[x1 + 1][i] != 4)
                        {
                            *map[x1 + 1][i] = 5;
                        }
                    }
                    if (*map[x1][y1 - 1] != 4)
                    {
                        *map[x1][y1 - 1] = 5;
                    }
                    if (*map[x1][y1 + 1] != 4)
                    {
                        *map[x1][y1 + 1] = 5;
                    }
                    x1--;
                    y1--;
                    *map[x1 + 1][y1 + 1] = ID;
                    addfront1(list, ID, x1, y1, size);
                    ID++;
                }
            } else
            {

                scanf("%d %d %d %d", &x1, &y1, &x2, &y2);                    //conditions are:
                if (x1 == x2 && y1 > y2)
                {
                    printf("wrong input!try again!\n");                        //end and beginning
                    i--;
                    continue;
                } else if (y1 == y2 && x1 > x2)
                {
                    printf("wrong input!try again!\n");                        //end and beginning
                    i--;
                    continue;
                }
                if (x1 == x2 && y1 == y2)                                        //not same
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                } else if (x1 != x2 && y1 != y2)                                  //not orib
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                } else if (x1 > 9 || y1 > 9 || x2 > 9 || y2 > 9)                       //less than max
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                } else if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0)                       //more than min
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                } else if (x1 == x2)                                           //checking size ofoghi
                {
                    if ((abs(y2 - y1) + 1) > size) {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    } else if ((abs(y2 - y1) + 1) < size) {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    }
                } else if (y1 == y2)                                           //checking size amoodi
                {
                    if ((abs(x2 - x1) + 1) > size) {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    } else if ((abs(x2 - x1) + 1) < size) {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    }
                }
                if (x1 == x2)                                        //checking other ships or borders
                {
                    if ((*map[x1 + 1][y1 + 1] > 100 && *map[x1 + 1][y1 + 1] < 115) || *map[x1 + 1][y1 + 1] == 5)
                    {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    }
                    for (int i = y1; i <= y2; i++)                              //inserting ships on map
                    {
                        *map[x1 + 1][i + 1] = ID;
                    }
                } else {
                    if (*map[x1 + 1][y1 + 1] == 7 || *map[x1 + 1][y1 + 1] == 5)
                        //checking other ships or borders
                    {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    }
                    for (int i = x1; i <= x2; i++)                              //inserting ships on map
                    {
                        *map[i + 1][y1 + 1] = ID;
                    }
                }
                if (x1 - x2 == 0)                                           //making border for the ship(ofoghi)
                {
                    x1++;
                    y1++;
                    x2++;
                    y2++;
                    for (int i = y1 - 1; i <= y2 + 1; i++)
                    {
                        if (*map[x1 - 1][i] != 4)
                        {
                            *map[x1 - 1][i] = 5;
                        }
                        if (*map[x1 + 1][i] != 4)
                        {
                            *map[x1 + 1][i] = 5;
                        }
                    }
                    if (*map[x1][y1 - 1] != 4)
                    {
                        *map[x1][y1 - 1] = 5;
                    }
                    if (*map[x1][y2 + 1] != 4)
                    {
                        *map[x1][y2 + 1] = 5;
                    }
                }
                if (y1 - y2 == 0)                                         //making border for the ship(amoodi)
                {
                    x1++;
                    y1++;
                    x2++;
                    y2++;
                    for (int i = x1 - 1; i <= x2 + 1; i++)
                    {
                        if (*map[i][y1 - 1] != 4)
                        {
                            *map[i][y1 - 1] = 5;
                        }
                        if (*map[i][y1 + 1] != 4)
                        {
                            *map[i][y1 + 1] = 5;
                        }
                    }
                    if (*map[x1 - 1][y1] != 4)
                    {
                        *map[x1 - 1][y1] = 5;
                    }
                    if (*map[x2 + 1][y1] != 4)
                    {
                        *map[x2 + 1][y1] = 5;
                    }
                }
                x1--;
                y1--;
                x2--;
                y2--;
                addfront(list, ID, x1, y1, x2, y2, size);
                ID++;
            }
        }
        break;
    }

}

void printmap(int *arr[12][12])                                         //printing maps
{
    printf("   ");
    for (int i = 0; i < 10; i++)
    {
        printf(" %d", i);
    }
    for (int i = 0; i < 12; i++)
    {
        if (i == 0 || i == 11)
        {
            continue;
        } else
        {
            printf("\n%d->", i - 1);
        }
        for (int j = 0; j < 12; j++)
        {
            switch (*arr[i][j])
            {
                case 0:
                case 10:
                case 11:
                case 12:
                case 20:
                    printf(" -");
                    break;
                case 1:                                       //shot
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
                case 5:
                case 25:                           //ships border
                    printf(" -");
                    break;
                case 101:
                case 102:
                case 103:
                case 104:
                case 105:
                case 106:
                    printf(" S");                                       //ships codes for map
                    break;
                case 107:
                case 108:
                case 109:
                case 110:
                case 111:
                    printf(" S");                                       //ships codes for map
                    break;
                case 121:
                case 122:
                case 123:
                case 124:
                case 125:
                case 126:
                    printf(" -");                                       //ships codes for enemy map
                    break;
                case 127:
                case 128:
                case 129:
                case 130:
                case 131:
                    printf(" -");                                       //ships codes for enemy map
                    break;

            }


        }
    }
}

int x, y;      //shooting choises
void shooting(int *arr[12][12], int x, int y, struct node **list, int turn)
{
    printf("\nyour enemy map is:\n");
    printmap(arr);
    while (*list != NULL)
    {
        printf("\nchoose your shot\n");
        scanf("%d %d", &x, &y);
        if (x > 9 || x < 0 || y > 9 || y < 0)
        {
            printf("wrong input! try again!\n");
            continue;
        }
        if (*arr[x + 1][y + 1] == 20 || *arr[x + 1][y + 1] == 25)               //shots on water or borders
        {
            *arr[x + 1][y + 1] = 3;
            printf("\n\noops,you missed it!\n\n");
            printf("the map now is: \n\n");
            printmap(arr);
            return;
        } else if (*arr[x + 1][y + 1] == 3 || *arr[x + 1][y + 1] == 1)                     //already shot
        {
            printf("\nyou cant shoot here!try again!\n");
        } else
        {
            int temp = *arr[x + 1][y + 1];                                       //correct shot
            *arr[x + 1][y + 1] = 1;
            searchships(temp - 20, list, turn);
            printf("\nwow!nice shot :O !\nas a gift shot again!:)\n");
            printf("\nnow the map is :\n");
            printmap(arr);
        }
    }
}
void botshooting(int *arr[12][12] ,struct node **list ,int turn)
{
    while (*list!=NULL)
    {
        srand(time(NULL));
        x = rand()%10;
        y= rand()%10;
        if (*arr[x + 1][y + 1] == 20 || *arr[x + 1][y + 1] == 25)               //shots on water or borders
        {
            *arr[x + 1][y + 1] = 3;
            printf("\n\nyour map is :\n\n");
            printmap(arr);
            return;
        } else if (*arr[x + 1][y + 1] == 3 || *arr[x + 1][y + 1] == 1){
            printf("\nyou cant shoot here!try again!\n");
        } else
        {
            int temp = *arr[x + 1][y + 1];                                       //correct shot
            *arr[x + 1][y + 1] = 1;
            searchships(temp - 20, list, turn);
            printf("\n\nyour map is :\n\n");
            printmap(arr);
            printf("\n\nthe bot shoud shoot you again:(\n!please enter shit to continue!\n\n");
            int boom;
            scanf("%d" , &boom);
        }
    }
}

void showWater(int *arr[12][12], int x1, int y1, int x2, int y2)        //showing the borders water
{
    x1++;
    y1++;
    x2++;
    y2++;
    if (x1 == x2)                                           //ofoghi
    {
        for (int i = y1 - 1; i <= y2 + 1; i++)
        {
            if (*arr[x1 - 1][i] != 4)
            {
                *arr[x1][i] = 2;
                *arr[x1 - 1][i] = 3;
            }
            if (*arr[x1 + 1][i] != 4)
            {
                *arr[x1][i] = 2;
                *arr[x1 + 1][i] = 3;
            }
        }
        if (*arr[x1][y1 - 1] != 4)
        {
            *arr[x1][y1 - 1] = 3;
        }
        if (*arr[x1][y2 + 1] != 4)
        {
            *arr[x1][y2 + 1] = 3;
        }
    }
    if (y1 - y2 == 0)                                           //amoodi
    {
        for (int i = x1 - 1; i <= x2 + 1; i++)
        {
            if (*arr[i][y1 - 1] != 4)
            {
                *arr[i][y1] = 2;
                *arr[i][y1 - 1] = 3;
            }
            if (*arr[i][y1 + 1] != 4)
            {
                *arr[i][y1] = 2;
                *arr[i][y1 + 1] = 3;
            }
        }
        if (*arr[x1 - 1][y1] != 4)
        {
            *arr[x1 - 1][y1] = 3;
        }
        if (*arr[x2 + 1][y1] != 4)
        {
            *arr[x2 + 1][y1] = 3;
        }
    }
}

void showmenu()                                                                 //showing main menu
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

void gameSTART()                                                                //starting game
{
    printf("\n\nLETS START THE GAME!\n\n");
    while (1)
    {
        //player 1 turn:
        printf("\n\nplayer 1 turn:\n\n");
        shooting(attackenemy2, x, y, &ships2, 1);
        if (ships2 == NULL)
        {
            printf("\n\nBOOOM!\n\nPlayer 1 win!!!\n\n");
            return;
        }
        
        //player 2 turn
        printf("\n\nplayer 2 turn:\n\n");
        while (1)
        {
            shooting(attackenemy1, x, y, &ships1, 2);
            if (ships1 == NULL)
            {
                printf("\n\nBOOOM!\n\nPlayer 2 win!!!\n\n");
                return;
            }
            break;
        }
    }
}
void gameSTARTbot()
{
    printf("\n\nLETS START THE GAME!\n\n");
    while (1)
    {
        //player 1 turn:
        printf("\n\nplayer 1 turn:\n\n");
        shooting(attackenemy3, x, y, &ships3, 3);
        if (ships3 == NULL)
        {
            printf("\n\nBOOOM!\n\nYou win!!!\n\n");
            return;
        }
        
        
        // bot turn
        
        botshooting(attackenemy1, &ships1, 2);
        if (ships1==NULL)
        {
            printf("\n\nOops!\n\nYou lose!!!\n\n");
            return;
        
        }
    }
    
}

int choice;
void entermenu()
{
    while (choice != 7)
    {
        showmenu();
        scanf("%d", &choice);
        if (choice == 1)
        {
            //PLAYER 1:
            int choice1;
            while (1)
            {

                printf("player 1\n");
                printf("choose user:\n");
                printf("1)choose from available users\n");
                printf("2)new user\n");
                scanf("%d", &choice1);
                if (choice1 == 1)
                {
                    //BLACK BOX FOR SHOWING SAVED PLAYERS
                } else if (choice1 == 2)
                {
                    //BLACK BOX FOR CREATING NEW USER
                } else {
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
                scanf("%d", &choice1);
                if (choice1 == 1)
                {
                    //BLACK BOX FOR AUTO PUTTING SHIPS
                } else if (choice1 == 2)
                {
                    printmap(pshots1);
                    printf("\n(Pay attention! first insert the beginning of ship then end of it!)\n\n");
                    getship(pshots1, &ships1, 1, 5);
                    printmap(pshots1);
                    getship(pshots1, &ships1, 2, 3);
                    printmap(pshots1);
                    getship(pshots1, &ships1, 3, 2);
                    printmap(pshots1);
                    getship(pshots1, &ships1, 4, 1);
                    printmap(pshots1);
                    syncingMaps(pshots1, attackenemy1);
                    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

                } else
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
                scanf("%d", &choice2);
                if (choice2 == 1)
                {
                    //BLACK BOX FOR SHOWING SAVED PLAYERS
                } else if (choice2 == 2)
                {
                    //BLACK BOX FOR CREATING NEW USER
                } else
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
                scanf("%d", &choice2);
                if (choice2 == 1)
                {
                    //BLACK BOX FOR AUTO PUTTING SHIPS
                } else if (choice2 == 2)
                {
                    ID = 101;
                    printmap(pshots2);
                    printf("\n(Pay attention! first insert the beginning of ship then end of it!)\n\n");
                    getship(pshots2, &ships2, 1, 5);
                    printmap(pshots2);
                    getship(pshots2, &ships2, 2, 3);
                    printmap(pshots2);
                    getship(pshots2, &ships2, 3, 2);
                    printmap(pshots2);
                    getship(pshots2, &ships2, 4, 1);
                    printmap(pshots2);
                    syncingMaps(pshots2, attackenemy2);
                    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");


                } else
                {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            gameSTART();
        } else if (choice == 2)
        {
            //STARTING THE GAME WITH BOT
            while (1)
            {
                int choice1;
                printf("put ships:\n");
                printf("1)auto\n");
                printf("2)manual\n");
                scanf("%d", &choice1);
                if (choice1 == 1)
                {
                    //BLACK BOX FOR AUTO PUTTING SHIPS
                } else if (choice1 == 2)
                {
                    printmap(pshots1);
                    printf("\n(Pay attention! first insert the beginning of ship then end of it!)\n\n");
                    getship(pshots1, &ships1, 1, 5);
                    printmap(pshots1);
                    getship(pshots1, &ships1, 2, 3);
                    printmap(pshots1);
                    getship(pshots1, &ships1, 3, 2);
                    printmap(pshots1);
                    getship(pshots1, &ships1, 4, 1);
                    printmap(pshots1);
                    syncingMaps(pshots1, attackenemy1);

                } else
                {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            mapbot(pshots3, &ships3);
            syncingMaps(pshots3, attackenemy3);
            printf("\n\nbot ships ar:\n\n");
            printships(ships3);
            gameSTARTbot();
        } else if (choice == 3)
        {
            //BLACK BOX FOR LOAD GAME
        } else if (choice == 4)
        {
            //BLACK BOX FOR LOAD LAST GAME
        } else if (choice == 5)
        {
            //SETTINGS
        } else if (choice == 6)
        {
            //SCORE BOARD
        } else
        {
            printf("wrong input!try again!\n---------------------\n");
            continue;

        }

    }

}

int main()
{
    //initialization of map1 & map2 & attackmap1 & attackmap2
    for (int i = 1; i < 12; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            map1[i][j] = 0;
            map2[i][j] = 0;
            attackmap1[i][j] = 10;
            attackmap2[i][j] = 10;
            map3[i][j] = 0;
            attackmap3[i][j] = 10;
        }
    }
    for (int i = 0; i < 12; i++)
    {
        //map1
        map1[0][i] = 4;
        map1[i][0] = 4;
        map1[11][i] = 4;
        map1[i][11] = 4;
        //map2
        map2[0][i] = 4;
        map2[i][0] = 4;
        map2[11][i] = 4;
        map2[i][11] = 4;
        //map3
        map3[0][i] = 4;
        map3[i][0] = 4;
        map3[11][i] = 4;
        map3[i][11] = 4;
        //attackmap1
        attackmap1[0][i] = 4;
        attackmap1[i][0] = 4;
        attackmap1[11][i] = 4;
        attackmap1[i][11] = 4;
        //attackmap2
        attackmap2[0][i] = 4;
        attackmap2[i][0] = 4;
        attackmap2[11][i] = 4;
        attackmap2[i][11] = 4;
        //attackmap3
        attackmap3[0][i] = 4;
        attackmap3[i][0] = 4;
        attackmap3[11][i] = 4;
        attackmap3[i][11] = 4;
    }
    //initializing pointer to map1
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            pshots1[i][j] = &map1[i][j];
        }
    }
    //initializing pointer to map2
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            pshots2[i][j] = &map2[i][j];
        }
    }
    //initializing pointer to map3
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            pshots3[i][j] = &map3[i][j];
        }
    }
    //initializing pointer to attackmap1
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            attackenemy1[i][j] = &attackmap1[i][j];
        }
    }
    //initializing pointer to attackmap2
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            attackenemy2[i][j] = &attackmap2[i][j];
        }
    }
    //initializing pointer to attackmap3
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            attackenemy3[i][j] = &attackmap3[i][j];
        }
    }
    entermenu();
    
}



