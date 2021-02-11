#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define USERSPATH "user.txt"

int map1[12][12];                                     //player1 game map
int map2[12][12];                                     //player2 game map
int map3[12][12];
int attackmap1[12][12];                                //map of player 2 for player 1
int attackmap2[12][12];                                //map of player 1 for player 2
int attackmap3[12][12];
int* pshots1[12][12];
int* pshots2[12][12];
int* pshots3[12][12];
int* attackenemy1[12][12];
int* attackenemy2[12][12];
int* attackenemy3[12][12];
int ID = 101;
struct node* ships1 = NULL;
struct node* ships2 = NULL;
struct node* ships3 = NULL;

char player1[100];
char player2[100];

void addfront(struct node** list, int id, int x1, int y1, int x2, int y2, int len);
void addfront1(struct node** list, int id, int x1, int y1, int len);

struct node
{
    int id;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int len;
    struct node* next;
};
struct Players
{
    int score;
    char username[100];
}users[2];

int updateuser(char username[100], int score)                               //updating users scores
{
    FILE* fp = fopen(USERSPATH, "w+");
    if (fp == NULL)
    {
        printf("\nerror!\n");
        return -1;
    }
    while (feof(fp) != 0)
    {
        char usrnm[100];
        char tmp[20];
        fscanf(fp, "%s", usrnm);
        fscanf(fp, "%s", tmp);
        score+=atoi(tmp);
        if (strcmp(username, usrnm) == 0)
        {
            char scr [5];
            fseek(fp, (-1)*strlen(tmp), SEEK_CUR);
            sprintf(scr, "%d", score);
            fputs(scr,fp);
            fputc('\n', fp);
            return 1;
        }
    }
    return -1;
}
int newuser(char username[100])                                         //getting new user & adding to file
{
    FILE* fp = fopen(USERSPATH, "a+");
    if (fp == NULL)
    {
        printf("\nerror!\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    while (feof(fp) != 1)
    {
        
        char usrnm[100];
        char tmp[20];
        fscanf(fp, "%s", usrnm);
        fscanf(fp, "%s", tmp);
        if (strcmp(username, usrnm) == 0)
        {
            printf("\nError!user exist!\n\n");
            return -2;
        }
    }
    fputs(username, fp);
    fputc(' ', fp);
    fputc('0', fp);
    fputc('\n', fp);
    fclose(fp);
    return 1;
}

int showusers()                                                         //showing the list of users
{
    FILE* fp = fopen(USERSPATH, "r");
    if (fp == NULL)
    {
        printf("\nerror!\n");
        return -1;
    }
    while(feof(fp)!=1)
    {
        char usrnm[100];
        int score;
        fscanf(fp, "%s %d",usrnm, &score);
        printf("%s: %d\n",usrnm,score);
    }
    fclose(fp);
    return 1;
}

int loadlinkedlist(char path[], int mode)                               //for loading ships
{
    FILE* fp = fopen(path, "r");
    if (mode == 1)
    {
        for (int i = 0; i < 10; i++)
        {
            int id;
            int startx, starty, endx, endy, len;
            fscanf(fp, "%d", &id);
            fscanf(fp, "%d", &startx);
            fscanf(fp, "%d", &starty);
            fscanf(fp, "%d", &endx);
            fscanf(fp, "%d", &endy);
            fscanf(fp, "%d", &len);
            if (len == 1)
            {
                addfront1(&ships1, id, startx, starty, len);
            }
            else
            {
                addfront(&ships1, id, startx, starty, endx, endy, len);
            }
        }
        for (int i = 0; i < 10; i++)
        {
            int id;
            int startx, starty, endx, endy, len;
            fscanf(fp, "%d", &id);
            fscanf(fp, "%d", &startx);
            fscanf(fp, "%d", &starty);
            fscanf(fp, "%d", &endx);
            fscanf(fp, "%d", &endy);
            fscanf(fp, "%d", &len);
            if (len == 1)
            {
                addfront1(&ships3, id, startx, starty, len);
            }
            else
            {
                addfront(&ships3, id, startx, starty, endx, endy, len);
            }
        }
        fclose(fp);
        return 0;
    }
    else if (mode == 2)
    {
        for (int i = 0; i < 10; i++)
        {
            int id;
            int startx, starty, endx, endy, len;
            fscanf(fp, "%d", &id);
            fscanf(fp, "%d", &startx);
            fscanf(fp, "%d", &starty);
            fscanf(fp, "%d", &endx);
            fscanf(fp, "%d", &endy);
            fscanf(fp, "%d", &len);
            if (len == 1)
            {
                addfront1(&ships1, id, startx, starty, len);
            }
            else
            {
                addfront(&ships1, id, startx, starty, endx, endy, len);
            }
        }
        for (int i = 0; i < 10; i++)
        {
            int id;
            int startx, starty, endx, endy, len;
            fscanf(fp, "%d", &id);
            fscanf(fp, "%d", &startx);
            fscanf(fp, "%d", &starty);
            fscanf(fp, "%d", &endx);
            fscanf(fp, "%d", &endy);
            fscanf(fp, "%d", &len);
            if (len == 1)
            {
                addfront1(&ships2, id, startx, starty, len);
            }
            else
            {
                addfront(&ships2, id, startx, starty, endx, endy, len);
            }
        }
        fclose(fp);
        return 0;
    }
    return -1;
}

int savelinklist(char usr1[], char usr2[], int mode)                    //saving linklist of ships
{
    char filename[256];
    bzero(filename, 256);
    strcat(filename, usr1);
    strcat(filename, "_");
    strcat(filename, usr2);
    strcat(filename, "linklist");
    strcat(filename, ".txt");
    FILE* fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("\nerror!\n");
        return -1;
    }
    if (mode == 1)
    {
        struct node* cur = ships1;
        while (cur != NULL)
        {
            char temp0[5];
            char temp1[5];
            char temp2[5];
            char temp3[5];
            char temp4[5];
            char temp5[5];
            
            sprintf(temp0, "%d", cur->id);
            sprintf(temp1, "%d", cur->start_x);
            sprintf(temp2, "%d", cur->start_y);
            sprintf(temp3, "%d", cur->end_x);
            sprintf(temp4, "%d", cur->end_y);
            sprintf(temp5, "%d", cur->len);
            
            fputs(temp0, fp);
            fputc(' ', fp);
            fputs(temp1, fp);
            fputc(' ', fp);
            fputs(temp2, fp);
            fputc(' ', fp);
            fputs(temp3, fp);
            fputc(' ', fp);
            fputs(temp4, fp);
            fputc(' ', fp);
            fputs(temp0, fp);
            fputc(' ', fp);
            fputc('\n', fp);
            
            cur = cur->next;
        }
        struct node* cur2 = ships3;
        while (cur2 != NULL)
        {
            char temp0[5];
            char temp1[5];
            char temp2[5];
            char temp3[5];
            char temp4[5];
            char temp5[5];
            
            sprintf(temp0, "%d", cur2->id);
            sprintf(temp1, "%d", cur2->start_x);
            sprintf(temp2, "%d", cur2->start_y);
            sprintf(temp3, "%d", cur2->end_x);
            sprintf(temp4, "%d", cur2->end_y);
            sprintf(temp5, "%d", cur2->len);
        
            fputs(temp0, fp);
            fputc(' ', fp);
            fputs(temp1, fp);
            fputc(' ', fp);
            fputs(temp2, fp);
            fputc(' ', fp);
            fputs(temp3, fp);
            fputc(' ', fp);
            fputs(temp4, fp);
            fputc(' ', fp);
            fputs(temp0, fp);
            fputc(' ', fp);
            fputc('\n', fp);
            
            cur2 = cur2->next;
        }
    }
    else if (mode == 2)
    {
        struct node* cur = ships1;
        while (cur != NULL)
        {
            char temp0[5];
            char temp1[5];
            char temp2[5];
            char temp3[5];
            char temp4[5];
            char temp5[5];
            
            sprintf(temp0, "%d", cur->id);
            sprintf(temp1, "%d", cur->start_x);
            sprintf(temp2, "%d", cur->start_y);
            sprintf(temp3, "%d", cur->end_x);
            sprintf(temp4, "%d", cur->end_y);
            sprintf(temp5, "%d", cur->len);
            
            fputs(temp0, fp);
            fputc(' ', fp);
            fputs(temp1, fp);
            fputc(' ', fp);
            fputs(temp2, fp);
            fputc(' ', fp);
            fputs(temp3, fp);
            fputc(' ', fp);
            fputs(temp4, fp);
            fputc(' ', fp);
            fputs(temp0, fp);
            fputc(' ', fp);
            fputc('\n', fp);
            
            cur = cur->next;
        }
        struct node* cur2 = ships2;
        while (cur2 != NULL)
        {
            char temp0[5];
            char temp1[5];
            char temp2[5];
            char temp3[5];
            char temp4[5];
            char temp5[5];
            
            sprintf(temp0, "%d", cur2->id);
            sprintf(temp1, "%d", cur2->start_x);
            sprintf(temp2, "%d", cur2->start_y);
            sprintf(temp3, "%d", cur2->end_x);
            sprintf(temp4, "%d", cur2->end_y);
            sprintf(temp5, "%d", cur2->len);
            
            fputc(' ', fp);
            fputs(temp1, fp);
            fputc(' ', fp);
            fputs(temp2, fp);
            fputc(' ', fp);
            fputs(temp3, fp);
            fputc(' ', fp);
            fputs(temp4, fp);
            fputc(' ', fp);
            fputs(temp0, fp);
            fputc(' ', fp);
            fputc('\n', fp);
            
            cur2 = cur2->next;
        }
    }
    fclose(fp);
    return 0;
}

int LoadGame(char path[], int size)                                         //for loading maps
{
    printf("%s", path);
    FILE* fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("\nerror!\n");
        return -1;
    }
    char mode;
    fscanf(fp, "%c", &mode);
    if (mode == '1')
    {
        //reading player 1's map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                fscanf(fp, "%d", &*pshots1[i][j]);
            }
        }
        //reading player 1's attackenemy
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                fscanf(fp, "%d", &*pshots1[i][j]);

            }
        }
        //reading bot's map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                fscanf(fp, "%d", &*pshots1[i][j]);
            }
        }
        //reading bot's attackenemy
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                fscanf(fp, "%d", &*pshots1[i][j]);
            }
        }
        fclose(fp);
        return 1;
    }
    else if (mode == '2')
    {
        printf("\nerror!\n");
        //reading player 1's map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                fscanf(fp, "%d", pshots1[i][j]);
            }
        }
        //reading player 1's attackenemy
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                fscanf(fp, "%d", attackenemy1[i][j]);
            }
        }
        //reading player 2's map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                fscanf(fp, "%d", pshots2[i][j]);
            }
        }
        //reading player 2's attackenemy
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                fscanf(fp, "%d", attackenemy2[i][j]);
            }
        }
        fclose(fp);
        return 2;
    }
    else
        return -1;
}

