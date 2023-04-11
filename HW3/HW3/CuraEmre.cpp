/*********
   CTIS164 - Template Source Program
----------
STUDENT : Emre Cura
SECTION : 002
HOMEWORK: HW3 - Chasing and Hitting a Target Simulation
----------
PROBLEMS: I can't change the shape's position according to target. 
----------
ADDITIONAL FEATURES: You can increase (F1) or decrease altitude (F2). You can see mouse's x and y coordinates on the bottom right corner. 
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
bool missile = false;



typedef struct {
    vec_t pos;
    float angle;
    vec_t   vel;
} player_t;

typedef struct {
    vec_t pos;
    vec_t vel;  //velocity
} enemy_t;

player_t p;
enemy_t  e;


float A = 200, //amplitude
fq = 1,  //frequency
C = 0,   //horizontal phase shift
B = 0;   //vertical phase shift
float angle = 0;
bool enemy = 0;
double mx, my;

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}


float f(float x) {
    return A * sin((fq * x + C) * D2R) + B;
}

void drawAngle() {
    //DRONE'S PATH
    float r = fabs(A);

    glColor3ub(242, 147, 22);
    circle_wire(0, 0, r);

    circle(r * cos(angle * D2R), r * sin(angle * D2R), 5);

}

void drawDrone() {
    float r = fabs(A);
 
    p.pos.x = r * cos(angle * D2R);
    p.pos.y = r * sin(angle * D2R);

    //Main Body
    glColor3ub(46, 64, 83);
    glBegin(GL_POLYGON);
    glVertex2f(p.pos.x - 25, p.pos.y - 15);
    glVertex2f(p.pos.x + 25, p.pos.y - 15);
    glVertex2f(p.pos.x + 25, p.pos.y + 15);
    glVertex2f(p.pos.x - 25, p.pos.y + 15);
    glEnd();

    

    //propellers
    glLineWidth(3);
    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x + 25, p.pos.y + 5);
    glVertex2f(p.pos.x + 45, p.pos.y - 5);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x - 25, p.pos.y + 5);
    glVertex2f(p.pos.x - 45, p.pos.y - 5);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x - 45, p.pos.y - 5);
    glVertex2f(p.pos.x - 45, p.pos.y + 15);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x + 45, p.pos.y - 5);
    glVertex2f(p.pos.x + 45, p.pos.y + 15);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x - 45, p.pos.y + 14);
    glVertex2f(p.pos.x - 55, p.pos.y + 25);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x + 45, p.pos.y + 14);
    glVertex2f(p.pos.x + 55, p.pos.y + 25);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x - 45, p.pos.y + 14);
    glVertex2f(p.pos.x - 55, p.pos.y + 5);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x + 45, p.pos.y + 14);
    glVertex2f(p.pos.x + 55, p.pos.y + 5);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x + 45, p.pos.y + 14);
    glVertex2f(p.pos.x + 35, p.pos.y + 5);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x - 45, p.pos.y + 14);
    glVertex2f(p.pos.x - 35, p.pos.y + 5);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x - 45, p.pos.y + 14);
    glVertex2f(p.pos.x - 35, p.pos.y + 25);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x + 45, p.pos.y + 14);
    glVertex2f(p.pos.x + 35, p.pos.y + 25);
    glEnd();

    //FEETS OF DRONE
    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x -5, p.pos.y -14);
    glVertex2f(p.pos.x -20, p.pos.y -25);
    glEnd();

    glColor3ub(46, 64, 83);
    glBegin(GL_LINES);
    glVertex2f(p.pos.x + 5, p.pos.y - 14);
    glVertex2f(p.pos.x + 20, p.pos.y - 25);
    glEnd();

  
}



bool collision() {
    double dx = p.pos.x - e.pos.x;
    double dy = p.pos.y - e.pos.y;
    double d = sqrt(dx * dx + dy * dy);
    return d <= 20;
}

void drawMissile() {
    if (missile == true) {
        //MAIN BODY
        glColor3ub(46, 64, 83);
        glBegin(GL_POLYGON);
        glVertex2f(e.pos.x - 10, e.pos.y - 20);
        glVertex2f(e.pos.x + 10, e.pos.y - 20);
        glVertex2f(e.pos.x + 10, e.pos.y + 10);
        glVertex2f(e.pos.x - 10, e.pos.y + 10);
        glEnd();
        //ROCKET HEAD
        glColor3ub(222, 37, 48);
        glBegin(GL_TRIANGLES);
        glVertex2f(e.pos.x - 15, e.pos.y + 5);
        glVertex2f(e.pos.x, e.pos.y + 25);
        glVertex2f(e.pos.x + 15, e.pos.y + 5);
        glEnd();
        //ENGINES
        glColor3ub(222, 37, 48);
        glBegin(GL_POLYGON);
        glVertex2f(e.pos.x - 10, e.pos.y - 20);
        glVertex2f(e.pos.x - 2, e.pos.y - 20);
        glVertex2f(e.pos.x - 2, e.pos.y - 30);
        glVertex2f(e.pos.x -10, e.pos.y - 30);
        glEnd();
       
        glColor3ub(222, 37, 48);
        glBegin(GL_POLYGON);
        glVertex2f(e.pos.x + 2, e.pos.y - 20);
        glVertex2f(e.pos.x + 10, e.pos.y - 20);
        glVertex2f(e.pos.x + 10, e.pos.y - 30);
        glVertex2f(e.pos.x + 2, e.pos.y - 30);
        glEnd();
        //WINGS
        glColor3ub(222, 37, 48);
        glBegin(GL_TRIANGLES);
        glVertex2f(e.pos.x - 10, e.pos.y);
        glVertex2f(e.pos.x - 10, e.pos.y - 10);
        glVertex2f(e.pos.x - 25, e.pos.y - 5);
        glEnd();
        
        glColor3ub(222, 37, 48);
        glBegin(GL_TRIANGLES);
        glVertex2f(e.pos.x + 10, e.pos.y);
        glVertex2f(e.pos.x + 10, e.pos.y - 10);
        glVertex2f(e.pos.x + 25, e.pos.y - 5);
        glEnd();
    }
}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAngle();
    drawDrone();
    drawMissile();
    vprint(winWidth / 2 - 85, winHeight / 2 - 580, GLUT_BITMAP_8_BY_13, " %4.f %4.f", mx, my);
    vprint(winWidth / 2 - 1195, winHeight / 2 - 25, GLUT_BITMAP_8_BY_13, "F1 Increase Amplitude ");
    vprint(winWidth / 2 - 1195, winHeight / 2 - 40, GLUT_BITMAP_8_BY_13, "F2 Decrease Amplitude ");
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    case GLUT_KEY_F1:    A += 10; break;
    case GLUT_KEY_F2:    A -= 10; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    if (button == GLUT_LEFT && stat == GLUT_DOWN && missile == false) {
        e.pos = { mx, my };
        missile = true;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.


    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    mx = x - winWidth / 2;
    my = winHeight / 2 - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    angle++;
   
    e.vel = mulV(6, unitV(subV(p.pos, e.pos)));
    vec_t targetPos = e.pos;

    e.pos = addV(e.pos, e.vel);

    //controls the collision state
    if (collision())
        missile = false;


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Chasing and Hitting a Target Simulation - Emre Cura");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}