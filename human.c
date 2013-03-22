#include "main.h"

int human(int x,int y, object * me, int ask)
{
    int i,j,c;

    if(ask == 1)
    {
        (*me).i = 0;
        return 2;
    }

    ++((*me).i);

    if( (*me).i > 1)
    {

        for(j=0; j<ysize; ++j)
        {
            for(i=0; i<xsize; ++i)
            {
                printf("%c", map[i][j]);
            }
            printf("\n");
        }

       printf("\nEnter command for player:");

       scanf("%d", &c);

       return c;
    }

    return 0;
}
