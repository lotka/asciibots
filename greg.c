#include "main.h"
#include <math.h>



int greg(int x,int y, object * me, int ask)
{
    if(ask==1)
    {
        (*me).i=0;
        return 2;
    }

    int randNo = rand();

    switch((int)floor(7*((randNo/(double)RAND_MAX)))){
        case 0:
            return 2;
        case 1:
            return 4;
        case 2:
            return 6;
        case 3:
            return 8;
        case 4:
            return 5;
        case 5:
            return 50;
        case 6:
            return 500;
    }


  return 0;


}

