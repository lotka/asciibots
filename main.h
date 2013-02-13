#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>
	#define PRINT(...) printf(__VA_ARGS__)
#else
	#include <ncurses.h>
	#define PRINT(...) printw(__VA_ARGS__)
#endif


int xsize; //these get set in either loadMap() or defaultMap()
int ysize;


typedef struct obj
{
    int type;
    int objId;
    int hp;
    int x, y, direction;
    char symbol;

    //For players only
    int (*ai)(int, int,struct obj *,int);
    int i,j,k;
    char **localMap;
    int *data;

} object;

typedef struct{
    char * name;
    int (*ai)(int, int,struct obj *,int);
}ai_t;


//Note that loadMap and defaultMap are probably bad names. They should  be load object_list & default object_list.
void loadMap();
void defaultMap();

void reprintMap();

void newBullet(int,int,int);
void newWall(int,int);
void newPlayer(int,int ,int (*)(int,int,object *,int));
void newMine(int , int );
void newAi_t(char *,int (*)(int, int,struct obj *,int));
void newStartingPosition(int ,int);
void destructor(int);
int update(object *);
int updatePlayer(object *);
int updateBullet(object *);
int updateMine(object *);
/*Ais*/
int greg(int,int,object *,int);
int human(int,int,object *,int);

char **map;   //Grid which is printed
