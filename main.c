#include "main.h"

/* Object types:
   0 - free space
   1 - wall
   2 - player
   3 - bullet
   4 - mine
   5 - starting position //only used during map loading or making default map

   Action types:
   2,4,6 and 8 change direction, like number pad. 5 moves forward, 50 fires a bullet, 500 deploys a mine.
   42 - offer peace
   999 - call the police
*/

int objectNumber=0;  //Counts number of objects
object *object_list;   //Stores all objects.

ai_t *ai_list; //stores the ais that can be used in the game
int aiNumber = 0;

int main()
{
    /*Any ai you want to be able to access must be entered here!*/
    newAi_t("greg",&greg);
    newAi_t("human",&human); //is this really a good idea?
    newAi_t("tito",&tito);

#ifndef _WIN32
    initscr();
    raw();
#endif

    int i,j;
    char c,d;
    char *enteredName = malloc(256*sizeof(char));
    char nonep[5] = "none\0";
    int foundName;

    do
    {
        PRINT("Do you want to load a map from file (y/n) (default is basic square):");
        c = getchar();
        d = getchar(); //needed to eat the '\0' that gets entered
    }
    while(c != 'y' && c  != 'n' && c != 'Y' && c != 'N');

    if(c == 'y' ||c == 'Y')
    {
        loadMap();
    }
    else if (c == 'n' ||c == 'N')
    {
        defaultMap();
    }

    map = malloc((xsize+2)*sizeof(char *)); //this has to go here because the map gets printed before ai location selection happens
    for(i=0; i<xsize+2; ++i)
    {
        map[i]=malloc(ysize);
    }

    reprintMap();
    PRINT("Available ais are :\n");
    for(i=0; i<aiNumber; ++i)
    {
        PRINT("%s\n",ai_list[i].name);
    }

    for(i=0; i<objectNumber; ++i)
    {
        if(object_list[i].type == 5)
        {

            foundName = 0;
            while(foundName == 0)
            {
                PRINT("Please enter the ai to go in position (%d,%d)\n(\"none\" to leave space empty)):",object_list[i].x,object_list[i].y);
                scanf("%s",enteredName);
                getchar();
                if(strcmp(nonep,enteredName) == 0) //logic required to leave the space empty
                {
                    foundName= -1;
                }
                else
                {
                    for(j=0; j<aiNumber; ++j)
                    {
                        if(strcmp(ai_list[j].name,enteredName) == 0) //logic to work out which ai goes here
                        {
                            foundName = 1;
                            break;
                        }
                    }
                }
            }
            if(foundName == 1)
            {
                newPlayer(object_list[i].x,object_list[i].y,ai_list[j].ai,object_list[i].direction); //add a player with the chosen ai to the game
            }

            //PRINT("%d\n",object_list[i].direction);
            destructor(i); // get rid of the starting location object
            --i; //this IS needed and explains some issues we have been having with destructor
        }
    }
    free(ai_list); //not needed any more
    reprintMap();

    /* Main Game Loop */
    while( 1 )
    {
        for(i=0; i<objectNumber; ++i)
        {
            if(update(&object_list[i]) == 1)//Allows objects to do stuff
            {
                --i;
            }
        }
        reprintMap();
#ifdef _WIN32
        Sleep(50);
        system("cls");
#else
        //PRINT("press Q to quit\n");
        refresh();
        mvprintw(0,0, "");
        usleep(100000);
#endif
    }


#ifndef _WIN32
    endwin();
#endif

    return 0;


}

void reprintMap()
{
    int i,j;
    for(i=0; i<xsize; ++i)
    {
        for(j=0; j<ysize; ++j)
        {
            map[i][j]=SPACE_SYMBOL;
        }
    }

    for(i=0; i<objectNumber; ++i)
    {
        if(object_list[i].type != 4)
        {
            map[object_list[i].x][object_list[i].y]=object_list[i].symbol;  //Looks through all the objects and places their symbol in their respective position.
        }
    }

    //Reprint the map
    for(i=0; i<ysize; ++i)
    {
        for(j=0; j<xsize; ++j)
        {
            PRINT("%c", map[j][i]);
        }
        PRINT("\n");
    }

    PRINT("objectNumber = %d\n", objectNumber);
}

