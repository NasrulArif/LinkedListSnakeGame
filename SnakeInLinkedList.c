#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <conio.h>
#include <windows.h>

#define xaxis 60 //Map size x-axis
#define yaxis 14 //Map size y-axis

enum Direction { UP, DOWN, LEFT, RIGHT }; //user-define constant (replace numerical values with string values)
enum Direction dir;

void Grow (int ,int);
void Disease ();
void CurrScrn ();
void MapForm ();
void Input();
int EnDeQueue (char button);
void sortRecord(int);
void readRecord();
void printRecord();

struct ScoreList
{
    char name[20];
    int score;
};

struct ScoreList PlayerScore;
struct ScoreList DBPlayerScore[200];

struct SnakeBody
{
    char body;
    struct SnakeBody *nextBody, *prevBody;
    int posX, posY; //Each body got their position
};

struct SnakeBody *snakeHead, *currSeg, *snakeSeg, *snakeTail;
int snakeSize = 0, locX,locY;
char mapMove [xaxis][yaxis],lastMove = '0'; //Initialize

void Grow (int coX, int coY) //for growing snake purpose, implementation of 'ADD TO END LIST'
{
    snakeSeg = (struct SnakeBody*)malloc(sizeof(struct SnakeBody));

    snakeSeg->body = 'O';

    if(snakeHead == NULL)
    {
        snakeHead = snakeSeg;
        snakeSeg->nextBody = NULL;
        snakeSeg->prevBody = NULL;
        snakeSeg->posX = coX;
        snakeSeg->posY = coY;
        snakeTail = snakeSeg;
    }
    else
    {
        snakeSeg->nextBody = NULL;
        snakeTail->nextBody = snakeSeg;
        snakeSeg->prevBody = snakeTail;

        if(lastMove == 'a')
        {
            snakeSeg->posX = coX+1;
            snakeSeg->posY = coY;
        }
        else if (lastMove == 'd')
        {
            snakeSeg->posX = coX-1;
            snakeSeg->posY = coY;
        }
        else if (lastMove == 's')
        {
            snakeSeg->posX = coX;
            snakeSeg->posY = coY-1;
        }
        else if (lastMove == 'w')
        {
            snakeSeg->posX = coX;
            snakeSeg->posY = coY+1;
        }

        snakeTail = snakeSeg;
    }

    snakeSize++;

}

void Disease () //UNDER CONSTRUCTION
{
    snakeTail = snakeTail->prevBody;
    snakeTail->nextBody = NULL;

    snakeSize--;

}

void MapForm () //MAP GENERATOR
{
    int i,j;

    for (i=0 ; i<yaxis ; i++)
    {
        for (j=0 ; j<xaxis ; j++)
        {
            if(i==0 || i==yaxis-1)
                mapMove[j][i] = '#';
            else
            {
                if(j==0 || j==xaxis-1)
                    mapMove[j][i] = '#';
                else
                    mapMove[j][i] = ' ';
            }
        }
    }
}

void CurrScrn () //MAP REFRESH EVERY FRAME
{
    system("CLS");

    int i,j;

    for (i=0 ; i<yaxis ; i++) //MAP BORDER CONSTRAIN
    {
        for (j=0 ; j<xaxis ; j++)
        {
            if(i==0 || i==yaxis-1)
                mapMove[j][i] = '#';
            else
            {
                if(j==0 || j==xaxis-1)
                    mapMove[j][i] = '#';
            }
        }
    }

    for(i=0 ; i<yaxis ; i++)
    {
        for(j=0 ; j<xaxis ; j++)
        {
            printf("%c",mapMove[j][i]);
            if(j==xaxis-1)
                        printf("\n");
        }
    }
    printf("\n");
    printf("SCORE: %d",PlayerScore.score);
}

