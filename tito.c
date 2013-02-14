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
void copymap(char **, char **, int);

int tito(int x,int y, object * me, int ask)
{

    int i,j;

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

    copymap(map,(*me).localMap,0);


    return 5;




}

void copymap(char **map, char **localMap, int flag)
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
        }
    }
}
