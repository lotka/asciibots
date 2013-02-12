#include "main.h"

int greg(int x,int y, object * me, int ask)
{
    if(ask==1)
    {
        (*me).i=0;
        return 2;
    }


    ++(*me).i;

    if((*me).i % 2 == 0)
        return 8;
    else
        return 200;

}

