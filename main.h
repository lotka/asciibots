#include <stdio.h>

#include <windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#define BULLET_SYMBOL '.'
#define WALL_SYMBOL '�'
#define SPACE_SYMBOL ' '


/* Objects */
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

typedef struct{
    int x;
    int y;
}point;


/* Map intizilation */
//Note that loadMap and defaultMap are probably bad names. They should  be load object_list & default object_list.
void loadMap();
void defaultMap();
void reprintMap();


/* Object constructors and destructor */
void newBullet(int,int,int);
void newWall(int,int);
void newPlayer(int,int ,int (*)(int,int,object *,int),int);
void newMine(int , int );
void newAi_t(char *,int (*)(int, int,struct obj *,int));
void newStartingPosition(int ,int,int);
void destructor(int);

/* Glut */
void renderScene(void);
void draw(object * obj);
void drawWall(double X, double Y);
void drawBullet(double X, double Y);
void drawPlayer(double X, double Y);


/* Object updating */
int update(object *);
int updatePlayer(object *);
int updateBullet(object *);
int updateMine(object *);

point next(object *);

/*AI Stuff*/
int greg(int,int,object *,int);
int human(int,int,object *,int);
int tito(int,int,object *,int);

typedef struct{
   int *x;
   int *y;
   int length;
} path;

path dijkstra(char **map, point start, point end);


/* Map Information */
char **map; //Grid which is printed
int xsize;  //these get set in either loadMap() or defaultMap()
int ysize;

