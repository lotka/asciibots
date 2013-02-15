#include "main.h"
#include <limits.h>

path voyage;

typedef struct nde
{
    int x;
    int y;
    int previous;
    int up,down,left,right;
    int distance;
    int visited;
} node;



path dijkstra(char **binMap, point start, point end)
{
    node *node_array;
    int i,j;
    int node_number=0;
    int starting=0;
    int current =0;
    int next =0;
    int finish=0;
    int pathLength=0;


    for(i=0; i<ysize; i++)
    {
        for(j=0; j<xsize; j++)
        {
            if(binMap[j][i]==0)
            {
                ++node_number;
            }
        }
    }

    node_array=malloc(node_number*sizeof(node));

    node_number=0;


    for(i=0; i<ysize; i++)
    {
        for(j=0; j<xsize; j++)
        {
            if(binMap[j][i]==0)
            {
                ++node_number;

                node_array[node_number-1].x=j;
                node_array[node_number-1].y=i;
                node_array[node_number-1].visited=0;
                node_array[node_number-1].up=-1;
                node_array[node_number-1].down=-1;
                node_array[node_number-1].right=-1;
                node_array[node_number-1].left=-1;

                if(node_array[node_number-1].x == start.x && node_array[node_number-1].y == start.y )
                {
                    starting=node_number-1;
                    current = node_number-1;
                    node_array[node_number-1].distance=0;
                }
                else if(node_array[node_number-1].x == end.x && node_array[node_number-1].y == end.y )
                {
                    finish = node_number -1;
                    node_array[node_number-1].distance=INT_MAX;
                }
                else
                {
                    node_array[node_number-1].distance=INT_MAX;
                }
            }
        }
    }

    for(i=0; i<node_number; ++i)
    {
        for(j=0; j<node_number; ++j)
        {
            if(node_array[j].x == node_array[i].x + 1 && node_array[j].y == node_array[i].y )
            {
                node_array[i].right = j;
               // break;
            }
            else if(node_array[j].x == node_array[i].x - 1 &&node_array[j].y == node_array[i].y )
            {
                node_array[i].left = j;
             //   break;
            }
            else if(node_array[j].x == node_array[i].x &&node_array[j].y == node_array[i].y -1 )
            {
                node_array[i].up = j;
             //   break;
            }
            else if(node_array[j].x == node_array[i].x &&node_array[j].y == node_array[i].y  +1)
            {
                node_array[i].down = j;
              //  break;
            }
        }
    }


    while(current != finish)
    {

        if(node_array[node_array[current].up].distance > node_array[current].distance && node_array[current].up != -1)
        {
            node_array[node_array[current].up].distance = node_array[current].distance+1;
           // printf("U%d     %d",current,node_array[node_array[current].up].previous);
            node_array[node_array[current].up].previous = current;
        }
        if(node_array[node_array[current].down].distance > node_array[current].distance && node_array[current].down != -1)
        {
            node_array[node_array[current].down].distance = node_array[current].distance+1;
          //  printf("D%d     %d",current,node_array[node_array[current].up].previous);
            node_array[node_array[current].down].previous = current;
        }
        if(node_array[node_array[current].left].distance > node_array[current].distance && node_array[current].left != -1)
        {
            node_array[node_array[current].left].distance = node_array[current].distance+1;
         //   printf("L%d     %d",current,node_array[node_array[current].up].previous);
            node_array[node_array[current].left].previous = current;
        }
        if(node_array[node_array[current].right].distance > node_array[current].distance && node_array[current].right != -1)
        {
            node_array[node_array[current].right].distance = node_array[current].distance+1;
          //  printf("R%d     %d",current,node_array[node_array[current].up].previous);
            node_array[node_array[current].right].previous = current;
        }

        node_array[current].visited=1;
        next=finish;

        for(i=0; i<node_number; i++)
        {
            if(node_array[i].distance<node_array[next].distance && node_array[i].visited == 0)
            {
                next=i;
            }

        }

        current = next;
    }

    current=finish;


    while(current != starting)
    {
        pathLength = pathLength +1;
        (voyage.x)=realloc(voyage.x,(pathLength+1)*sizeof(int));
        (voyage.y)=realloc(voyage.y,(pathLength+1)*sizeof(int));
        (voyage.x)[pathLength-1]=node_array[current].x;
        //printf("\n%d",(voyage.x)[pathLength-1]);
        (voyage.y)[pathLength-1]=node_array[current].y;
       // printf("\n%d",(voyage.y)[pathLength-1]);
        current=node_array[current].previous;
    }

    voyage.length=pathLength;


   return voyage;

}
