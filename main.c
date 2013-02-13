#include "main.h"

/* Object types:
   0 - free space
   1 - wall
   2 - player
   3 - bullet
   4 - mine
   5 - starting position //only used during map loading or making default map

   Action types:
   2,4,6,8 - Move like a numpad
   20,40,60,80 - FIRE!
   200,400,600,800, - drop mine
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
#ifndef _WIN32
    initscr();
    raw();
#endif
    int i,j;
    char c,d;
    char *enteredName = malloc(256*sizeof(char));
    char nonep[] = "none";
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
                PRINT("Please enter the ai to go in position (%d,%d) (\"none\" to leave space empty)):",object_list[i].x,object_list[i].y);
                gets(enteredName);
                if(strcmp(nonep,enteredName) == 0) //logig required to leave the space empty
                {
                    foundName= -1;
                }else{
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
                newPlayer(object_list[i].x,object_list[i].y,ai_list[j].ai); //add a player with the chosen ai to the game
            }
            destructor(i); // get rid of the starting location object
            --i; //this IS needed and explains some issues we have been having with destructor
        }
    }
    free(ai_list); //not needed any more

    while(1)
    {
        for(i=0; i<objectNumber; ++i)
        {
            update(&object_list[i]); //Allows objects to do stuff
        }

        reprintMap();

        // for(k=77; k<objectNumber; ++k)
        //  {
        //       PRINT("type=%d x=%d y=%d\n",object_list[k].type, object_list[k].x, object_list[k].y);
        //   }
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
    endwin();
    return 0;
}

void reprintMap()
{
    int i,j;
    for(i=0; i<xsize; ++i)
    {
        for(j=0; j<ysize; ++j)
        {
            map[i][j]=' ';
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
        fileP = fopen(fileName,"r");
    }
    free(fileName);

    fscanf(fileP,"%d\n%d\n", &xsize,&ysize);

    while(!feof(fileP))
    {
        fscanf(fileP,"%d %d %d %d\n",&type,&x,&y,&direction);
        switch (type)   //when mines are implemented this needs extending
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
            newStartingPosition(x,y);
            break;
        }
    }
    fclose(fileP);

    return;
}

void defaultMap()
{
    int i;
    xsize = 20;
    ysize = 20;
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
    newStartingPosition(1,1);
    newStartingPosition(xsize-2,1);
    newStartingPosition(xsize-2,ysize-2);
    newStartingPosition(1,ysize-2);
}

int update(object *currentObject)
{
    switch((*currentObject).type)
    {
    case 1:
        break;
    case 2:
        updatePlayer(currentObject);
        break;
    case 3:
        updateBullet(currentObject);
        updateBullet(currentObject);
        break;
    case 4:
        updateMine(currentObject);
        break;
    default:
        // printf("Unknown object type found. Was it Greg?");
        break;
    }
    return 0;
}

int updatePlayer(object *currentObject)
{
    int aiChoice = ((*currentObject).ai)((*currentObject).x,(*currentObject).y,currentObject ,0);
    int i;
    //printf("%c", aiChoice);
    /*

     2,4,6,8 - Move like a numpad
       20,40,60,80 - FIRE!
       200,400,600,800, - drop mine
       42 - offer peace
       999 - call the police */

    if((*currentObject).hp <= 0)
    {
        destructor((*currentObject).objId);
#ifdef _WIN32
        Beep(500,100);
#endif

    }
    switch(aiChoice)
    {
    case 2:
        for(i=0; i<objectNumber; i++)
        {
            if((*currentObject).x==object_list[i].x && (*currentObject).y+1==object_list[i].y)
            {
                if(object_list[i].type == 1)
                {
                    return 0;

                }
            }
        }
        ++(*currentObject).y;
        return 0;
        break;
    case 4:
        for(i=0; i<objectNumber; i++)
        {
            if((*currentObject).x-1==object_list[i].x && (*currentObject).y==object_list[i].y)
            {
                if(object_list[i].type == 1)
                {
                    return 0;

                }
            }
        }
        --(*currentObject).x;
        return 0;

        break;
    case 6:
        for(i=0; i<objectNumber; i++)
        {
            if((*currentObject).x+1==object_list[i].x && (*currentObject).y==object_list[i].y)
            {
                if(object_list[i].type == 1)
                {
                    return 0;

                }
            }
        }
        ++(*currentObject).x;
        return 0;
        break;
    case 8:
        for(i=0; i<objectNumber; i++)
        {
            if((*currentObject).x==object_list[i].x && (*currentObject).y-1==object_list[i].y)
            {
                if(object_list[i].type == 1)
                {
                    return 0;
                }
            }
        }
        --(*currentObject).y;
        return 0;
        break;
    case 20:
        newBullet((*currentObject).x,(*currentObject).y +1,2);
        break;
    case 40:
        newBullet((*currentObject).x-1,(*currentObject).y,4);
        break;
    case 60:
        newBullet((*currentObject).x+1,(*currentObject).y,6);
        break;
    case 80:
        newBullet((*currentObject).x,(*currentObject).y-1,8);
        break;
    case 200:
        newMine((*currentObject).x,(*currentObject).y+1);
        break;
    case 400:
        newMine((*currentObject).x-1,(*currentObject).y);
        break;
    case 600:
        newMine((*currentObject).x+1,(*currentObject).y);
        break;
    case 800:
        newMine((*currentObject).x,(*currentObject).y-1);
        break;
    case 42:
        break;
    case 999:
        break;


    }

    return 0;
}

