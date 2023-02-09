//
//  main.cpp
//  SOILProject
//
//  Created by Luis Valencia on 10/1/22.
//
// READ ME: In this program the earth will remain in the center place, while being able to rotate on its own axis.
// The moon will orbit around the earth, while being able to rotate on its own axis. The mars will also orbit around
// the earth, while being able to rotate on its own axis. The sun will be on the far left of the screen, you will have
// to zoom out to see it, and will remain in that position, while being able to rotate on its own axis. Considering
// the position of the sun, you will see the light hit the left side of the planets, and will have a dark shadow
// effect on the right side of the planet. You can zoom in with the up key and zoom out with the down key.
// You can rotate the entire scene to the left with the left key and to the right with the right key.

#define GL_SILENCE_DEPRECATION      // used to silence OpenGL deprecation warnings for MacOS
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <SOIL.h>

#define PI 3.14159

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

float Wwidth,Wheight;

/* MY GLOBALS */
// global Texture pointers (each planet will have a different texture)
GLuint earthTex;
GLuint moonTex;
GLuint marsTex;
GLuint sunTex;

GLUquadricObj *sphere = NULL;       // null sphere object used to display planets

bool startSim = false;              // will start space simulation on space press

static float rotateScene = 0.0;     // controls the left or right rotation of entire scene
static float zoomScene = 0.0;       // controls the zoom in or out of entire scene
static float earthRot = 0.0;        // controls the axis rotation of earth
static float moonOrbit = 0.0;       // controls the moon orbit around earth
static float moonRot = 0.0;         // controls the axis rotation of moon
static float marsOrbit = 0.0;       // controls the mars orbit around Earth
static float marsRot = 0.0;         // controls the axis rotation of mars
static float sunRot = 0.0;          // controls the axis rotation of sun


/* GLUT callback Handlers */

static void resize(int width, int height) {
    double Ratio;
    
    Wwidth = (float)width;
    Wheight = (float)height;
    
    Ratio= (double)width /(double)height;
    
    glViewport(0,0,(GLsizei) width,(GLsizei) height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective (45.0f,Ratio,0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10.0,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);        //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        //Toggle WIRE FRAME

    /* MY CODE STARTS HERE */
    
    // main glPushMatrix to manipulate the entire scene
    glPushMatrix();
    
    glRotatef(rotateScene, 0.0, 1.0, 0.0);      // rotating the entire scene left or right by the y-axis
    glTranslatef(0.0, 0.0, zoomScene);          // zooming the entire scene in or out

        // initialize the earth sphere
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, earthTex); // giving earth its image texture
        glTranslatef(0.0, 0.0, 0.0);            // centering earth
        glRotatef(-90.0, 1.0, 0.0, 0.0);        // rotating -90.0 around x-axis (poles get positioned vertically correctly)
        glRotatef(80.0, 0.0, 0.0, 1.0);         // rotating 80.0 around z-axis (earth position starts with North and South America facing us)
        glRotatef(earthRot, 0.0, 0.0, 1.0);     // earth will constantly rotate around z-axis
        gluSphere(sphere, 1.0, 40, 40);         // creating earth sphere object
        glPopMatrix();
        
        // initialize the moon sphere
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, moonTex);  // giving moon its image texture
        glTranslatef(0.0, 0.0, 0.0);            // centering moon
        glRotatef(moonOrbit, 0.0, 1.0, 0.0);    // rotating with respect to y-axis (orbiting around earth)
        glTranslatef(2.0, 0.0, 0.0);            // moon will be translated 2 to the right
        glRotatef(moonRot, 0.0, 0.0, 0.0);      // rotating with respect to z-axis (rotating moon on its own axis)
        gluSphere(sphere, 0.25, 40, 40);        // creating moon sphere object
        glPopMatrix();
        
        // initialize the mars sphere
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, marsTex);  // giving mars its image texture
        glTranslatef(0.0, 0.0, 0.0);            // centering mars
        glRotatef(marsOrbit, 0.0, 1.0, 0.0);    // rotating with respect to y-axis (orbiting around the earth)
        glTranslatef(5.0, 0.0, 0.0);            // mars will be translated 5 to the right
        glRotatef(marsRot, 0.0, 1.0, 0.0);      // rotating with respect to z-axis (rotating mars on its own axis)
        glRotatef(90.0, 1.0, 0.0, 0.0);         // rotating 90.0 around x-axis (poles get positioned vertically correctly)
        gluSphere(sphere, 0.5, 40, 40);         // creating mars sphere object
        glPopMatrix();
    
        // initialize the sun sphere
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, sunTex);   // giving sun its image texture
        glTranslatef(-20.0, 0.0, 0.0);          // sun will be on far right, out of frame, zoom out to see the sun
        glRotatef(-90.0, 1.0, 0.0, 0.0);        // rotating -90.0 around x-axis (poles get positioned vertically correctly)
        glRotatef(sunRot, 0.0, 0.0, 1.0);       // sun will constantly rotate around z-axis
        gluSphere(sphere, 5.0, 40, 40);         // creating sun sphere object
        glPopMatrix();
    
    // glPopMatrix for manipulate entire scene
    glPopMatrix();
    
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y) {
    switch (key){
        // if space key is pressed start the simulation, then stop when pressed again
        case ' ':
            if(startSim){
                startSim = false;
            } else{
                startSim = true;
            }
            break;
        // if w key is pressed show the wireframe
        case 'w':
            WireFrame =! WireFrame;
            break;
        // if q key is pressed terminate the program
        case 27 :
        case 'q':
            exit(0);
            break;
    }
}