int LoadUser(char username[])                                           //for loading users
{

    FILE* fp = fopen(USERSPATH, "r");
    if (fp == NULL)
    {
        printf("\nerror!\n");
        return -1;
    }
    char usrnm[100];
    while (feof(fp)!=1)
    {
        fscanf(fp, "%s", usrnm);
        if (strcmp(usrnm, username) == 0)
        {
            int scr;
            char temp[20];
            fscanf(fp, "%s", temp);
            scr = atoi(temp);
            strcpy(users[1].username, usrnm);
            users[1].score = scr;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return -1;
}

int SaveGame(char usr1[], char usr2[])                                       //saving the game in the file
{
    char filename[256];
    bzero(filename, 256);
    strcat(filename, usr1);
    strcat(filename, "_");
    strcat(filename, usr2);
    strcat(filename, ".txt");
    FILE* fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("\nerror!\n");
        return -1;
    }
    int mode;
    if (strcmp(usr2, "bot") == 0)
        mode = 1;
    else
        mode = 2;

    if (mode == 1)
    {
        fprintf(fp, "%d\n", 1);
        //writing player 1's map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                char temp[4];
                sprintf(temp, "%d", *pshots1[i][j]);
                fputs(temp, fp);
                fputc(' ', fp);
            }
            fputs("\n", fp);
        }
        fputs("\n", fp);
        //writing player 1's attackenemy
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                char temp[4];
                sprintf(temp, "%d", *attackenemy1[i][j]);
                fputs(temp, fp);
                fputc(' ', fp);
            }
            fputs("\n", fp);
        }
        fputs("\n", fp);
        //writing bot's map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                char temp[4];
                sprintf(temp, "%d", *pshots3[i][j]);
                fputs(temp, fp);
                fputc(' ', fp);
            }
            fputs("\n", fp);
        }
        fputs("\n", fp);
        //writing bot's attackenemy
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                char temp[4];
                sprintf(temp, "%d", *attackenemy3[i][j]);
                fputs(temp, fp);
                fputc(' ', fp);
            }
            fputs("\n", fp);
        }
        fputs("\n", fp);
        fclose(fp);
        return 1;
    }
    else if (mode == 2)
    {
        fprintf(fp, "%d\n", 2);
        //writing player 1's map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                char temp[4];
                sprintf(temp, "%d", *pshots1[i][j]);
                fputs(temp, fp);
                fputc(' ', fp);
            }
            fputs("\n", fp);
        }
        fputs("\n", fp);
        //writing player 1's attackenemy
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                char temp[4];
                sprintf(temp, "%d", *attackenemy1[i][j]);
                fputs(temp, fp);
                fputc(' ', fp);
            }
            fputs("\n", fp);
        }
        fputs("\n", fp);
        //writing player 2's map
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                char temp[4];
                sprintf(temp, "%d", *pshots2[i][j]);
                fputs(temp, fp);
                fputc(' ', fp);
            }
            fputs("\n", fp);
        }
        fputs("\n", fp);
        //writing player 2's attackenemy
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                char temp[4];
                sprintf(temp, "%d", *attackenemy2[i][j]);
                fputs(temp, fp);
                fputc(' ', fp);
            }
            fputs("\n", fp);
        }
        fputs("\n", fp);
        fclose(fp);
        return 1;
    }
    return -1;
}

