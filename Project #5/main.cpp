//
//  main.cpp
//  StanfordBunny
//
//  Created by Luis Valencia on 12/5/22.
//
// NOTE: THE IMAGE FILEPATHS IN MY CODE ARE DIRECT TO MY SYSTEM, YOU WILL HAVE TO CHANGE THEM TO
// HOWEVER THE IMAGES ON YOUR SYSTEM ARE FOR THE IMAGES TO BE USED
// READ ME:
// USE DOWN UP LEFT AND RIGHT ARROW KEYS TO FLY IN THE SKYBOX (x and z direction)
// USE MOUSE DRAG AND DROP TO ROTATE AROUND THE SKYBOX FOR A BETTER 360 VIEW OF THE BUNNY OBJ
// USE A W S D KEYS TO ROTATE THE SKYBOX ONLY
// PRESS P TO TOGGLE WIREFRAME
// PRESS SPACE BAR TO RESET
// Q OR ESC KEY TO QUIT
#define GL_SILENCE_DEPRECATION // used to silence OpenGL deprecation warnings for MacOS
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
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#define PI 3.14159
using namespace std;

/* STRUCTS */
// to hold the vertices of a point
struct Vertex {
    GLfloat x, y, z;
};

// to hold to normals of a vertex
struct NormalVertex {
    GLfloat x, y, z;
};

// to hold the texture coords for texture mapping
struct TextureCoords {
    GLfloat x, y;
};

// to hold the x/x/x y/y/y z/z/z
struct Faces {
    GLfloat vx, vy, vz, vnx, vny, vnz;
    GLfloat x, y, z;
};

/* GLOBALS */
bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

float xpos = 0;
float ypos = 0;
float Wwidth,Wheight;

// GLuint textures for the skybox
GLuint front_texture;
GLuint back_texture;
GLuint top_texture;
GLuint bottom_texture;
GLuint left_texture;
GLuint right_texture;
// GLuint textures for the bunny
GLuint bunny_texture;

// globals for the bunny obj
vector<Vertex> bunny_vertices;
vector<NormalVertex> bunny_vertex_normals;
vector<Faces> bunny_faces;
vector<TextureCoords> bunny_text_coords;
int bunny_reduce = 100;

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

// variables used to control mouse drag movements
int btnMode = 1;
float X = 0.0, Y = 0.0;
float mx = 0.0, my = 0.0;
float transX = 0.0, transY = 0.0;
float mTransX = 0.0, mTransY = 0.0;
float oldXPos = 0.0, oldYPos = 0.0;

// spin for the light / model rotation
GLfloat spin1 = 0.0;
GLfloat spin2 = 0.0;