int EnDeQueue (char button) //BEND , THE APPLICATION OF QUEUE AND DEQUEUE AT SAME TIME
{
    snakeSeg = (struct SnakeBody*)malloc(sizeof(struct SnakeBody));

    snakeSeg->body = 'O';

    switch (dir) {			//make decision from input()
	case LEFT:
        if(snakeSize == 1)
        {
            snakeSeg->nextBody = NULL;
            snakeSeg->prevBody = NULL;
            snakeSeg->posX = snakeHead->posX-1;
            snakeSeg->posY = snakeHead->posY;
            mapMove[snakeHead->posX][snakeHead->posY] = ' ';
            snakeHead = snakeSeg;

            mapMove[snakeSeg->posX][snakeHead->posY] = 'O';
            snakeTail = snakeSeg;
        }
        else
        {
            snakeSeg->nextBody = snakeHead;
            snakeHead->prevBody = snakeSeg;
            snakeSeg->prevBody = NULL;
            snakeSeg->posX = snakeHead->posX-1;
            snakeSeg->posY = snakeHead->posY;
            snakeHead = snakeSeg;

            mapMove[snakeSeg->posX][snakeHead->posY] = 'O';
            mapMove[snakeTail->posX][snakeTail->posY] = ' ';

            currSeg = snakeTail->prevBody;
            currSeg->nextBody = NULL;
            snakeTail = currSeg;
        }

        if(mapMove[snakeHead->posX-1][snakeHead->posY] == '#')
        {
            CurrScrn();
            printf("\n\n\t  ======GAME OVER=======\n\n\tYou crashed into the wall\n\n");
            return 0;
        }
        else if (mapMove[snakeHead->posX-1][snakeHead->posY] == 'O')
        {
            CurrScrn();
            printf("\n\n\t  ======GAME OVER=======\n\n\tYou crashed onto yourself\n\n");
            return 0;
        }

		break;

	case RIGHT:

		if(snakeSize == 1)
        {
            snakeSeg->nextBody = NULL;
            snakeSeg->prevBody = NULL;
            snakeSeg->posX = snakeHead->posX+1;
            snakeSeg->posY = snakeHead->posY;
            mapMove[snakeHead->posX][snakeHead->posY] = ' ';
            snakeHead = snakeSeg;

            mapMove[snakeSeg->posX][snakeHead->posY] = 'O';
            snakeTail = snakeSeg;
        }
        else
        {
            snakeSeg->nextBody = snakeHead;
            snakeHead->prevBody = snakeSeg;
            snakeSeg->prevBody = NULL;
            snakeSeg->posX = snakeHead->posX+1;
            snakeSeg->posY = snakeHead->posY;
            snakeHead = snakeSeg;

            mapMove[snakeSeg->posX][snakeHead->posY] = 'O';
            mapMove[snakeTail->posX][snakeTail->posY] = ' ';

            currSeg = snakeTail->prevBody;
            currSeg->nextBody = NULL;
            snakeTail = currSeg;
        }
        if(mapMove[snakeHead->posX+1][snakeHead->posY] == '#')
        {
            CurrScrn();
            printf("\n\n\t  ======GAME OVER=======\n\n\tYou crashed into the wall\n\n");
            return 0;
        }
        else if (mapMove[snakeHead->posX+1][snakeHead->posY] == 'O')
        {
            CurrScrn();
            printf("\n\n\t  ======GAME OVER=======\n\n\tYou crashed onto yourself\n\n");
            return 0;
        }

		break;

	case UP:
		if(snakeSize == 1)
        {
            snakeSeg->nextBody = NULL;
            snakeSeg->prevBody = NULL;
            snakeSeg->posX = snakeHead->posX;
            snakeSeg->posY = snakeHead->posY-1;
            mapMove[snakeHead->posX][snakeHead->posY] = ' ';
            snakeHead = snakeSeg;

            mapMove[snakeSeg->posX][snakeHead->posY] = 'O';
            snakeTail = snakeSeg;
        }
        else
        {
            snakeSeg->nextBody = snakeHead;
            snakeHead->prevBody = snakeSeg;
            snakeSeg->prevBody = NULL;
            snakeSeg->posX = snakeHead->posX;
            snakeSeg->posY = snakeHead->posY-1;
            snakeHead = snakeSeg;

            mapMove[snakeSeg->posX][snakeHead->posY] = 'O';
            mapMove[snakeTail->posX][snakeTail->posY] = ' ';

            currSeg = snakeTail->prevBody;
            currSeg->nextBody = NULL;
            snakeTail = currSeg;
        }

        if(mapMove[snakeHead->posX][snakeHead->posY-1] == '#')
        {
            CurrScrn();
            printf("\n\n\t  ======GAME OVER=======\n\n\tYou crashed into the wall\n\n");
            return 0;
        }
        else if (mapMove[snakeHead->posX][snakeHead->posY-1] == 'O')
        {
            CurrScrn();
            printf("\n\n\t  ======GAME OVER=======\n\n\tYou crashed onto yourself\n\n");
            return 0;
        }

		break;

	case DOWN:
		if(snakeSize == 1)
        {

            snakeSeg->nextBody = NULL;
            snakeSeg->prevBody = NULL;
            snakeSeg->posX = snakeHead->posX;
            snakeSeg->posY = snakeHead->posY+1;
            mapMove[snakeHead->posX][snakeHead->posY] = ' ';
            snakeHead = snakeSeg;

            mapMove[snakeSeg->posX][snakeHead->posY] = 'O';
            snakeTail = snakeSeg;
        }
        else
        {
            snakeSeg->nextBody = snakeHead;
            snakeHead->prevBody = snakeSeg;
            snakeSeg->prevBody = NULL;
            snakeSeg->posX = snakeHead->posX;
            snakeSeg->posY = snakeHead->posY+1;
            snakeHead = snakeSeg;

            mapMove[snakeSeg->posX][snakeHead->posY] = 'O';
            mapMove[snakeTail->posX][snakeTail->posY] = ' ';

            currSeg = snakeTail->prevBody;
            currSeg->nextBody = NULL;
            snakeTail = currSeg;
        }

        if(mapMove[snakeHead->posX][snakeHead->posY+1] == '#')
        {
            CurrScrn();
            printf("\n\n\t  ======GAME OVER=======\n\n\tYou crashed into the wall\n\n");
            return 0;
        }
        else if (mapMove[snakeHead->posX][snakeHead->posY+1] == 'O')
        {
            CurrScrn();
            printf("\n\n\t  ======GAME OVER=======\n\n\tYou crashed onto yourself\n\n");
            return 0;
        }

		break;
	}

}