void syncingMaps(int* map[12][12], int* attackmap[12][12])                //syncing main map and enemy map
{
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            *attackmap[i][j] = *map[i][j] + 20;
        }
    }
}

void showWater(int* arr[12][12], int x1, int y1, int x2, int y2);

struct node* createnode(int id, int x1, int y1, int x2, int y2, int len)        //ships bigger than 1
{
    struct node* newnode;
    newnode = (struct node*) malloc(sizeof(struct node));
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

struct node* createnode1(int id, int x1, int y1, int len)                    //ships with size of 1
{
    struct node* newnode;
    newnode = (struct node*) malloc(sizeof(struct node));
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

void addfront(struct node** list, int id, int x1, int y1, int x2, int y2, int len) //addfront for more than 1
{
    struct node* newnode = createnode(id, x1, y1, x2, y2, len);
    newnode->next = *list;
    *list = newnode;
}

void addfront1(struct node** list, int id, int x1, int y1, int len)                   //add front size 1
{
    struct node* newnode = createnode1(id, x1, y1, len);
    newnode->next = *list;
    *list = newnode;
}

void searchships(int id, struct node** list, int turn)              //searching and destroying ships
{
    struct node* cur = *list;
    struct node* prev = NULL;
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
        }
        else if (turn == 2)
        {
            showWater(attackenemy1, cur->start_x, cur->start_y, cur->end_x, cur->end_y);
        }
        else if (turn == 3)
        {
            showWater(attackenemy3, cur->start_x, cur->start_y, cur->end_x, cur->end_y);
        }
        if (prev == NULL)
        {
            (*list) = (*list)->next;
            free(cur);
        }
        else
        {
            prev->next = cur->next;
            free(cur);
        }
    }

}
void mapbot(int* arr[12][12], struct node** list)
{
    for (int i = 1; i <= 5; i++)                           //ship size 5
    {
        *arr[1][i] = 101;
    }
    for (int i = 1; i <= 3; i++)                           //ship size 3
    {
        *arr[3][i] = 102;
    }
    for (int i = 5; i <= 7; i++)                            //ship size 3
    {
        *arr[5][i] = 103;
    }
    for (int i = 1; i <= 2; i++)                            //ship size 2
    {
        *arr[i][10] = 104;
    }
    for (int i = 8; i <= 9; i++)                            //ship size 2
    {
        *arr[i][1] = 105;
    }
    for (int i = 8; i <= 9; i++)                            //ship size 2
    {
        *arr[i][6] = 106;
    }
    *arr[2][7] = 107;                                       //ships size 1:
    *arr[6][1] = 108;
    *arr[7][10] = 109;
    *arr[10][10] = 110;
    addfront(list, 101, 0, 0, 0, 4, 5);
    addfront(list, 102, 2, 0, 2, 2, 3);
    addfront(list, 103, 4, 4, 4, 6, 3);
    addfront(list, 104, 0, 9, 1, 9, 2);
    addfront(list, 105, 7, 0, 8, 0, 2);
    addfront(list, 106, 7, 5, 8, 5, 2);
    addfront1(list, 107, 1, 6, 1);
    addfront1(list, 108, 5, 0, 1);
    addfront1(list, 109, 6, 9, 1);
    addfront1(list, 110, 9, 9, 1);
}
void getship(int* map[12][12], struct node** list, int n, int size)              //getting all ships
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
                }
                else
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
            }
            else
            {

                scanf("%d %d %d %d", &x1, &y1, &x2, &y2);                    //conditions are:
                if (x1 == x2 && y1 > y2)
                {
                    printf("wrong input!try again!\n");                        //end and beginning
                    i--;
                    continue;
                }
                else if (y1 == y2 && x1 > x2)
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
                }
                else if (x1 != x2 && y1 != y2)                                  //not orib
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }
                else if (x1 > 9 || y1 > 9 || x2 > 9 || y2 > 9)                       //less than max
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }
                else if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0)                       //more than min
                {
                    printf("wrong input!try again!\n");
                    i--;
                    continue;
                }
                else if (x1 == x2)                                           //checking size ofoghi
                {
                    if ((abs(y2 - y1) + 1) > size) {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    }
                    else if ((abs(y2 - y1) + 1) < size) {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    }
                }
                else if (y1 == y2)                                           //checking size amoodi
                {
                    if ((abs(x2 - x1) + 1) > size) {
                        printf("wrong input!try again!\n");
                        i--;
                        continue;
                    }
                    else if ((abs(x2 - x1) + 1) < size) {
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
                }
                else {
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

void printmap(int* arr[12][12])                                         //printing maps
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
        }
        else
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
int shooting(int* arr[12][12], int x, int y, struct node** list, int turn)
{
    x = 0;
    y = 0;
    printf("\nyour enemy map is:\n");
    printmap(arr);
    while (*list != NULL)
    {
        printf("\nchoose your shot\n");
        scanf("%d", &x);
        if (x == -1)
        {
            SaveGame(player1, player2);
            if (strcmp(player2, "bot") == 0)
                savelinklist(player1, player2, 1);
            else
                savelinklist(player1, player2, 2);
            return 0;
        }
        scanf("%d", &y);
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
            printf("\n\n");
            return -1;
        }
        else if (*arr[x + 1][y + 1] == 3 || *arr[x + 1][y + 1] == 1)                     //already shot
        {
            printf("\nyou cant shoot here!try again!\n");
        }
        else
        {
            int temp = *arr[x + 1][y + 1];                                       //correct shot
            *arr[x + 1][y + 1] = 1;
            searchships(temp - 20, list, turn);
            printf("\nwow!nice shot :O !\nas a gift shot again!:)\n");
            printf("\nnow the map is :\n");
            printmap(arr);
        }
    }
    return 1;
}
void botshooting(int* arr[12][12], struct node** list, int turn)               //my smart bot:))
{
    int flag =0;
    while (*list != NULL)
    {
        srand(time(NULL));
        if (flag==0)
        {
            x = rand() % 10;
            y = rand() % 10;
        }else
        {
            if (x==0)                                                   //bala nmitune
            {
                if (y==0)                                               //chapam nmitune
                {
                    int jahat = rand()%2;
                    jahat ? x++ : y++;
                }else if(y==9)                                          //rastam nmitune
                {
                    int jahat = rand()%2;
                    jahat ? x++ : y--;
                }else
                {
                    int jahat = rand()%3;
                    if (jahat==0)                                       //bere rast
                        y++;
                    else if(jahat==1)                                   //bere payin
                        x++;
                    else                                                //bere chao
                        y--;
                }
            }else if(x==9)                                             //payin nmitune
            {
                if (y==0)                                               //chapam nmitune
                {
                    int jahat = rand()%2;
                    jahat ? y++ : x--;
                }else if(y==9)                                          //rastam nmitune
                {
                    int jahat = rand()%2;
                    jahat ? x-- : y--;
                }else
                {
                    int jahat = rand()%3;
                    if (jahat==0)                                       //bere chap
                        y--;
                    else if(jahat==1)                                   //bere bala
                        x--;
                    else                                                //bere rast
                        y++;
                }
            }else
            {
                int jahat = rand()%4;
                switch (jahat)
                {
                    case 0:                                            //bere bala
                        x--;
                        break;
                    case 1:                                            //bere rast
                        y++;
                        break;
                    case 2:                                            //bere payin
                        x++;
                        break;
                    case 3:                                            //bere chap
                        y--;
                        break;
                }
            }
        }
        if (*arr[x + 1][y + 1] == 20 || *arr[x + 1][y + 1] == 25)               //shots on water or borders
        {
            flag=0;
            *arr[x + 1][y + 1] = 3;
            printf("\n\nyour map is :\n\n");
            printmap(arr);
            return;
        }
        else if (*arr[x + 1][y + 1] == 3 || *arr[x + 1][y + 1] == 1);
        else
        {
            int temp = *arr[x + 1][y + 1];                                       //correct shot
            *arr[x + 1][y + 1] = 1;
            searchships(temp - 20, list, turn);
            printf("\n\nyour map is :\n\n");
            printmap(arr);
            printf("\n\nthe bot shoud shoot you again:(\n!press any key to continue!\n\n");
            getchar();
            flag=1;
            
        }
    }
}

