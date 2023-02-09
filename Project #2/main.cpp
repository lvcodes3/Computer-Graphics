//
//  main.cpp
//  Project #2
//
//  Created by Luis Valencia on 9/19/22.
//  CSCI 172
//
// Using OpenGL + GLUT Frameworks on MacOS, works 100%

#define GL_SILENCE_DEPRECATION      // used to silence OpenGL deprecation warnings for MacOS
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* MY GLOBALS */
// these bool variables are used to help control which object we want to actually move, for example we can
// say that if cPress is true, then we will check which arrow key is pressed and perform the operation needed
// and only apply the operation to cube bc cPress is true, and the rest are false and so on
bool cPress, tPress, sPress;

// these next 3 variables will be used to control the z-axis of our objects when translating (glTranslatef())
static float zCube = -2.0;
static float zTeapot = -3.0;
static float zSphere = -1.0;

// these next 3 variables will be used to control the rotation of our objects (glRotatef())
static float cubeRot = 0.0;
static float teapotRot = 0.0;
static float sphereRot = 0.0;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    // unused variable Ratio ?
     //double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,1,0.1f, 100.0f);
 }


// display function is running in a loop
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);        //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        //Toggle WIRE FRAME

    /* START MY CODE HERE */
    
    // drawing solid cube of size 2, at pos x=4.0,y=0.0,z=-2.0, color green
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(4.0, 0.0, zCube);      // only modifying the z axis
    glRotatef(cubeRot, 0.0, 1.0, 0.0);  // rotating around y axis
    glutSolidCube(2);
    glPopMatrix();
    
    // drawing solid teapot of size 1.5, at pos x=-0.0,y=0.0,z=-3.0, color red
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(-0.0, 0.0, zTeapot);     // only modifying the z axis
    glRotatef(teapotRot, 0.0, 1.0, 0.0);  // rotating around y axis
    glutSolidTeapot(1.5);
    glPopMatrix();
    
    // drawing solid sphere of rad 1.3, slices 16, stacks 16, at pos x=-3.5,y=0.0,z=-1.0, color blue
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(-3.5, 0.0, zSphere);     // only modifying the z axis
    glRotatef(sphereRot, 0.0, 1.0, 0.0);  // rotating around y axis
    glutSolidSphere(1.3, 16, 16);
    glPopMatrix();
    
    /* END MY CODE HERE */

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        // c key is pressed (cube)
        case 'c':
            // set press bool values
            cPress = true;
            tPress = false;
            sPress = false;
            break;
            
        // t key is pressed (teapot)
        case 't':
            // set press bool values
            cPress = false;
            tPress = true;
            sPress = false;
            break;
            
        // s key is pressed (sphere)
        case 's':
            // set press bool values
            cPress = false;
            tPress = false;
            sPress = true;
            break;
        
        // added feature: a key is pressed (selects all three objects, instead of just one)
        case 'a':
            // set press bool values
            cPress = false;
            tPress = false;
            sPress = false;
            break;
            
        // q key is pressed (quit)
        case 27 :
        case 'q':
            exit(0);
            break;
            
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key){
            
        // increase the z-value in glTranslatef of specified object
        case GLUT_KEY_UP:
            if(cPress){
                zCube += 0.5;
            }
            else if(tPress){
                zTeapot += 0.5;
            }
            else if(sPress){
                zSphere += 0.5;
            }
            else{
                // if no single object is true, then all are false, means perform operation to all objects at once
                zCube += 0.5;
                zTeapot += 0.5;
                zSphere += 0.5;
            }
            break;
            
        // decrease the z-value in glTranslatef of specified object
        case GLUT_KEY_DOWN:
            if(cPress){
                zCube -= 0.5;
            }
            else if(tPress){
                zTeapot -= 0.5;
            }
            else if(sPress){
                zSphere -= 0.5;
            }
            else{
                // if no single object is true, then all are false, means perform operation to all objects at once
                zCube -= 0.5;
                zTeapot -= 0.5;
                zSphere -= 0.5;
            }
            break;
            
        // decrease the angle (makes it go negative // clockwise) of specified object
        case GLUT_KEY_LEFT:
            if(cPress){
                cubeRot -= 5.0;
            }
            else if(tPress){
                teapotRot -= 5.0;
            }
            else if(sPress){
                sphereRot -= 5.0;
            }
            else{
                // if no single object is true, then all are false, means perform operation to all objects at once
                cubeRot -= 5.0;
                teapotRot -= 5.0;
                sphereRot -= 5.0;
            }
            break;
            
        // increase the angle (makes it go positive // counter-clockwise) of specified object
        case GLUT_KEY_RIGHT:
            if(cPress){
                cubeRot += 5.0;
            }
            else if(tPress){
                teapotRot += 5.0;
            }
            else if(sPress){
                sphereRot += 5.0;
            }
            else{
                // if no single object is true, then all are false, means perform operation to all objects at once
                cubeRot += 5.0;
                teapotRot += 5.0;
                sphereRot += 5.0;
            }
            break;
            
    }
    // redisplay the changes
    glutPostRedisplay();
}


// idle function is running in a loop
static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    // professor said to comment out
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
