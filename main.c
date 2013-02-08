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

int xsize = 20;
int ysize = 20;

char **map;   //Grid which is printed

int main()
{
    int i,j,k;

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


    map = malloc((xsize+2)*sizeof(char *));
    for(i=0; i<xsize+2; ++i)
    {
        map[i]=malloc(ysize);
    }
    //newBullet(0,0,6);     //Test bullets
    //newBullet(18,1,6);
    newPlayer(10,8,&greg);
    newPlayer(15,14,&helen);


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
            map[object_list[i].x][object_list[i].y]=object_list[i].symbol;  //Looks through all the objects and places their symbol in their respective position.
        }

        //Reprint the map
        for(j=0; j<ysize; ++j)
        {
            for(k=0; k<xsize; ++k)
            {
                printf("%c", map[k][j]);
            }
            printf("\n");
        }

        printf("objectNumber = %d\n", objectNumber);

        for(k=77; k<objectNumber; ++k)
            printf("type=%d x=%d y=%d\n",object_list[k].type, object_list[k].x, object_list[k].y);
        Sleep(50);
        system("cls");

    }


    return 0;
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
    default:
        printf("Unknown object type found. Was it Greg?");
        break;
    }
    return 0;
}

int updatePlayer(object *currentObject)
{
    int aiChoice = ((*currentObject).ai)((*currentObject).x,(*currentObject).y,(*currentObject).objId ,0);
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
        Beep(500,100);

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
        break;
    case 400:
        break;
    case 600:
        break;
    case 800:
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

void newWall(int x, int y)
{

    ++objectNumber;

    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=1;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].symbol='Û';

}

void newPlayer(int x, int y, int (*newFunct)(int,int,int,int))
{
    ++objectNumber;
    object_list = realloc(object_list,objectNumber*sizeof(object));
    object_list[objectNumber-1].type=2;
    object_list[objectNumber-1].objId = objectNumber-1;
    object_list[objectNumber-1].x=x;
    object_list[objectNumber-1].y=y;
    object_list[objectNumber-1].hp=10;
    object_list[objectNumber-1].ai=newFunct;
    object_list[objectNumber-1].symbol=(*newFunct)(0,0,0,1);
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

int helen(int x,int y, int objId, int ask)
{


    if(ask==1)
    {
        object_list[objId].i=0;
        return 2;
    }

    ++object_list[objId].i;

    if(map[object_list[objId].x-1][object_list[objId].y]=='.')
       return 2;

    if(object_list[objId].i % 2 == 0)
        return 8;
    else
        return 8;

}

int greg(int x,int y, int objId, int ask)
{
    if(ask==1)
    {
        object_list[objId].i=0;
        return 2;
    }

    ++object_list[objId].i;

    if(object_list[objId].i % 2 == 0)
        return 8;
    else
        return 60;

}