void showWater(int* arr[12][12], int x1, int y1, int x2, int y2)        //showing the borders water
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
        printf("\n\n%s turn:\n\n" , player1);
        if(shooting(attackenemy2, x, y, &ships2, 1)==0)
            return;
        if (ships2 == NULL)
        {
            printf("\n\nBOOOM!\n\n%s win!!!\n\n" ,player1);
            updateuser(player1, 71);
            updateuser(player2, rand()%20 + 15);
            return;
        }

        //player 2 turn
        printf("\n\nplayer 2 turn:\n\n");
        while (1)
        {
            if(shooting(attackenemy1, x, y, &ships1, 2)==0)
                return;
            if (ships1 == NULL)
            {
                printf("\n\nBOOOM!\n\n%s win!!!\n\n" ,player2);
                updateuser(player2, 71);
                updateuser(player1, rand()%25+10);
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
        printf("\n\n%s turn:\n\n" , player1);
        if(shooting(attackenemy3, x, y, &ships3, 3)==0)
            return;
        if (ships3 == NULL)
        {
            printf("\n\nBOOOM!\n\nYou win!!!\n\n");
            return;
        }
        
        // bot turn
        botshooting(attackenemy1, &ships1, 2);
        if (ships1 == NULL)
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
                    char usrnm[100];
                    showusers();
                    printf("\nplease enter the username:\n");
                    scanf("%s", usrnm);
                    if (newuser(usrnm)>=0)
                    {
                        strcpy(player1, usrnm);
                    }else
                        continue;
                }
                else if (choice1 == 2)
                {
                    char temp[100];
                    printf("\nEnter your username: \n");
                    scanf("%s", temp);
                    if (newuser(temp)>=0)
                    {
                        strcpy(player1, temp);
                    }
                    else
                        continue;

                }
                else {
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
                }
                else if (choice1 == 2)
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

                }
                else
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
                    char usrnm[100];
                    showusers();
                    printf("\nplease enter the username:\n");
                    scanf("%s", usrnm);
                    if (newuser(usrnm)>=0)
                    {
                        strcpy(player1, usrnm);
                    }
                    else
                        continue;
                }
                else if (choice2 == 2)
                {
                    char temp[100];
                    printf("\nEnter your username: \n");
                    scanf("%s", temp);
                    if (newuser(temp)>=0)
                    {
                        strcpy(player2, temp);
                    }else
                        continue;
                }
                else
                {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            while (1)
            {

                printf("put ships:\n");
                printf("1)manual\n");
                scanf("%d", &choice2);
                if (choice2 == 1)
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


                }
                else
                {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            gameSTART();
        }
        else if (choice == 2)
        {
            //STARTING THE GAME WITH BOT
            while (1)
            {
                int choice1;
                printf("player 1\n");
                printf("choose user:\n");
                printf("1)choose from available users\n");
                printf("2)new user\n");
                scanf("%d", &choice1);
                if (choice1 == 1)
                {
                    char usrnm[100];
                    showusers();
                    printf("\nplease enter the username:\n");
                    scanf("%s", usrnm);
                    if (newuser(usrnm)>=0)
                    {
                        strcpy(player1, usrnm);
                    }else
                        continue;
                }
                else if (choice1 == 2)
                {
                    char temp[100];
                    printf("\nEnter your username: \n");
                    scanf("%s", temp);
                    if (newuser(temp)>=0)
                    {
                        strcpy(player2, temp);
                    }else
                        continue;
                }
                else {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                printf("put ships:\n");
                printf("1)manual\n");
                scanf("%d", &choice1);
                if (choice1 == 1)
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

                }
                else
                {
                    printf("wrong input!try again!\n---------------------\n");
                    continue;
                }
                break;
            }
            mapbot(pshots3, &ships3);
            syncingMaps(pshots3, attackenemy3);
            gameSTARTbot();
        }
        else if (choice == 3)
        {
            char usr1[100];
            char usr2[100];
            printf("\n enter user1's username: ");
            scanf("%s", usr1);
            printf("\n enter user2's username: ");
            scanf("%s", usr2);
            //loading the game
            char filename[256];
            strcat(filename, usr1);
            strcat(filename, "_");
            strcat(filename, usr2);
            //loading the ships
            char linkfilename[256];
            strcpy(linkfilename, filename);
            strcat(linkfilename, "linklist");
            strcat(filename, ".txt");
            strcat(linkfilename, ".txt");
            LoadGame(filename, 256);
            if (strcmp(usr2, "bot") == 0) {
                loadlinkedlist(linkfilename, 1);
                gameSTARTbot();
            }
            else
            {
                loadlinkedlist(linkfilename, 2);
                gameSTART();
            }
        }
        else if (choice == 4)
        {
            //BLACK BOX FOR LOAD LAST GAME
        }
        else if (choice == 5)
        {
            //SETTINGS
        }
        else if (choice == 6)
        {
            printf("\n\nthe score board is:\n\n");
            showusers();
            printf("\n\n");
        }
        else
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