int updateBullet(object *currentObject)
{
    int i;

    switch((*currentObject).direction)
    {
    case 2:

        for(i=0; i<objectNumber; i++)
        {
            if((*currentObject).x==object_list[i].x && (*currentObject).y+1==object_list[i].y)
            {
                if(object_list[i].type == 1)
                {
                    destructor((*currentObject).objId);
                    return 0;
                }
                else if(object_list[i].type == 2)
                {
                    --(object_list[i].hp);
                    destructor((*currentObject).objId);
                    return 0;
                }
            }
        }
        ++(*currentObject).y;
        return 0;
    case 4:
        for(i=0; i<objectNumber; i++)
        {
            if((*currentObject).x-1==object_list[i].x && (*currentObject).y==object_list[i].y)
            {
                if(object_list[i].type == 1)
                {
                    (*currentObject).direction=6;
                    destructor((*currentObject).objId);
                    return 0;
                }
                else if(object_list[i].type == 2)
                {
                    --(object_list[i].hp);
                    destructor((*currentObject).objId);
                    return 0;
                }

            }
        }
        --(*currentObject).x;
        return 0;
    case 6:
        for(i=0; i<objectNumber; i++)
        {
            if((*currentObject).x+1==object_list[i].x && (*currentObject).y==object_list[i].y)
            {
                if(object_list[i].type == 1)
                {
                    (*currentObject).direction=4;
                    destructor((*currentObject).objId);
                    // printf("The destructor was triggered!\n");
                    return 0;
                }
                else if(object_list[i].type == 2)
                {
                    --(object_list[i].hp);
                    destructor((*currentObject).objId);
                    return 0;
                }
            }
        }
        ++(*currentObject).x;
        return 0;
    case 8:
        for(i=0; i<objectNumber; i++)
        {
            if((*currentObject).x==object_list[i].x && (*currentObject).y-1==object_list[i].y)
            {
                if(object_list[i].type == 1)
                {
                    (*currentObject).direction=2;
                    destructor((*currentObject).objId);
                    return 0;
                }
                else if(object_list[i].type == 2)
                {
                    --(object_list[i].hp);
                    destructor((*currentObject).objId);
                    return 0;
                }
            }
        }
        --(*currentObject).y;
        return 0;
    }


    return 0;
}

int updateMine(object *currentObject)
{
    int i;
    for(i=0; i<objectNumber; ++i)
    {
        if(object_list[i].type == 2 && object_list[i].x == (*currentObject).x && object_list[i].y == (*currentObject).y)
        {
            object_list[i].hp = object_list[i].hp - 5;
            destructor((*currentObject).objId);
        }
    }

    return 0;
}

void newWall(int x, int y)
{
    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=1;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].symbol='²';

}

void newPlayer(int x, int y, int (*newFunct)(int,int,object *,int))
{
    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=2;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].hp=10;
    object_list[objectNumber-1].ai=newFunct;
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
    object_list[objectNumber-1].symbol='.';

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
    ai_list[aiNumber-1].name = malloc(sizeof(char) * (strlen(name) +1));
    strcpy(ai_list[aiNumber-1].name,name);
    ai_list[aiNumber-1].ai = ai;
}
void newStartingPosition(int x,int y)
{
    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=5;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].symbol='°';
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


