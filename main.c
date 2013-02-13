#include "main.h"

/* Object types:
   0 - free space
   1 - wall
   2 - player
   3 - bullet
   4 - mine

   Action types:
   2,4,6,8 - Move like a numpad
   20,40,60,80 - FIRE!
   200,400,600,800, - drop mine
   42 - offer peace
   999 - call the police
*/

int objectNumber=0;  //Counts number of objects

object *object_list;   //Stores all objects.

int main()
{
#ifndef _WIN32
    initscr();
    raw();
#endif
    int i,j,k;
    char c,d;

    do
    {
        printf("Do you want to load a map from file (y/n) (default is basic square):");
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

    map = malloc((xsize+2)*sizeof(char *));
    for(i=0; i<xsize+2; ++i)
    {
        map[i]=malloc(ysize);
    }
    //newBullet(0,0,6);     //Test bullets
    //newBullet(18,1,6);

    while(1)
    {
        for(i=0; i<objectNumber; ++i)
        {
            update(&object_list[i]); //Allows objects to do stuff
        }

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
        for(j=0; j<ysize; ++j)
        {
            for(k=0; k<xsize; ++k)
            {
                PRINT("%c", map[k][j]);
            }
            PRINT("\n");
        }

        PRINT("objectNumber = %d\n", objectNumber);

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
        printf("Please enter the name of the map:");
        scanf("%s",fileName);
        fileP = fopen(fileName,"r");
    }
    free(fileName);

    fscanf(fileP,"%d\n%d\n", &xsize,&ysize);

    while(!feof(fileP)){
        fscanf(fileP,"%d %d %d %d\n",&type,&x,&y,&direction);
        switch (type){  //when mines are implemented this needs extending
            case 1: newWall(x,y);
                break;
            case 3: newBullet(x,y,direction);
                break;
        }
    }
    close(fileP);

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

    newPlayer(1,ysize -4,&greg);
    newPlayer(3,3,&human);
    newPlayer(xsize-2,ysize-2,&greg);

    newMine(1,3);
    newMine(1,1);
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
        printf("Unknown object type found. Was it Greg?");
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
    for(i=0;i<objectNumber;++i){
        if(object_list[i].type == 2 && object_list[i].x == (*currentObject).x && object_list[i].y == (*currentObject).y){
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


