//
//  main.cpp
//  Skybox
//
//  Created by Luis Valencia on 10/27/22.
//

// used to silence OpenGL deprecation warnings for MacOS
#define GL_SILENCE_DEPRECATION
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <SOIL.h>
#include <vector>

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

/* GLOBALS */
// GLuint textures
GLuint front_texture;
GLuint back_texture;
GLuint top_texture;
GLuint bottom_texture;
GLuint left_texture;
GLuint right_texture;
GLuint tnt_side_texture_1;
GLuint tnt_side_texture_2;
GLuint tnt_side_texture_3;
GLuint tnt_side_texture_4;
GLuint tnt_top_texture;
GLuint tnt_bottom_texture;

// used to move the cube in x dir
static float cubeX = 0.0;
// used to move the cube in y dir
static float cubeY = 0.0;
// used to move the skybox in x dir
static float skyboxX = 0.0;
// used to move the skybox in y dir
static float skyboxY = 0.0;
// used to move the camera in x dir
static float cameraX = 0.0;
// used to move the camara in y dir
static float cameraY = 0.0;
// controls the zoom in or out of entire scene
static float zoomScene = 0.0;


// TextureLoad uses SOIL to load textures
void TextureLoad(char* filename, GLuint &tex){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height;  // width & height for the image reader
    unsigned char* image;

    image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
    // making sure the image loads, or else log the error
    if(image == NULL){
        cout << SOIL_last_result() << endl;
        exit(EXIT_FAILURE);
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    // binding image data
    SOIL_free_image_data(image);    // Able to discard image since we are done with it, saves memory

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


/* GLUT callback Handlers */
static void resize(int width, int height){
    //double Ratio;

    if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,1,0.1f, 100.0f);
}


// renderCube will render the cube by drawing GL_QUADS
void renderCube(){
    // first disable lighting
    glDisable(GL_LIGHTING);

    // back of the cube
    glBindTexture(GL_TEXTURE_2D, tnt_side_texture_1);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();

    // front of the cube
    glBindTexture(GL_TEXTURE_2D, tnt_side_texture_2);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glEnd();
    
    // left of the cube
    glBindTexture(GL_TEXTURE_2D, tnt_side_texture_3);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    // right of the cube
    glBindTexture(GL_TEXTURE_2D, tnt_side_texture_4);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glEnd();
    
    // bottom of the cube
    glBindTexture(GL_TEXTURE_2D, tnt_bottom_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -1.0f, -1.0f);
    glEnd();

    // top of the cube
    glBindTexture(GL_TEXTURE_2D, tnt_top_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
     
    // enable lighting
    glEnable(GL_LIGHTING);
}


// renderSkybox will render the skybox by drawing GL_QUADS
void renderSkybox(){
    // first disable lighting
    glDisable(GL_LIGHTING);

    // back of the cube
    glBindTexture(GL_TEXTURE_2D, back_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();

    // front of the cube
    glBindTexture(GL_TEXTURE_2D, front_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glEnd();
    
    // bottom of the cube
    glBindTexture(GL_TEXTURE_2D, bottom_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.0f, -1.0f, -1.0f);
    glEnd();

    // top of the cube
    glBindTexture(GL_TEXTURE_2D, top_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
    
    // left of the cube
    glBindTexture(GL_TEXTURE_2D, left_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    // right of the cube
    glBindTexture(GL_TEXTURE_2D, right_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glEnd();
     
    // enable lighting
    glEnable(GL_LIGHTING);
}


static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // glTranslated used to move the camera
    glTranslated(cameraX, cameraY, 0.0);
    // initial viewpoint
    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame){
        //Draw Our Mesh In Wireframe Mesh
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else{
        //Toggle WIRE FRAME
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
       
    // my code starts here
    glPushMatrix();
        
        // zoom in or out of entire scene
        glTranslatef(0.0, 0.0, zoomScene);
    
        // manipulting matrix for the cube
        glPushMatrix();
            // rotating cube on x-axis
            glRotatef(cubeX, 1.0, 0.0, 0.0);
            // rotating cube on y-axis
            glRotatef(cubeY, 0.0, 1.0, 0.0);
            // rendering cube
            renderCube();
        glPopMatrix();
        
        
        // manipulating matrix for the skybox
        glPushMatrix();
            // rotating skybox on x-axis
            glRotatef(skyboxX, 1.0, 0.0, 0.0);
            // rotating skybox on y-axis
            glRotatef(skyboxY, 0.0, 1.0, 0.0);
            // scaling skybox 15x
            glScaled(15.0, 15.0, 15.0);
            // render skybox
            renderSkybox();
        glPopMatrix();
    
    glPopMatrix();
    // my code ends here
    
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y){
    switch (key){
        // a key rotates cube clockwise on y-axis
        case 'a':
            cubeY -= 3 % 360;
            break;
        // w key rotates cube upwards on x-axis
        case 'w':
            cubeX -= 3 % 360;
            break;
        // s key rotates cube downwards on x-axis
        case 's':
            cubeX += 3 % 360;
            break;
        // d key rotates cube counter-clockwise on y-axis
        case 'd':
            cubeY += 3 % 360;
            break;
        // j key moves camera to the left
        case 'j':
            cameraX += 3;
            break;
        // k key moves camera downwards
        case 'k':
            cameraY += 3;
            break;
        // i key moves camera upwards
        case 'i':
            cameraY -= 3;
            break;
        // l key moves camera to the right
        case 'l':
            cameraX -= 3;
            break;
        // o key resets the camera
        case 'o':
            cameraY = 0;
            cameraX = 0;
            break;
        // z key zooms in the entire scene
        case 'z':
            zoomScene += 1.0;
            break;
        // x key zooms in the entire scene
        case 'x':
            zoomScene -= 1.0;
            break;
        // space key will reset everything to intial position
        case ' ':
            cubeX = 0.0;
            cubeY = 0.0;
            skyboxX = 0.0;
            skyboxY = 0.0;
            cameraX = 0.0;
            cameraY = 0.0;
            zoomScene = 0.0;
            break;
        // quit cases
        case 27 :
        case 'q':
            exit(0);
            break;
    }
}
void Specialkeys(int key, int x, int y){
    switch(key){
        // if left key is pressed rotate entire skybox clockwise
        case GLUT_KEY_LEFT:
            skyboxY -= 3 % 360;
            break;
        // if right key is pressed rotate entire skybox counter-clockwise
        case GLUT_KEY_RIGHT:
            skyboxY += 3 % 360;
            break;
        // if up key is pressed rotate entire skybox downwards
        case GLUT_KEY_UP:
            skyboxX -= 3 % 360;
            break;
        // if down key is pressed rotate entire skybox upwards
        case GLUT_KEY_DOWN:
            skyboxX += 3 % 360;
            break;
    }
    glutPostRedisplay();
}


static void idle(void){
    glutPostRedisplay();
}


static void init(void){
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

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    
    // loading cube and skybox image textures
    glGenTextures(1, &front_texture);
    char p1[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/front.jpg";
    TextureLoad(p1, front_texture);
    
    glGenTextures(1, &back_texture);
    char p2[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/back.jpg";
    TextureLoad(p2, back_texture);
    
    glGenTextures(1, &top_texture);
    char p3[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/top.jpg";
    TextureLoad(p3, top_texture);
    
    glGenTextures(1, &bottom_texture);
    char p4[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/bottom.jpg";
    TextureLoad(p4, bottom_texture);
    
    glGenTextures(1, &left_texture);
    char p5[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/left.jpg";
    TextureLoad(p5, left_texture);
    
    glGenTextures(1, &right_texture);
    char p6[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/right.jpg";
    TextureLoad(p6, right_texture);
    
    glGenTextures(1, &tnt_side_texture_1);
    char p7[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/tnt_side_1.jpg";
    TextureLoad(p7, tnt_side_texture_1);
    
    glGenTextures(1, &tnt_side_texture_2);
    char p8[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/tnt_side_2.jpg";
    TextureLoad(p8, tnt_side_texture_2);
    
    glGenTextures(1, &tnt_side_texture_3);
    char p9[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/tnt_side_3.jpg";
    TextureLoad(p9, tnt_side_texture_3);
    
    glGenTextures(1, &tnt_side_texture_4);
    char p10[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/tnt_side_4.jpg";
    TextureLoad(p10, tnt_side_texture_4);
    
    glGenTextures(1, &tnt_top_texture);
    char p11[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/tnt_top.jpg";
    TextureLoad(p11, tnt_top_texture);
    
    glGenTextures(1, &tnt_bottom_texture);
    char p12[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/Skybox/Skybox/tnt_bottom.jpg";
    TextureLoad(p12, tnt_bottom_texture);
}


/* Program entry point */
int main(int argc, char *argv[]){
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Skybox Project");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
