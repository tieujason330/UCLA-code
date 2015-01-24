// Assignment 0:     Source code for Make a Cube in OpenGL
// Description: Creates an OpenGL window and draws a 3D cube
//              That the user can rotate using the arrow keys
//
// Controls:    Left Arrow  - Rotate Left
//              Right Arrow - Rotate Right
//              Up Arrow    - Rotate Up
//              Down Arrow  - Rotate Down
//              a - Spin Animation

// ----------------------------------------------------------
// Includes
// ----------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void specialKeys();
void drawCube(double size);
void idle();

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_x=0;
double rotate_y=0;
double rotate_z=0;
int animation;

// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display(){
    
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    
    // Rotate when user changes rotate_x and rotate_y
    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );
    glRotatef( rotate_z, 0.0, 0.0, 1.0 );

    drawCube(0.5);
    
    glFlush();
    glutSwapBuffers();
    
}

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
    
    //  Right arrow - increase rotation by 5 degree
    if (key == GLUT_KEY_RIGHT)
        rotate_y += 5;
    
    //  Left arrow - decrease rotation by 5 degree
    else if (key == GLUT_KEY_LEFT)
        rotate_y -= 5;
    
    else if (key == GLUT_KEY_UP)
        rotate_x += 5;
    
    else if (key == GLUT_KEY_DOWN)
        rotate_x -= 5;
    else if (key == 'a')
        animation = 1 - animation;
    
    //  Request display update
    glutPostRedisplay();
    
}


//implement your cube here
void drawCube(double size){
  
}

void idle(){
    if (animation) {
        rotate_x += 1;
        rotate_y += 1;
        rotate_z += 1;
    }
    glutPostRedisplay();
}



// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){
    
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);
    
    //  Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Create window
    glutCreateWindow("Assignment 0");
    
    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);
    
    // Callback functions
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);
    
    //  Pass control to GLUT for events
    glutMainLoop();
    
    //  Return to OS
    return 0;
    
}