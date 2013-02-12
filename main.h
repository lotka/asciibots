#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>
	#define PRINT(...) printf(__VA_ARGS__)
#else
	#include <ncurses.h>
	#define PRINT(...) printw(__VA_ARGS__)
#endif

typedef struct obj
{
    int type;
    int objId;
    int hp;
    int x, y, direction;
    int (*ai)(int, int,struct obj *,int);
    char symbol;
    int i;
} object;


//Note that loadMap and defaultMap are probably bad names. They should  be load object_list & default object_list.
void loadMap();
void defaultMap();

void newBullet(int,int,int);
void newWall(int,int);
void newPlayer(int,int ,int (*)(int,int,object *,int));
void destructor(int);
int update(object *);
int updatePlayer(object *);
int updateBullet(object *);
/*Ais*/
int greg(int,int,object *,int);

char **map;   //Grid which is printed