void Input () //USER INPUT
{
    char choice;

    if (kbhit()) {
		switch (getch()) {

		case 'a':   if(lastMove == 'd')
                    {
                        printf("\n\nYOU CANNOT DO THAT NOW\n\n");
                        break;
                    }

                    choice = 'a';

                    //EnDeQueue(choice);

                    lastMove = 'a';

                    dir = LEFT;
                    break;

		case 'd':   if(lastMove == 'a')
                    {
                        printf("\n\nYOU CANNOT DO THAT NOW\n\n");
                        break;
                    }

                    choice = 'd';



                    //EnDeQueue(choice);

                    lastMove = 'd';

                    dir = RIGHT;
                    break;

		case 'w':   if(lastMove == 's')
                    {
                        printf("\n\nYOU CANNOT DO THAT NOW\n\n");
                        break;
                    }

                    choice = 'w';



                    //EnDeQueue(choice);
                    lastMove = 'w';
                    dir = UP;
                    break;

		case 's':   if(lastMove == 'w')
                    {
                        printf("\n\nYOU CANNOT DO THAT NOW\n\n");
                        break;
                    }

                    choice = 's';
                    lastMove = 's';
                    dir = DOWN;
                    break;

		case 'x':
			return;
		}
	}
}

void FoodGenerate() //FOOD GENERATOR
{
    srand(time(NULL));

    locX = (rand() % 56) + 2;
    locY = (rand() % 10) + 2;

    if(mapMove[locX][locY] == 'O')
        FoodGenerate();
    else
        mapMove[locX][locY] = 'F';
}

