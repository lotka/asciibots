#include "main.h"

int greg(int x,int y, object * me, int ask)
{
    if(ask==1)
    {
        (*me).i=0;
        return 2;
    }


    ++(*me).i;

    if((*me).i > 240)
       return 6;

    if((*me).i > 160)
       return 2;

    if((*me).i > 80)
       return 4;

    if((*me).i % 3 == 0){
        return 8;
    }
    else if((*me).i % 3 == 1){
        return 40;
    }
    else{
    return 200;
    }


}

