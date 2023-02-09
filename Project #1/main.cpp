//
//  main.cpp
//  Project #1
//
//  Created by Luis Valencia on 9/2/22.
//

// Using OpenGL + GLUT Frameworks on MacOS, works 100%, but needs this line
// of code to get rid of OpenGL deprecation warnings
#define GL_SILENCE_DEPRECATION

#include <string.h>
#include <GLUT/glut.h>
#include <stdlib.h>

// global variables init
float a, b, c, d, e, f;
float x, y, x0, y0;
int it;


/* GLUT callback Handlers */

// used to resize the GUI
void resize(int width, int height)
{
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

// initialization
void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport

    // assign 1 to x0 and y0
    x0 = 1;
    y0 = 1;
}


void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

    // loop 200,000 times
    for(int i = 0; i < 200000; i++){
        
        // gives us random number 1-100
        it = (rand() % 100) + 1;
        
        if(it <= 7){
            // 7% chance of choosing f4 transformation
            a = -0.15;
            b = 0.26;
            c = 0.28;
            d = 0.24;
            e = 0;
            f = 0.44;
        }
        else if(it <= 14){
            // 7% chance of choosing f3 transformation
            a = 0.2;
            b = 0.23;
            c = -0.26;
            d = 0.22;
            e = 0;
            f = 1.6;
        }
        else if(it <= 99){
            // 85% chance of choosing f2 transformation
            a = 0.85;
            b = -0.04;
            c = 0.04;
            d = 0.85;
            e = 0;
            f = 1.6;
        }
        else{
            // 1% chance of choosing f1 transformation
            a = 0;
            b = 0;
            c = 0;
            d = 0.16;
            e = 0;
            f = 0;
        }
        
        // set up x and y values (affine transformations)
        x = a * x0 + c * y0 + e;
        y = b * x0 + d * y0 + f;
        
        // plot the points and add color
        glColor3f(0, 0.6, 0);   // shade of green
        glBegin(GL_POINTS);
        glVertex3f(x, y, 0);
        glEnd();
        
        // set new starting x0 and y0 values
        x0 = x;
        y0 = y;
    }
    
    glFlush (); // clear buffer
}


void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (600, 800);                //window screen
    glutInitWindowPosition (100, 100);            //window position
    glutCreateWindow ("Program1");                //program title
    init();
    glutDisplayFunc(display);                     //callback function for display
    glutReshapeFunc(resize);                      //callback for reshape
    glutKeyboardFunc(key);                        //callback function for keyboard
    glutMainLoop();                               //loop

    return EXIT_SUCCESS;
}
