/* Plan

Priorities
0. Don't get hit by an incoming bullet
1. Record possible mine positions, never go on these suspected coordinates
2. If under 40% hp find three cells where your sure no mines were placed and dodge bullets from there
2. Line up with other players from a distance and fire
3. Lay mines if bored
4. If alone shoot the walls like a maniac

*/

#include "main.h"
#define MINEFLAG 1351
#define DIJK 10
void copymap(char **localMap,char **map, int flag);

int tito(int x,int y, object * me, int ask)
{

    int i,j;
    char **binaryMap;
    point start;
    point end;

    path trip;





    if(ask==1)
    {
        /* Zero the map array */
        for(i=0; i<xsize; ++i)
        {
            for(j=0; j<ysize; ++j)
            {
                ((*me).localMap)[i][j]=' ';
            }
        }

        return 14;
    }

    start.x=x;
    start.y=y;

    end.x=17;
    end.y=2;


    binaryMap = malloc(sizeof(int*)*xsize);
    for(i=0;i<xsize;i++)
    {
        binaryMap[i]=malloc(sizeof(int)*ysize);
    }

    copymap((*me).localMap,map,0);
    copymap(binaryMap,(*me).localMap,DIJK);
    binaryMap[x][y]=0;


/* Binary Map Print
        for(j=0; j<ysize; ++j)
        {
            for(i=0; i<xsize; ++i)
            {
                printf("%d", binaryMap[i][j]);
            }
            printf("\n");
        }
*/
    if((x != end.x) || (y != end.y))
    {
       //printf("\n%d %d\n", x,y);
       trip = dijkstra(binaryMap,start,end);
       (*me).x=trip.x[trip.length-1];
       (*me).y=trip.y[trip.length-1];
    }

/*
    for(i=0; i<trip.length; ++i)
    {
        printf("\n x= %d y=%d", trip.x[i], trip.y[i]);
    }
*/
 return 0;






}

void copymap(char **localMap,char **map, int flag)
{
    int i,j;

    for(j=0; j<ysize; ++j)
    {
        for(i=0; i<xsize; ++i)
        {
            if(localMap[i][j] != MINEFLAG)
            {
                localMap[i][j]=map[i][j];
            }
            else if((map[i][j] != SPACE_SYMBOL) && (map[i][j] != BULLET_SYMBOL) && (map[i][j] != WALL_SYMBOL))
            {
                localMap[i][j]=map[i][j];
            }
        }
    }

    if(flag==DIJK)
    {
        for(j=0;j<ysize;j++)
        {
            for(i=0;i<xsize;i++)
            {
                if(localMap[i][j] == SPACE_SYMBOL || localMap[i][j] == BULLET_SYMBOL)
                {
                    localMap[i][j]=0;
                }
                else
                {
                    localMap[i][j]=1;
                }

            }
        }
    }
}