void loadMap()
{

    /*A file should consist of the following:
        Line 0: an int giving the xsize
        Line 1: an int giving the ysize
        Rest of file: each line should contain four ints separated by spaces representing:
            Object type
            Object x location (0 <= x < xsize)
            Object y location (0 <= y < ysize)
            Object direction (ignored if a wall should be 2,4,6 or 8 for a bullet)
    */
    char * fileName = malloc(256 * sizeof(char));
    FILE * fileP = NULL;
    int type;
    int direction;
    int x,y;


    while(fileP == NULL)
    {
        PRINT("Please enter the name of the map:");
        scanf("%s",fileName);
        getchar();
        fileP = fopen(fileName,"r");
    }
    free(fileName);

    fscanf(fileP,"%d\n%d\n", &xsize,&ysize);

    while(!feof(fileP))
    {
        fscanf(fileP,"%d %d %d %d\n",&type,&x,&y,&direction);
        switch (type)
        {
        case 1:
            newWall(x,y);
            break;
        case 3:
            newBullet(x,y,direction);
            break;
        case 4:
            newMine(x,y);
            break;
        case 5:
            newStartingPosition(x,y,direction);
            break;
        }
    }
    fclose(fileP);

    return 0;
}

void defaultMap()
{
    int i;
    xsize = 25;
    ysize = 25;
    for(i=0; i<xsize; ++i)
    {
        newWall(i,0);

        newWall(i,ysize-1);
    }

    for(i=1; i<ysize-1; ++i)
    {
        newWall(0,i);

        newWall(xsize-1,i);
    }

    newStartingPosition(1,1,6);
    newStartingPosition(xsize-2,1,4);
    newStartingPosition(xsize-2,ysize-2,4);
    newStartingPosition(1,ysize-2,6);
}

int update(object *currentObject)
{

    switch((*currentObject).type)
    {
    case 1:
        break;
    case 2:
        return updatePlayer(currentObject);
    case 3:
        if(updateBullet(currentObject) == 1)    //this dodgy looking section just allows bullets to move twice as fast as other stuff!
        {
            return 1;
        }
        return updateBullet(currentObject);
    case 4:
        return updateMine(currentObject);
    default:
        // printf("Unknown object type found. Was it Greg?");
        break;
    }
    return 0;
}

int updatePlayer(object *currentObject)
{
    int aiChoice;
    int i;

    if((*currentObject).hp <= 0)
    {
        destructor((*currentObject).objId);
#ifdef _WIN32
        Beep(500,100);
#endif
        return 1;
    }

    aiChoice = ((*currentObject).ai)((*currentObject).x,(*currentObject).y,currentObject ,0);
    point nextPoint = next(currentObject);

    switch(aiChoice)
    {
    case 2:
        (*currentObject).direction=2;
        return 0;
        break;
    case 4:
        (*currentObject).direction=4;
        return 0;
        break;
    case 6:
        (*currentObject).direction=6;
        return 0;
        break;
    case 8:
        (*currentObject).direction=8;
        return 0;
        break;

    case 5:

        for(i=0; i<objectNumber; i++)   //Object list is an array of structs!!! Don't forget that!!!
        {
            if( object_list[i].x==(*currentObject).x + nextPoint.x && object_list[i].y==(*currentObject).y+nextPoint.y && (object_list[i].type==1 || object_list[i].type==2))
            {
                return 0;
            }
        }

        (*currentObject).x= (*currentObject).x+nextPoint.x;
        (*currentObject).y= (*currentObject).y+nextPoint.y;

        break;

    case 50:

        newBullet((*currentObject).x+nextPoint.x, (*currentObject).y+nextPoint.y, (*currentObject).direction);

    case 500:

        newMine((*currentObject).x+nextPoint.x, (*currentObject).y+nextPoint.y);

    case 42:            //To be completed...
        break;
    case 999:
        break;


    }

    return 0;
}

