#include "main.h"
extern int objectNumber;
extern object *object_list;


void renderScene(void)
{
    int i;
    for(i=0; i<objectNumber; ++i)
    {
        if(update(&object_list[i]) == 1)//Allows objects to do stuff
        {
            --i;
        }
    }

    reprintMap();
    system("cls");


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(i=0; i<objectNumber; ++i)
    {
        draw(&object_list[i]);
    }
    glFlush();
    glutSwapBuffers();
}


void draw(object * currentObject)
{

    double X = 2*(*currentObject).x/(double)xsize - 1;
    double Y = 2*(*currentObject).y/(double)ysize - 1;
    switch((*currentObject).type){
        case 0:
            break;
        case 1:
            drawWall(X,Y);
            break;
        case 2:
            drawPlayer(X,Y);
            break;
        case 3:
            drawBullet(X,Y);
            break;
        default:
            break;
    }
    //drawWall(X,Y);

}
void drawWall(double X, double Y)
{
    glColor3f(0,0.0756,1);

    glBegin(GL_POLYGON);
    glVertex2d(X,Y);
    glVertex2d(X,Y + 2/(double)ysize);
    glVertex2d(X+ 2/(double)xsize,Y + 2/(double)ysize);
    glVertex2d(X+ 2/(double)xsize,Y);
    glEnd();
}

void drawBullet(double X, double Y)
{
    glColor3f(1,1,1);

    glBegin(GL_POLYGON);
    glVertex2d(X+ 0.25/(double)xsize,Y+ 0.25/(double)ysize);
    glVertex2d(X+ 0.25/(double)xsize,Y + 0.5/(double)ysize);
    glVertex2d(X+ 0.5/(double)xsize,Y + 0.5/(double)ysize);
    glVertex2d(X+ 0.5/(double)xsize,Y+ 0.25/(double)ysize);
    glEnd();
}
void drawPlayer(double X, double Y)
{
    glColor3f(1,0,0);

    glBegin(GL_POLYGON);
    glVertex2d(X,Y);
    glVertex2d(X,Y + 1/(double)ysize);
    glVertex2d(X+ 1/(double)xsize,Y + 1/(double)ysize);
    glVertex2d(X+ 1/(double)xsize,Y);
    glEnd();
}