void readRecord() {
    FILE *info = fopen("record.txt","r");

    int i=0;
    char buffer[30];

    while(!feof(info))
    {
        fscanf(info,"%s",buffer);
        fscanf(info,"%s",&DBPlayerScore[i].name);
        fscanf(info,"%i",&DBPlayerScore[i].score);
        i++;
    }

    sortRecord(i);
}

void sortRecord(int qty)
{
    int j,k;
    char tempname[20];
    int tempscore;

    for(j=0; j<qty-1; j++)
    {
        for(k=0; k<qty-j; k++)
        {
            if(DBPlayerScore[k].score < DBPlayerScore[k+1].score)
            {
                strcpy(tempname,DBPlayerScore[k].name);
                tempscore = DBPlayerScore[k].score;

                strcpy(DBPlayerScore[k].name,DBPlayerScore[k+1].name);
                DBPlayerScore[k].score = DBPlayerScore[k+1].score;

                strcpy(DBPlayerScore[k+1].name,tempname);
                DBPlayerScore[k+1].score = tempscore;
            }
        }
    }

    printf("\n\n\n\n\t========CURRENT TOP 10 SCORE RANKING========\n");

    for(j=0; j<10; j++)
    {
        printf("\n\t\t%i\t:\t",DBPlayerScore[j].score);
        printf("%s",DBPlayerScore[j].name);
    }

    printf("\n\n\n");
}

void saveRecord() {				//save record on a file

    system("cls");

    FILE *info;
    info = fopen("record.txt","a+");

    fprintf(info,"\n=================\n");
    fprintf(info,"%s\n", PlayerScore.name);
    fprintf(info,"%i", PlayerScore.score);

    fclose(info);

}

void printRecord() {			//printing on screen only
    char cha;
    int counter = 0, z;

    system("cls");

    printf("Enter your name: ");
    scanf("%[^\n]s", &PlayerScore.name);

    for (z = 0; PlayerScore.name[z]; z++)
        if (PlayerScore.name[z] != ' ')
            PlayerScore.name[counter++] = PlayerScore.name[z];

    PlayerScore.name[counter++] = '\0';

    system("CLS");

    printf("Player name :%s\n", PlayerScore.name);
    printf("Your score :%i\n\n", PlayerScore.score);

    printf("Do you want to save your record?(y/n)");
    cha=getch();
    if (cha == 'y'){
        saveRecord();
        printf("Your score have been save, thank you");

        readRecord();
    }
    else if (cha == 'n') {
        printf("\nThank you. Play again :)");

        readRecord();
    }


}

void Engine ()
{
    int Continuity = 1;

    FoodGenerate(); //GENERATE FOOD IN MAP
    CurrScrn(); //REFRESH FRAME


        while(mapMove[locX][locY] != 'O') //IF HEAD DOES NOT OVERWRITE FOOD (eat), LOOP WILL NOT TERMINATE
        {
            CurrScrn();
            Continuity = EnDeQueue(lastMove);
            Input();
            Sleep(50);

            if(Continuity == 0) //LOSING GAME WHEN CONTINUITY = 0aaaaaaaaa
            {
                printRecord();
                return;
            }
        }
        Grow(snakeTail->posX,snakeTail->posY); //GROWING UP SNAKE EVERYTIME eat IS DONE
        PlayerScore.score = PlayerScore.score + 10;

        Engine();
}

int main () //MAIN ENGINE
{
    PlayerScore.score = 0;

    MapForm();
    CurrScrn();

    Grow(19,5);
    mapMove[snakeHead->posX][snakeHead->posY] = snakeHead->body;
    lastMove = 'd';
    dir = RIGHT;

    Engine();

}