// resizing the gui
static void resize(int width, int height)
{
    double Ratio;

    Wwidth = (float)width;
    Wheight = (float)height;

    Ratio = (double)width /(double)height;

   glViewport(0,0,(GLsizei) width,(GLsizei) height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   gluPerspective (45.0f,Ratio,0.1f, 100.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


// TextureLoad uses SOIL to load textures
void TextureLoad(char* filename, GLuint &tex)
{
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


// renderSkybox will render the skybox by drawing GL_QUADS
void renderSkybox()
{
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


// used to read a .obj model in order to then display it
void loadObj(string filepath, int reduce, vector<Vertex> &vertices, vector<NormalVertex> &vertex_normals, vector<Faces> &faces, vector<TextureCoords> &textCoords)
{
    // clearing vectors
    vertices.clear();
    vertex_normals.clear();
    faces.clear();
    textCoords.clear();
    
    // local variables
    ifstream file;
    string line;
    GLfloat vx, vy, vz;
    GLfloat vnx, vny, vnz;
    GLfloat tx, ty;
    GLfloat fvx, fvy, fvz, fvnx, fvny, fvnz, fx, fy, fz;
    
    file.open(filepath);
    if(file.is_open() == true){
        
        while(getline(file, line)){ // read the entire line
            
            // skip empty lines
            if(line.substr(0, 2) == "") continue;
                
            // dealing with vertex lines
            else if(line.substr(0, 2) == "v "){
                istringstream nums(line.substr(2));
                nums >> vx >> vy >> vz;
                vertices.push_back({vx/reduce, vy/reduce, vz/reduce});
            }
            
            // dealing with vertex normal lines
            else if(line.substr(0, 2) == "vn"){
                istringstream nums(line.substr(2));
                nums >> vnx >> vny >> vnz;
                vertex_normals.push_back({vnx, vny, vnz});
            }
            
            else if(line.substr(0, 2) == "vt"){
                istringstream nums(line.substr(2));
                nums >> tx >> ty;
                textCoords.push_back({tx, ty});
            }
            
            // dealing with face lines
            else if(line.substr(0, 2) == "f "){
                // trimming line to remove first 2 characters
                line = line.substr(2, line.length());
                // loop through line
                for(string::iterator it = line.begin(); it != line.end(); ++it){
                    // if character is a slash
                    if(*it == '/'){
                        // erase the slash
                        line.erase(it);
                        // insert a space between the number
                        line.insert(it, ' ');
                    }
                }
                istringstream nums(line);
                nums >> fvx >> fvy >> fvz >> fvnx >> fvny >> fvnz >> fx >> fy >> fz;
                faces.push_back({fvx, fvy, fvz, fvnx, fvny, fvnz, fx, fy, fz});
            }
            
            // anything else just skip
            else{
                continue;
            }
            
        }
        
    }

    file.close();
}


// used to calculate vertex normals for the bunny obj
void calculateVertexNormals(vector<Faces> faces, vector<Vertex> vertices, vector<NormalVertex> &vertex_normals){
    
    // surface_normals will store the calculated surface normals
    vector<NormalVertex> surface_normals;
    
    // vertex_indexes will store i indexes for later use
    vector<vector<int>> vertex_indexes(vertices.size());
    
    // loop through bunny face points
    for(int i = 0; i < faces.size(); i++){
        
        // p1 p2 p3 represents the 3 points to make a triangle
        // they can also be thought of as individual vectors
        // each point will store x, y, z value
        GLfloat p1[3], p2[3], p3[3];
        
        // vector U will contain the difference of the vectors p2 - p1
        // which means vector U will also store x, y, z value
        // vector V will contain the difference of the vectors p1 - p3
        // which means vector V will also store x, y, z value
        GLfloat U[3], V[3];
        
        // proceed to calculate the surface normals
        
        // extract data from the current face for vectors p1, p2, p3
        p1[0] = vertices[(faces[i].vx) - 1].x;
        p1[1] = vertices[(faces[i].vx) - 1].y;
        p1[2] = vertices[(faces[i].vx) - 1].z;
        p2[0] = vertices[(faces[i].vnx) - 1].x;
        p2[1] = vertices[(faces[i].vnx) - 1].y;
        p2[2] = vertices[(faces[i].vnx) - 1].z;
        p3[0] = vertices[(faces[i].x) - 1].x;
        p3[1] = vertices[(faces[i].x) - 1].y;
        p3[2] = vertices[(faces[i].x) - 1].z;
        
        // calculate vector differences for vectors U & V
        // U = p2 - p1
        // U = vertices[1] - vertices[0]
        U[0] = p2[0] - p1[0];
        U[1] = p2[1] - p1[1];
        U[2] = p2[2] - p1[2];
        // V = p1 - p3
        // V = vertices[0] - vertices[2]
        V[0] = p1[0] - p3[0];
        V[1] = p1[1] - p3[1];
        V[2] = p1[2] - p3[2];
        
        // calculate cross product of vectors U & V to get the normal of each point
        // normal of the x, normal of the y, normal of the z
        // Nx = U.y * V.z - U.z * V.y
        GLfloat Nx = (U[1] * V[2]) - (U[2] * V[1]);
        // Ny = U.z * V.x - U.x * V.z
        GLfloat Ny = (U[2] * V[0]) - (U[0] * V[2]);
        // Nz = U.x * V.y - U.y * V.x
        GLfloat Nz = (U[0] * V[1]) - (U[1] * V[0]);
        
        // calculate length of the normal vector
        GLfloat vector_length = sqrt((Nx * Nx) + (Ny * Ny) + (Nz * Nz));
        
        // normalize the normals of each point
        GLfloat Nnx = (Nx / vector_length);
        GLfloat Nny = (Ny / vector_length);
        GLfloat Nnz = (Nz / vector_length);

        // save normalized surface normals
        surface_normals.push_back({Nnx, Nny, Nnz});
        
        // save the indexes
        vertex_indexes[(faces[i].vx) - 1].push_back(i);
        vertex_indexes[(faces[i].vnx) - 1].push_back(i);
        vertex_indexes[(faces[i].x) - 1].push_back(i);
    }
    
    // now compute vertex normals
    for(int i = 0; i < vertices.size(); i++){
        
        // getting indexes from vertex_indexes
        vector<int> indexes = vertex_indexes[i];
        
        // new_surface_normals will contain previously saved surface normals at given indexes
        vector<NormalVertex> new_surface_normals;
        
        // summation of surface normals at given x y z
        GLfloat xSum = 0.0, ySum = 0.0, zSum = 0.0;
        
        // loop through indexes to collect data for new_surface_normals
        for(int j = 0; j < indexes.size(); j++){
            new_surface_normals.push_back(surface_normals[indexes[j]]);
        }
        
        // sum values of the surface normals
        for(int j = 0; j < new_surface_normals.size(); j++){
            xSum += new_surface_normals[j].x;
            ySum += new_surface_normals[j].y;
            zSum += new_surface_normals[j].z;
        }
        
        // calculate the vector length of the vertex normals
        GLfloat vector_length_2 = sqrt((xSum * xSum) + (ySum * ySum) + (zSum * zSum));
        
        // normalize the normals
        GLfloat xNorm = (xSum / vector_length_2);
        GLfloat yNorm = (ySum / vector_length_2);
        GLfloat zNorm = (zSum / vector_length_2);
        
        // push official vertex normals after being normalized
        vertex_normals.push_back({xNorm, yNorm, zNorm});
    }
}


// display function
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // glTranslated used to move the camera left and right direction
    glTranslated(cameraX, cameraY, 0.0);
    // initial viewpoint
    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw Our Mesh In Wireframe Mesh
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Toggle WIRE FRAME
        
    // main push matrix
    glPushMatrix();
        
        // zoom in or out of entire scene
        glTranslatef(0.0, 0.0, zoomScene);
    
        // used to translate around the skybox using mouse
        glTranslatef(mTransX + oldXPos, -mTransY - oldYPos, 0);
        // used to rotate around the skybox using mouse
        glRotatef(my, 1, 0, 0);
        glRotatef(mx, 0, 1, 0);
    
        // manipulating matrix for bunny obj
        glPushMatrix();
    
            // light rotation around the bunny model
            glPushMatrix();
                GLfloat position[] = { 0.0, 0.0, 3.5, 1.0 };
                glRotatef(spin1, 0.0, 1.0, 0.0);
                glLightfv(GL_LIGHT0, GL_POSITION, position);
            glPopMatrix();
    
            // bind the texture
            glBindTexture(GL_TEXTURE_2D, bunny_texture);
    
            // place bunny in front of us
            glTranslatef(0.0, 0.0, 0.0);
    
            // rotate bunny
            glRotatef(spin2, 0.0, 1.0, 0.0);
    
            // drawing bunny with texture
            glBegin(GL_TRIANGLES);
            for(int i = 0; i < bunny_faces.size(); i++){
                 glNormal3f(bunny_vertex_normals[(bunny_faces[i].vz)-1].x, bunny_vertex_normals[(bunny_faces[i].vz)-1].y, bunny_vertex_normals[(bunny_faces[i].vz)-1].z);
                 glTexCoord2f(bunny_text_coords[(bunny_faces[i].vy)-1].x, bunny_text_coords[(bunny_faces[i].vy)-1].y);
                 glVertex3f(bunny_vertices[(bunny_faces[i].vx)-1].x, bunny_vertices[(bunny_faces[i].vx)-1].y, bunny_vertices[(bunny_faces[i].vx)-1].z);

                 glNormal3f(bunny_vertex_normals[(bunny_faces[i].vnz)-1].x, bunny_vertex_normals[(bunny_faces[i].vnz)-1].y, bunny_vertex_normals[(bunny_faces[i].vnz)-1].z);
                 glTexCoord2f(bunny_text_coords[(bunny_faces[i].vny)-1].x, bunny_text_coords[(bunny_faces[i].vny)-1].y);
                 glVertex3f(bunny_vertices[(bunny_faces[i].vnx)-1].x, bunny_vertices[(bunny_faces[i].vnx)-1].y, bunny_vertices[(bunny_faces[i].vnx)-1].z);

                 glNormal3f(bunny_vertex_normals[(bunny_faces[i].z)-1].x, bunny_vertex_normals[(bunny_faces[i].z)-1].y, bunny_vertex_normals[(bunny_faces[i].z)-1].z);
                 glTexCoord2f(bunny_text_coords[(bunny_faces[i].y)-1].x, bunny_text_coords[(bunny_faces[i].y)-1].y);
                 glVertex3f(bunny_vertices[(bunny_faces[i].x)-1].x, bunny_vertices[(bunny_faces[i].x)-1].y, bunny_vertices[(bunny_faces[i].x)-1].z);
            }
            glEnd();
    
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
    
    glutSwapBuffers();
}


// key functions start here
static void key(unsigned char key, int x, int y)
{
    switch (key){
        // if a (left) key is pressed rotate entire skybox clockwise
        case 'a':
            skyboxY -= 3 % 360;
            break;
        // if w (up) key is pressed rotate entire skybox downwards
        case 'w':
            skyboxX -= 3 % 360;
            break;
        // if s (down) key is pressed rotate entire skybox downwards
        case 's':
            skyboxX += 3 % 360;
            break;
        // if d (right) key is pressed rotate entire skybox counter-clockwise
        case 'd':
            skyboxY += 3 % 360;
            break;
        // space key will reset everything to intial position
        case ' ':
            skyboxX = 0.0;
            skyboxY = 0.0;
            cameraX = 0.0;
            zoomScene = 0.0;
            btnMode = 1;
            X = 0.0; Y = 0.0;
            mx = 0.0; my = 0.0;
            transX = 0.0; transY = 0.0;
            mTransX = 0.0; mTransY = 0.0;
            oldXPos = 0.0; oldYPos = 0.0;
            break;
        case 'p':
            WireFrame =! WireFrame;
            break;
        // quit cases
        case 27 :
        case 'q':
            exit(0);
            break;
    }
}
void Specialkeys(int key, int x, int y)
{
    switch(key){
        // if left key is pressed camera moves to the left
        case GLUT_KEY_LEFT:
            cameraX += 1.5;
            break;
        // if right key is pressed camera moves to the right
        case GLUT_KEY_RIGHT:
            cameraX -= 1.5;
            break;
        // if up key is pressed camera moves forward
        case GLUT_KEY_UP:
            zoomScene += 1.0;
            break;
        // if down key is pressed camera moves backwards
        case GLUT_KEY_DOWN:
            zoomScene -= 1.0;
            break;
    }
    glutPostRedisplay();
}


// idle used for bunny and light rotation
static void idle(void)
{
    // for light
    spin1 += 2;
    // for bunny model rotation
    spin2 += 0.25;
    glutPostRedisplay();
}


// mouse function used to help rotate the skybox
void mouse(int btn, int state, int x, int y)
{
    //float scale = 100*(Wwidth/Wheight);
    switch(btn){
        case GLUT_LEFT_BUTTON:
            X = x;
            Y = y;
            btnMode = 1;
            break;
        case GLUT_RIGHT_BUTTON:
            transX = x;
            transY = y;
            btnMode = 0;
            break;
    }
     glutPostRedisplay();
}
// mouseMove function in pair with mouse function
void mouseMove(int x, int y) {
    // btnMode is either 0 or 1
    switch(btnMode){
        case 0:
            mTransX += (x - transX) / 20;
            mTransY += (y - transY) / 20;
            transX = x;
            transY = y;
            break;
        case 1:
            mx += (x - X);
            my += (y - Y);
            X = x;
            Y = y;
            break;
    }
    glutPostRedisplay();
}


static void init(void)
{
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                 // assign a color you like
    
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

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    
    // loading skybox image textures
    glGenTextures(1, &front_texture);
    char p1[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/StanfordBunny/StanfordBunny/front.jpg";
    TextureLoad(p1, front_texture);
    
    glGenTextures(1, &back_texture);
    char p2[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/StanfordBunny/StanfordBunny/back.jpg";
    TextureLoad(p2, back_texture);
    
    glGenTextures(1, &top_texture);
    char p3[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/StanfordBunny/StanfordBunny/top.jpg";
    TextureLoad(p3, top_texture);
    
    glGenTextures(1, &bottom_texture);
    char p4[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/StanfordBunny/StanfordBunny/bottom.jpg";
    TextureLoad(p4, bottom_texture);
    
    glGenTextures(1, &left_texture);
    char p5[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/StanfordBunny/StanfordBunny/left.jpg";
    TextureLoad(p5, left_texture);
    
    glGenTextures(1, &right_texture);
    char p6[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/StanfordBunny/StanfordBunny/right.jpg";
    TextureLoad(p6, right_texture);
    
    // loading bunny image texture
    glGenTextures(1, &bunny_texture);
    char p7[] = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/StanfordBunny/StanfordBunny/bunny_texture.jpg";
    TextureLoad(p7, bunny_texture);
    
    // loading bunny obj model
    string filepath = "/Users/luisvalencia/Desktop/School/Senior/CSCI 172/OpenGL Projects/StanfordBunny/StanfordBunny/bunny-3.obj";
    loadObj(filepath, bunny_reduce, bunny_vertices, bunny_vertex_normals, bunny_faces, bunny_text_coords);
    
    // calculating bunny vertex normals from what was given in the initial bunny obj model
    calculateVertexNormals(bunny_faces, bunny_vertices, bunny_vertex_normals);
    
    /* OUTPUTTING OBJ DATA TO CONSOLE FOR VERIFICATION */
    // output for bunny
    cout << "Bunny Data:" << endl << endl;
    cout << "Vertices:" << endl;
    for(int i = 0; i < bunny_vertices.size(); i++){
        cout << "x:" << bunny_vertices[i].x << " y:" << bunny_vertices[i].y << " z:" << bunny_vertices[i].z << endl;
    }
    cout << endl;
    cout << "Vertex Normals:" << endl;
    for(int i = 0; i < bunny_vertex_normals.size(); i++){
        cout << "x:" << bunny_vertex_normals[i].x << " y:" << bunny_vertex_normals[i].y << " z:" << bunny_vertex_normals[i].z << endl;
    }
    cout << endl;
    cout << "Faces:" << endl;
    for(int i = 0; i < bunny_faces.size(); i++){
        cout << "vx:" << bunny_faces[i].vx << " vy:" << bunny_faces[i].vy << " vz:" << bunny_faces[i].vz << "   vnx:" << bunny_faces[i].vnx << " vny:" << bunny_faces[i].vny << " vnz:" << bunny_faces[i].vnz << "   x:" << bunny_faces[i].x << " y:" << bunny_faces[i].y << " z:" << bunny_faces[i].z << endl;
    }
    cout << endl;
    cout << "Texture Coords:" << endl;
    for(int i = 0; i < bunny_text_coords.size(); i++){
        cout << "tx:" << bunny_text_coords[i].x << " ty:" << bunny_text_coords[i].y << endl;
    }
    cout << endl;
}


/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Stanford Bunny Project");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);
    glutIdleFunc(idle);
    glutMainLoop();
    return EXIT_SUCCESS;
}