void Specialkeys(int key, int x, int y) {
    switch(key) {
        // if left key is pressed rotate entire scene to the left
        case GLUT_KEY_LEFT:
            rotateScene -= 1.0;
            break;
        // if right key is pressed rotate entire scene to the right
        case GLUT_KEY_RIGHT:
            rotateScene += 1.0;
            break;
        // if up key is pressed zoom in the scene
        case GLUT_KEY_UP:
            zoomScene += 1.0;
            break;
        // if down key is pressed zoom out the scene
        case GLUT_KEY_DOWN:
            zoomScene -= 1.0;
            break;
    }
    glutPostRedisplay();
}


static void idle(void) {
    // if space was pressed then start the simulation, if it is pressed again it will stop the simulation
    if(startSim){
        earthRot += 5.0;    // speed at which earth will rotate on its own axis
        moonRot += 0.5;     // speed at which moon will rotate on its own axis
        moonOrbit += 2.0;   // speed at which moon will orbit around earth
        marsRot += 4.9;     // speed at which mars will rotate on its own axis
        marsOrbit += 0.2;   // speed at which mars will orbit around earth
        sunRot += 0.5;      // speed at which sun will rotate on its own axis
    }
    glutPostRedisplay();
}


// was not assigned to use mouse for this project
void mouse(int btn, int state, int x, int y){
   // float scale = 54*(Wwidth/Wheight);
   //  printf(" original %d %d \n", x,y);
    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

        }
        break;

        case GLUT_MIDDLE_BUTTON:
            break;

        case GLUT_RIGHT_BUTTON:
            break;
    }
    glutPostRedisplay();
};


// TextureLoad uses SOIL to load textures and wrap them around an object
void TextureLoad(char* filename, GLuint &tex){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height;  // width & height for the image reader
    unsigned char* image;

    image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
    // making sure the image loads, or else log the error
    if (image == NULL) {
        cout << SOIL_last_result() << endl;
        exit(EXIT_FAILURE);
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    // binding image data
    SOIL_free_image_data(image);    // Able to discard image since we are done with it, saves memory

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static void init(void) {
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    
    // sun is located to the left of earth, therefore the light from the sun will hit the left
    // side of the planets
    GLfloat lightpos[] =  {-10, 10, 10, 1.0};

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    // adding the lightpos
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    
    // initialize the sphere
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    
    // loading earth image texture
    glGenTextures(1, &earthTex); // Possible to have 1D, 2D and even 3D textures
    char p1[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/SOILProject/images/earth.jpeg";
    TextureLoad(p1, earthTex);
    
    // loading moon image texture
    glGenTextures(1, &moonTex); // Possible to have 1D, 2D and even 3D textures
    char p2[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/SOILProject/images/moon.jpeg";
    TextureLoad(p2, moonTex);
    
    // loading mars image texture
    glGenTextures(1, &marsTex); // Possible to have 1D, 2D and even 3D textures
    char p3[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/SOILProject/images/mars.jpeg";
    TextureLoad(p3, marsTex);
    
    // loading sun image texture
    glGenTextures(1, &sunTex); // Possible to have 1D, 2D and even 3D textures
    char p4[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/SOILProject/images/sun.jpeg";
    TextureLoad(p4, sunTex);
}

/* Program entry point */

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("SOIL Project 3");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
