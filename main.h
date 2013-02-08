#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct
{
    int type;
    int objId;
    int hp;
    int x, y, direction;
    int (*ai)(int, int,int,int);
    char symbol;
    int i;
} object;

void newBullet(int,int,int);
void newWall(int,int);
void newPlayer(int,int ,int (*)(int,int,int,int));
void destructor(int);
int update(object *);
int updatePlayer(object *);
int updateBullet(object *);
/*Ais*/
int greg(int,int,int,int);
int helen(int,int,int,int);
object *object_list;   //Stores all objects.