int updateBullet(object *currentObject)
{
    int i;
    point nextPoint = next(currentObject);

    for(i=0; i<objectNumber; i++)
    {
        if(((*currentObject).x + nextPoint.x ==object_list[i].x && (*currentObject).y + nextPoint.y==object_list[i].y))
        {
            if(object_list[i].type == 1)
            {
                destructor((*currentObject).objId);
                return 1;
            }
            else if(object_list[i].type == 2)
            {
                --(object_list[i].hp);
                destructor((*currentObject).objId);
                return 1;
            }
        }
    }
    ++(*currentObject).y;
    return 0;

}

int updateMine(object *currentObject)
{
    int i;
    for(i=0; i<objectNumber; ++i)
    {
        if(object_list[i].type == 2 && object_list[i].x == (*currentObject).x && object_list[i].y == (*currentObject).y)
        {
            object_list[i].hp = object_list[i].hp - 4;
            destructor((*currentObject).objId);
            return 1;
        }
    }

    return 0;
}

point next(object * currentObject)  //takes an object and works out its next position relative to it's current if it moves forward 1
{
    point n;

    if((*currentObject).direction==2)
    {
        n.x=0;
        n.y=1;
    }
    if((*currentObject).direction==4)
    {
        n.x=-1;
        n.y=0;
    }
    if((*currentObject).direction==6)
    {
        n.x=1;
        n.y=0;
    }
    if((*currentObject).direction==8)
    {
        n.x=0;
        n.y=-1;
    }

    return n;
}

void newWall(int x, int y)
{
    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=1;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].symbol=WALL_SYMBOL;

}

void newPlayer(int x, int y, int (*newFunct)(int,int,object *,int), int direction)
{
    int i;

    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=2;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].hp=10;
    object_list[objectNumber-1].ai=newFunct;
    object_list[objectNumber-1].data=malloc(sizeof(int)*100);
    object_list[objectNumber-1].localMap=malloc(sizeof(char*)*xsize);
    object_list[objectNumber-1].direction=direction;

    for(i=0; i<xsize; i++)
    {
        object_list[objectNumber-1].localMap[i]=malloc(sizeof(char)*ysize);  //In Kieran's memory.
    }


    object_list[objectNumber-1].symbol=(*newFunct)(0,0,&object_list[objectNumber-1],1);
}

void newBullet(int x, int y, int direction)
{
    int i;

    for(i=0; i<objectNumber; ++i)
    {
        if( ((object_list[i].x) == x) && ((object_list[i].y) == y) && ((object_list[i].type) == 1) )
        {
            // printf("There was a wall there!\n");
            return;
        }
    }

    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=3;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].direction=direction;
    object_list[objectNumber-1].symbol=BULLET_SYMBOL;
}

void newMine(int x, int y)
{
    int i;

    for(i=0; i<objectNumber; ++i)
    {
        if( ((object_list[i].x) == x) && ((object_list[i].y) == y) && (((object_list[i].type) == 4) || ((object_list[i].type) == 1)))
        {
            // printf("There was a wall there!\n");
            return;
        }
    }
    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=4;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].symbol='x';
}

void newAi_t(char * name,int (*ai)(int, int,struct obj *,int))
{
    ++aiNumber;
    ai_list = realloc(ai_list,aiNumber*sizeof(ai_t));
    ai_list[aiNumber-1].name = malloc(sizeof(char) * (strlen(name)+1));
    strcpy(ai_list[aiNumber-1].name,name);

    ai_list[aiNumber-1].name[strlen(ai_list[aiNumber-1].name)]='\0';

    printf("%s,%d\n",ai_list[aiNumber-1].name,strlen(ai_list[aiNumber-1].name));
    ai_list[aiNumber-1].ai = ai;
}

void newStartingPosition(int x,int y, int direction)
{
    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=5;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].symbol='°';
    object_list[objectNumber-1].direction=direction;
}

void destructor(int n)
{
    int i;

    for(i=n; i<objectNumber; ++i)
    {
        object_list[i]=object_list[i+1];
        --object_list[i].objId;
    }

    --objectNumber;
    object_list = realloc(object_list, (objectNumber)*sizeof(object));
}


