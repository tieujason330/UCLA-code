////////////////////////////////////////////////////
// anim.cpp version 4.1
// Template code for drawing an articulated figure.
// CS 174A 
////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GLUT/glut.h>
#endif

#include "Ball.h"
#include "FrameSaver.h"
#include "Timer.h"
#include "Shapes.h"
#include "tga.h"

#include "Angel/Angel.h"

#ifdef __APPLE__
#define glutInitContextVersion(a,b)
#define glutInitContextProfile(a)
#define glewExperimental int glewExperimentalAPPLE
#define glewInit()
#endif

//ADDED FOR RANDOM NUM GENERATOR
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
//using namespace std;
/////////////////////////////////////////////////////
// These are global variables
//
//
// Add additional ones if you need,
// especially for animation
//////////////////////////////////////////////////////

FrameSaver FrSaver ;
Timer TM ;

BallData *Arcball = NULL ;
int Width = 800;
int Height = 800 ;
int Button = -1 ;
float Zoom = 1 ;
int PrevY = 0 ;

int Animate = 0 ;
int Recording = 0 ;

void resetArcball() ;
void save_image();
void instructions();
void set_colour(float r, float g, float b) ;

const int STRLEN = 100;
typedef char STR[STRLEN];

#define PI 3.1415926535897
#define X 0
#define Y 1
#define Z 2

//texture
GLuint texture_cube;
GLuint texture_earth;

// Structs that hold the Vertex Array Object index and number of vertices of each shape.
ShapeData cubeData;
ShapeData sphereData;
ShapeData coneData;
ShapeData cylData;

// Matrix stack that can be used to push and pop the modelview matrix.
class MatrixStack {
    int    _index;
    int    _size;
    mat4*  _matrices;

   public:
    MatrixStack( int numMatrices = 32 ):_index(0), _size(numMatrices)
        { _matrices = new mat4[numMatrices]; }

    ~MatrixStack()
	{ delete[]_matrices; }

    void push( const mat4& m ) {
        assert( _index + 1 < _size );
        _matrices[_index++] = m;
    }

    mat4& pop( void ) {
        assert( _index - 1 >= 0 );
        _index--;
        return _matrices[_index];
    }
};

MatrixStack  mvstack;
mat4         model_view;
GLint        uModelView, uProjection, uView;
GLint        uAmbient, uDiffuse, uSpecular, uLightPos, uShininess;
GLint        uTex, uEnableTex;

// The eye point and look-at point.
// Currently unused. Use to control a camera with LookAt().
Angel::vec4 eye(0, 0.0, 50.0,1.0);
Angel::vec4 ref(0.0, 0.0, 0.0,1.0);
Angel::vec4 up(0.0,1.0,0.0,0.0);

double TIME = 0.0 ;

//store the random x and z locations of a tree
//vector<float> tree_xLoc[10];
GLfloat tree_xLoc[80];
GLfloat tree_zLoc[80];

//for first display call set the locations for trees
bool firstDisplayCall = true;

//global locations to help correct movement
//GLfloat arrow_x = 0.0f;
GLfloat bird_x = 0.0f, bird_y = 0.0f, bird_z = 0.0f, bird_turn = 0.0f, bird_turn_rot;
GLfloat Monster_x = 0.0f, Monster_y = 0.0f, Monster_z = 0.0f;

/////////////////////////////////////////////////////
//    PROC: drawCylinder()
//    DOES: this function 
//          render a solid cylinder  oriented along the Z axis. Both bases are of radius 1. 
//          The bases of the cylinder are placed at Z = 0, and at Z = 1.
//
//          
// Don't change.
//////////////////////////////////////////////////////
void drawCylinder(void)
{
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cylData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cylData.numVertices );
}


//////////////////////////////////////////////////////
//    PROC: drawCone()
//    DOES: this function 
//          render a solid cone oriented along the Z axis with base radius 1. 
//          The base of the cone is placed at Z = 0, and the top at Z = 1. 
//         
// Don't change.
//////////////////////////////////////////////////////
void drawCone(void)
{
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( coneData.vao );
    glDrawArrays( GL_TRIANGLES, 0, coneData.numVertices );
}


//////////////////////////////////////////////////////
//    PROC: drawCube()
//    DOES: this function draws a cube with dimensions 1,1,1
//          centered around the origin.
// 
// Don't change.
//////////////////////////////////////////////////////
void drawCube(void)
{
    glBindTexture( GL_TEXTURE_2D, texture_cube );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
}


//////////////////////////////////////////////////////
//    PROC: drawSphere()
//    DOES: this function draws a sphere with radius 1
//          centered around the origin.
// 
// Don't change.
//////////////////////////////////////////////////////
void drawSphere(void)
{
    glBindTexture( GL_TEXTURE_2D, texture_earth);
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( sphereData.vao );
    glDrawArrays( GL_TRIANGLES, 0, sphereData.numVertices );
}


void resetArcball()
{
    Ball_Init(Arcball);
    Ball_Place(Arcball,qOne,0.75);
}


/*********************************************************
 PROC: set_colour();
 DOES: sets all material properties to the given colour
 -- don't change
 **********************************************************/

void set_colour(float r, float g, float b)
{
    float ambient  = 0.2f;
    float diffuse  = 0.6f;
    float specular = 0.2f;
    glUniform4f(uAmbient,  ambient*r,  ambient*g,  ambient*b,  1.0f);
    glUniform4f(uDiffuse,  diffuse*r,  diffuse*g,  diffuse*b,  1.0f);
    glUniform4f(uSpecular, specular*r, specular*g, specular*b, 1.0f);
}


/*********************************************************
 PROC: instructions()
 DOES: display instruction in the console window.
 -- No need to change

 **********************************************************/
void instructions()
{
    printf("Press:\n");
    printf("  s to save the image\n");
    printf("  r to restore the original view.\n") ;
    printf("  0 to set it to the zero state.\n") ;
    printf("  a to toggle the animation.\n") ;
    printf("  m to toggle frame dumping.\n") ;
    printf("  q to quit.\n");
}


/*********************************************************
 PROC: myinit()
 DOES: performs most of the OpenGL intialization
 -- change these with care, if you must.
 
 **********************************************************/
void myinit(void)
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram(program);
    
    // Generate vertex arrays for geometric shapes
    generateCube(program, &cubeData);
    generateSphere(program, &sphereData);
    generateCone(program, &coneData);
    generateCylinder(program, &cylData);
    
    uModelView  = glGetUniformLocation( program, "ModelView"  );
    uProjection = glGetUniformLocation( program, "Projection" );
    uView       = glGetUniformLocation( program, "View"       );
    
    glClearColor( 0.1, 0.1, 0.2, 1.0 ); // dark blue background
    
    uAmbient   = glGetUniformLocation( program, "AmbientProduct"  );
    uDiffuse   = glGetUniformLocation( program, "DiffuseProduct"  );
    uSpecular  = glGetUniformLocation( program, "SpecularProduct" );
    uLightPos  = glGetUniformLocation( program, "LightPosition"   );
    uShininess = glGetUniformLocation( program, "Shininess"       );
    uTex       = glGetUniformLocation( program, "Tex"             );
    uEnableTex = glGetUniformLocation( program, "EnableTex"       );
    
    glUniform4f(uAmbient,    0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uDiffuse,    0.6f,  0.6f,  0.6f, 1.0f);
    glUniform4f(uSpecular,   0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uLightPos,  15.0f, 15.0f, 30.0f, 0.0f);
    glUniform1f(uShininess, 100.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    Arcball = new BallData;
    Ball_Init(Arcball);
    Ball_Place(Arcball,qOne,0.75);
}


//////////////////////////////////////////////////////
//    PROC: myKey()
//    DOES: this function gets caled for any keypresses
// 
//////////////////////////////////////////////////////
void myKey(unsigned char key, int x, int y)
{
    float time ;
    switch (key) {
        case 'q':
        case 27:
            exit(0); 
        case 's':
            FrSaver.DumpPPM(Width,Height) ;
            break;
        case 'r':
            resetArcball() ;
            glutPostRedisplay() ;
            break ;
        case 'a': // togle animation
            Animate = 1 - Animate ;
            // reset the timer to point to the current time		
            time = TM.GetElapsedTime() ;
            TM.Reset() ;
            // printf("Elapsed time %f\n", time) ;
            break ;
        case '0':
            //reset your object
            break ;
        case 'm':
            if( Recording == 1 )
            {
                printf("Frame recording disabled.\n") ;
                Recording = 0 ;
            }
            else
            {
                printf("Frame recording enabled.\n") ;
                Recording = 1  ;
            }
            FrSaver.Toggle(Width);
            break ;
        case 'h':
        case '?':
            instructions();
            break;
    }
    glutPostRedisplay() ;

}


/**********************************************
 PROC: myMouseCB()
 DOES: handles the mouse button interaction
 
 -- don't change
 **********************************************************/
void myMouseCB(int button, int state, int x, int y)
{
    Button = button ;
    if( Button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        HVect arcball_coords;
        arcball_coords.x = 2.0*(float)x/(float)Width-1.0;
        arcball_coords.y = -2.0*(float)y/(float)Height+1.0;
        Ball_Mouse(Arcball, arcball_coords) ;
        Ball_Update(Arcball);
        Ball_BeginDrag(Arcball);

    }
    if( Button == GLUT_LEFT_BUTTON && state == GLUT_UP )
    {
        Ball_EndDrag(Arcball);
        Button = -1 ;
    }
    if( Button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        PrevY = y ;
    }


    // Tell the system to redraw the window
    glutPostRedisplay() ;
}


/**********************************************
 PROC: myMotionCB()
 DOES: handles the mouse motion interaction
 
 -- don't change
 **********************************************************/
void myMotionCB(int x, int y)
{
    if( Button == GLUT_LEFT_BUTTON )
    {
        HVect arcball_coords;
        arcball_coords.x = 2.0*(float)x/(float)Width - 1.0 ;
        arcball_coords.y = -2.0*(float)y/(float)Height + 1.0 ;
        Ball_Mouse(Arcball,arcball_coords);
        Ball_Update(Arcball);
        glutPostRedisplay() ;
    }
    else if( Button == GLUT_RIGHT_BUTTON )
    {
        if( y - PrevY > 0 )
            Zoom  = Zoom * 1.03 ;
        else 
            Zoom  = Zoom * 0.97 ;
        PrevY = y ;
        glutPostRedisplay() ;
    }
}


/**********************************************
 PROC: myReshape()
 DOES: handles the window being resized
 
 -- don't change
 **********************************************************/
void myReshape(int w, int h)
{
    Width = w;
    Height = h;
    
    glViewport(0, 0, w, h);
    
    mat4 projection = Perspective(50.0f, (float)w/(float)h, 1.0f, 1000.0f);
    glUniformMatrix4fv( uProjection, 1, GL_TRUE, projection );
}


/*********************************************************
 **********************************************************
 **********************************************************
 
 PROC: display()
 DOES: this gets called by the event handler to draw the scene
       so this is where you need to build your BEE
 
 MAKE YOUR CHANGES AND ADDITIONS HERE
 
 ** Add other procedures, such as drawLegs
 *** Use a hierarchical approach
 
 **********************************************************
 **********************************************************
 **********************************************************/
void drawMonsterTop(mat4& model_trans, mat4& view_trans, const vec3& loc, char x)
{
	//for traps
	GLfloat l = 2.0f, h = 0.5f, w = 2.0f;
	//for monster
	GLfloat ml = 2.0f, mh = 0.5f, mw = 1.5f;
	if (x == 't') //for monster with eye
	{
		//top half
		model_trans *= Translate(loc.x, loc.y + 0.5 + 0.9*sin(7.0*TIME), loc.z); //periodic motion
		//model_trans *= RotateY(30.0f);
		mvstack.push(model_trans); //save top half for teeth
		
		model_trans = mvstack.pop(); //get top half loc
		mvstack.push(model_trans); //save top half

		//Monster eyes
		GLfloat eye_size = 1.0f;

		//Right eye
		model_trans *= Translate(0.5, 0.7f, 0.7f);
		
		model_trans *= Scale(eye_size);
		model_trans *= RotateX(-45.0f * TIME);
		model_view = view_trans * model_trans;
		set_colour(10.0f, 0.0f, 0.0f);
		drawCube();

		model_trans = mvstack.pop(); //get top half loc
		mvstack.push(model_trans); //save top half

		//Left eye
		model_trans *= Translate(0.5, 0.7f, -0.7f);
		
		model_trans *= Scale(eye_size);
		model_trans *= RotateX(45.0f * TIME);
		model_view = view_trans * model_trans;
		set_colour(10.0f, 0.0f, 0.0f);
		drawCube();
		
		model_trans = mvstack.pop(); //get top half loc
		mvstack.push(model_trans); //save top half
	}
	if (x == 'b') //bottom for monster
	{
		//bottom half
		model_trans *= Scale(1.0f, -1.0f, 1.0f); //reflects top basically
		model_trans *= Translate(loc.x, loc.y + 0.5 + 0.9*sin(7.0*TIME), loc.z); //periodic motion
		//model_trans *= RotateY(30.0f * TIME);
		mvstack.push(model_trans); //save top half for teeth
	}
	else if (x == 'e') //if used for end then rotate so facing field and scale it
	{
		model_trans *= Translate(loc.x, loc.y, loc.z);
		model_trans *= Scale(10.0f);
		model_trans *= RotateX(90.0f * TIME);
		model_trans *= RotateZ(-90.0f);
		mvstack.push(model_trans); //save top half for teeth
	}
	else if (x == 's') //used for setting2
	{
		model_trans *= Translate(loc.x, loc.y + 0.5 + 0.5*sin(7.0*TIME), loc.z); //periodic motion
		model_trans *= Scale(3.0f);
		model_trans *= RotateY(30.0f);
		mvstack.push(model_trans); //save top half for teeth
	}
	model_trans = mvstack.pop(); //get top half loc
	mvstack.push(model_trans); //save top half

	//top half bod
	
	if (x == 't' || x == 'b') model_trans *= Scale(ml, mh, mw);
	else model_trans *= Scale(l, h, w);
	model_view = view_trans * model_trans;
	if (x == 't' || x == 'b') set_colour(0.0, 0.0, 0.0);
	else set_colour(0.5, 0.5, 0.5);
	//drawCube();
	if (x == 't' || x == 'b') drawCylinder();
	else drawCube();

	model_trans = mvstack.pop(); //get loc of top half
	mvstack.push(model_trans); // save loc of top half
	
	//teeth
	GLfloat t_l = 0.2f, t_h = 0.3f, t_w = t_l;
	model_trans *= Translate(-0.8f, 0.0f, -0.8f);
	mvstack.push(model_trans); //save loc of teeth set
	for (GLfloat i = 0.0f; i < 2.0f; i += 0.8f)
	{
		for (GLfloat j = 0.0f; j < 2.0f; j += 0.8f)
		{

			model_trans = mvstack.pop(); //get loc of teeth set
			mvstack.push(model_trans); //save loc of teeth set

			model_trans *= Translate(i, -0.5f, j);
			if (x == 't' || x == 'b') model_trans *= Scale(3.3 * t_l, 2.8 * t_h, 3.5 * t_w);
			else model_trans *= Scale(t_l, t_h, t_w);
			model_trans *= RotateX(-90.0f);
			model_view = view_trans * model_trans;
			if (x == 't' || x == 'b') set_colour(1.0, 1.0, 1.0);
			else set_colour(0.5, 0.5, 0.5);
			drawCone();
		}
	}
	mvstack.pop(); //get rid of loc of teeth set
	mvstack.pop(); //get rid of top half origin
}

void drawMonster(mat4& model_trans, mat4& view_trans, const vec3& loc)
{
	GLfloat l = 2.0f, h = 0.5f, w = 2.0f;

	model_trans *= Translate(Monster_x, Monster_y, Monster_z);
	mvstack.push(model_trans); //save origin

	//top half
	model_trans *= Translate(loc.x, loc.y + 1.6 + 0.5*sin(5.0*TIME), loc.z); //periodic motion
	mvstack.push(model_trans); //save top half

	model_trans = mvstack.pop(); //get top half loc
	mvstack.push(model_trans); //save top half
	
	drawMonsterTop(model_trans, view_trans, loc, 't');

	mvstack.pop(); //pop off top half

	model_trans = mvstack.pop(); //get origin
	mvstack.push(model_trans); //save origin
	
	//bottom half is top half reflected
	model_trans *= Translate(-0.2f, 0.0f, 0.0f);
	//model_trans *= Scale(1.0f, -1.0f, 1.0f); //reflect
	drawMonsterTop(model_trans, view_trans, loc, 'b');

	mvstack.pop(); //pop origin
}

//ARROW NOT USED FOR THIS ASSIGNMENT
void drawArrow(mat4& model_trans, mat4& view_trans, const vec3& loc)
{
	//random color
	float r = rand() % (5 - 1);
	srand(time(0));
	float b = rand() % (5 - 1);
	srand(time(0));
	float g = rand() % (5 - 1);
	//*** Arrow Model *****
	bird_x = (loc.x * (40 * TIME)) + 2.0f;
	model_trans *= Translate(loc.x * (40* TIME), loc.y, loc.z);
	model_trans *= Scale(0.3f);
	model_trans *= RotateY(-90.0f);
	model_trans *= RotateX(500.0 * TIME);
	
	mvstack.push(model_trans); //save arrow location

	//Arrow Head
	model_trans *= Scale(1.5, 1.0f, 0.5f);
	model_trans *= RotateY(-90.0);

	model_view = view_trans * model_trans;
	set_colour(r, g, b);
	drawCone();

	model_trans = mvstack.pop(); //get arrow location
	mvstack.push(model_trans); //save arrow location

	//Arrow Body
	GLfloat ab_length = 5.0f, ab_height = 0.4f, ab_width = ab_height;
	model_trans *= Translate(-6.0f, 0.0f, 0.0f);
	model_trans *= Scale(ab_length, ab_height, ab_width);
	model_trans *= RotateY(-90.0);
	model_view = view_trans * model_trans;
	set_colour(0.4f, 0.2f, 0.0f);
	drawCylinder();

	model_trans = mvstack.pop(); //get arrow location
	mvstack.push(model_trans); //save arrow location

	//Arrow Feathers
	GLfloat af_length = 1.5f, af_height = 0.7f, af_width = 0.2f;

	//Top feather
	model_trans *= Translate(-2.0f * ab_length - 0.5f, ab_height + 0.2, 0.0f);
	model_trans *= Scale(af_length, af_height, af_width);
	model_view = view_trans * model_trans;
	set_colour(r, g ,b);
	drawCube();

	model_trans = mvstack.pop(); //get arrow location
	mvstack.push(model_trans); //save arrow location

	//Bottom feather
	model_trans *= Translate(-2.0f * ab_length - 0.5f, -ab_height - 0.2, 0.0f);
	model_trans *= Scale(af_length, af_height, af_width);
	model_view = view_trans * model_trans;
	set_colour(r, g, b);
	drawCube();

	model_trans = mvstack.pop(); //get arrow location
	mvstack.push(model_trans); //save arrow location

	//Right feather
	model_trans *= Translate(-2.0f * ab_length - 0.5f, 0.0f, ab_width + 0.2);
	model_trans *= RotateX(90.0);
	model_trans *= Scale(af_length, af_height, af_width);
	model_view = view_trans * model_trans;
	set_colour(r, g, b);
	drawCube();

	model_trans = mvstack.pop(); //get arrow location
	mvstack.push(model_trans); //save arrow location

	//Left feather
	model_trans *= Translate(-2.0f * ab_length - 0.5f, 0.0f, -ab_width - 0.2);
	model_trans *= RotateX(90.0);
	model_trans *= Scale(af_length, af_height, af_width);
	model_view = view_trans * model_trans;
	set_colour(r, g, b);
	drawCube();

	model_trans = mvstack.pop(); //get rid of arrow location
}

void drawTree(mat4& model_trans, mat4& view_trans, const vec3& loc)
{
	//tree trunk size
	GLfloat t_length = 0.5f, t_height = 1.0f, t_width = t_length;

	//tree part1
	
	model_trans *= Translate(loc);
	model_trans *= Scale(1.5f);
	mvstack.push(model_trans); //save tree part1
	model_trans *= Scale(t_length, t_height, t_width);
	model_view = view_trans * model_trans;
	set_colour(0.4f, 0.2f, 0.0f);
	drawCube();
	

	model_trans = mvstack.pop(); //get tree part 1
	//tree part2
	model_trans *= Translate(0.0f, 1, 0.0f);
	mvstack.push(model_trans); //save tree part2
	model_trans *= Scale(t_length, t_height, t_width);
	model_view = view_trans * model_trans;
	set_colour(0.4f, 0.2f, 0.0f);
	drawCube();
	
	GLfloat size = 1;
	//tree (top) part
	for (int i = 0; i < 7; i++)
	{
		model_trans = mvstack.pop(); //get tree part

		//tree (top) part
		model_trans *= Translate(0.0f, size, 0.0f);
		mvstack.push(model_trans); //save tree (top part
		model_trans *= Scale(size);
		model_trans *= RotateX(90.0);
		model_view = view_trans * model_trans;
		set_colour(0.0f, 1.0f, 0.0f);
		drawCone();
		//cout << size << endl;
		size = size - 0.1;
	}

	mvstack.pop(); //get rid of tree part7
}

//TARGET NOT USED FOR THIS ASSIGNMENT
void drawTarget(mat4& model_trans, mat4& view_trans, const vec3& loc)
{
	model_trans *= Translate(loc.x, loc.y, loc.z);
	model_trans *= Scale(0.4f);
	model_trans *= RotateY(-90.0f);
	mvstack.push(model_trans);
	GLfloat t_length = 2.0f, t_height = t_length, t_width = 0.5f;
	GLfloat x = loc.x, y = loc.y, z = loc.z;

	for (int i = 0; i < 5; i++)
	{
		//model_trans = mvstack.pop();
		//mvstack.push(model_trans);
		model_trans *= Scale(t_length, t_height, t_width);
		model_trans *= Translate(0.0, 0.0, 0.0);
		
		model_view = view_trans * model_trans;
		if (i % 2 == 0) //evens are red
			set_colour(1.0f, 0.0f, 0.0f);
		else //odds are white
			set_colour(1.0f, 1.0f, 1.0f);
		drawCylinder();

		t_length -= 0.1f;
		t_height = t_length;
		z += 0.01f;
	}
	mvstack.pop();
}

void drawBird(mat4& model_trans, mat4& view_trans, const vec3& loc)
{
	//using f(x) = a + bsin(wt) for periodic motion along Y axis (up/down)
	//loc.x in this case is bird_x defined by monster
	model_trans *= Translate(loc.x, bird_y, bird_z); //periodic motion
	model_trans *= Scale(0.9f);
	model_trans *= RotateZ(bird_turn);
	mvstack.push(model_trans); //save bird origin

	//center of bird
	//Bird body
	GLfloat bod_length = 1.0f, 
		bod_height = 0.5f, 
		bod_width = bod_height;

	model_trans *= Scale(bod_length, bod_height, bod_width);
	model_view = view_trans * model_trans;
	set_colour(0.0f, 0.6f, 2.0f);
	drawSphere();

	model_trans = mvstack.pop(); //get bird origin
	mvstack.push(model_trans); //save bird origin

	//Bird head
	GLfloat head_size = 0.4f;
	model_trans *= Translate(1.1f, 0.0f, 0.0f);
	mvstack.push(model_trans); //save loc of head
	model_trans *= Scale(head_size);
	model_view = view_trans * model_trans;
	set_colour(0.0f, 0.8f, 2.0f);
	drawSphere();

	model_trans = mvstack.pop(); //get bird head loc
	mvstack.push(model_trans); //save bird head loc

	//Bird eyes
	GLfloat eye_size = 0.1f;
	
	//Right eye
	model_trans *= Translate(0.2f, 0.2f, 0.2f);
	model_trans *= Scale(eye_size);
	model_view = view_trans * model_trans;
	set_colour(0.0f, 0.0f, 0.0f);
	drawSphere();

	model_trans = mvstack.pop(); //get bird head loc
	mvstack.push(model_trans); //save bird head loc

	//Left eye
	model_trans *= Translate(0.2f, 0.2f, -0.2f);
	model_trans *= Scale(eye_size);
	model_view = view_trans * model_trans;
	set_colour(0.0f, 0.0f, 0.0f);
	drawSphere();

	mvstack.pop(); //get rid of head loc

	model_trans = mvstack.pop(); //get bird origin
	mvstack.push(model_trans); //save bird origin

	//Bird beak
	GLfloat beak_length = 1.0f,
		beak_height = 0.2f,
		beak_width = 0.3;
	model_trans *= Translate(1.5f, 0.0f, 0.0f);
	model_trans *= Scale(beak_length, beak_height, beak_width);
	model_trans *= RotateY(-90.0f);
	model_view = view_trans * model_trans;
	set_colour(1.0f, 1.0f, 0.0f);
	drawCone();

	model_trans = mvstack.pop(); //get bird origin
	mvstack.push(model_trans); //save bird origin

	//Bird tail
	GLfloat tail_length = 1.0f,
		tail_height = 0.1f,
		tail_width = 0.5;

	model_trans *= Translate(-1.0f, 0.0f, 0.0f);
	model_trans *= Scale(tail_length, tail_height, tail_width);
	model_trans *= RotateY(90.0f);
	model_trans *= RotateX(45.0f);
	model_view = view_trans * model_trans;
	set_colour(0.0f, 0.8f, 2.0f);
	drawCube();

	model_trans = mvstack.pop(); //get bird origin
	mvstack.push(model_trans); //save bird origin
	//Bird Wing
	GLfloat wing_length = 0.5f,
		wing_height = 0.1f,
		wing_width = 1.5f;

	//Bird Right Wing
	model_trans *= Translate(0.0f, 0.4f, 0.3f);
	mvstack.push(model_trans); //save where right wing is

	model_trans *= RotateX(-60 * sin(10.0f * TIME));
	model_trans *= Scale(wing_length, wing_height, wing_width);
	//Translate so end is at origin of wing (pivot)
	model_trans *= Translate(0.0f, 0.0f, 0.5f);
	model_view = view_trans * model_trans;
	set_colour(0.0f, 1.1f, 2.0f);
	drawCube();

	mvstack.pop(); //get rid of where right wing is 
	model_trans = mvstack.pop(); //get bird origin
	mvstack.push(model_trans); //save bird origin

	//Bird Left Wing
	model_trans *= Translate(0.0f, 0.4f, -0.3f);
	mvstack.push(model_trans); //save where right wing is

	model_trans *= RotateX(60 * sin(10.0f * TIME));
	model_trans *= Scale(wing_length, wing_height, wing_width);
	model_trans *= Scale(1.0f, 1.0f, 1.0f); //reflect
	//Translate so end is at origin of wing (pivot)
	model_trans *= Translate(0.0f, 0.0f, -0.5f);
	set_colour(0.0f, 1.1f, 2.0f);
	model_view = view_trans * model_trans;
	drawCube();

	mvstack.pop(); //get rid of where left wing is 

	mvstack.pop(); //get rid of bird origin

}

void drawStruct(mat4& model_trans, mat4& view_trans, const vec3& loc)
{
	model_trans *= Scale(4.0f);
	mvstack.push(model_trans); //save location of struct

	GLfloat pillar_length = 0.3f,
		pillar_height = 1.0f,
		pillar_width = pillar_length;
	srand(time(0));
	float r = rand() % (5 - 1);
	srand(time(0));
	float b = rand() % (5 - 1);
	srand(time(0));
	float g = rand() % (5 - 1);

	model_trans = mvstack.pop(); //get location of struct
	mvstack.push(model_trans); //save location of struct
	//Right pillar
	model_trans *= Translate(loc.x, loc.y, loc.z + 1.0f);
	model_trans *= Scale(pillar_length, pillar_height, pillar_width);
	model_trans *= RotateX(90.0f);
	model_view = view_trans * model_trans;
	set_colour(r, g, b);
	drawCylinder();

	model_trans = mvstack.pop(); //get location of struct
	mvstack.push(model_trans); //save location of struct
	//Left pillar
	model_trans *= Translate(loc.x, loc.y, loc.z - 1.0f);
	model_trans *= Scale(pillar_length, pillar_height, pillar_width);
	model_trans *= RotateX(90.0f);
	model_view = view_trans * model_trans;
	set_colour(r, g, b);
	drawCylinder();


	//Roofs
	GLfloat roof_length = 0.7f,
		roof_height = 0.5f,
		roof_width = 3.0f,
		roof_y = loc.y;
	for (int i = 0; i < 4; i++)
	{
		model_trans = mvstack.pop(); //get location of struct
		mvstack.push(model_trans); //save location of struct

		srand(time(0));
		float r = rand() % (5 - 1);
		float b = rand() % (5 - 1);
		float g = rand() % (5 - 1);
		model_trans *= Translate(loc.x, roof_y + 1.0f, loc.z);
		model_trans *= Scale(roof_length, roof_height, roof_width);
		model_view = view_trans * model_trans;
		set_colour(r, g, b);
		drawCube();

		roof_width -= 0.5f;
		roof_y += 0.5f;
	}

	mvstack.pop(); //get rid of location of struct

}

void drawSetting1(mat4& model_trans, mat4& view_trans, const vec3& loc)
{
	//static ground plane 
	GLfloat g_length = 300.0f, g_height = 1.0f, g_width = 11.0f;
	model_trans *= Translate(100.0f, -5.0f + loc.y , 0.0f);
	mvstack.push(model_trans); //saves ground as origin
	model_trans *= Scale(g_length, g_height, g_width);
	model_view = view_trans * model_trans;
	set_colour(0.0f, 0.25f, 0.0f);
	drawCube();

	model_trans = mvstack.pop(); //get ground location
	mvstack.push(model_trans); // save ground location
	//*** Tree Model ******	
	if (firstDisplayCall)
	{
		/*
		//make random locations for tree vector
		srand(time(0));
		for (int i = 0; i < 20; i++)
		{
		float x = rand() % (100 - 1);
		float z = rand() % (10 - 1);
		z = z - 5.0f;
		tree_xLoc[i] = x;
		tree_zLoc[i] = z;
		}
		*/
		/*
		x ranges between 0.0f and 100.0f
		z ranges between 2.0f and 4.9f*/
		float xfloor = 80.0f, xceil = 300.0f, xrange = (xfloor - xceil);
		float zfloor = 2.0f, zceil = 4.9f, zrange = (zceil - zfloor);
		for (int i = 0; i < 80; i++)
		{
			float x = xfloor + float((xrange * rand()) / (RAND_MAX + 1.0));
			float z = zfloor + float((zrange * rand()) / (RAND_MAX + 1.0));
			if (i < 21)
			{
				tree_xLoc[i] = x;
				tree_zLoc[i] = z;
			}
			else
			{
				tree_xLoc[i] = x;
				tree_zLoc[i] = -z;
			}
		}
	}

	//draw the trees
	for (int i = 0; i < 80; i++)
	{
		model_trans = mvstack.pop(); //get ground location
		mvstack.push(model_trans); // save ground location

		vec3 newLoc(tree_xLoc[i], 1.0f, tree_zLoc[i]);

		drawTree(model_trans, view_trans, newLoc);
	}

	//draw 5 structures
	for (int i = -30; i < 30; i = i + 10)
	{
		model_trans = mvstack.pop(); //get ground location
		mvstack.push(model_trans); // save ground location

		vec3 locc(i, 1.0f, 0.0f);
		drawStruct(model_trans, view_trans, locc);
	}

	mvstack.pop(); //pop off ground location
}

void drawSetting2(mat4& model_trans, mat4& view_trans, const vec3& loc)
{
	//srand(time(0));
	float r1 = rand() % (5 - 1);
	float b1 = rand() % (5 - 1);
	float g1 = rand() % (5 - 1);
	float r2 = rand() % (5 - 1);
	float b2 = rand() % (5 - 1);
	float g2 = rand() % (5 - 1);
	float r3 = rand() % (5 - 1);
	float b3 = rand() % (5 - 1);
	float g3 = rand() % (5 - 1);
	float r4 = rand() % (5 - 1);
	float b4 = rand() % (5 - 1);
	float g4 = rand() % (5 - 1);

	model_trans *= RotateX(15.0 * TIME);
	model_trans *= Scale(1.5f);
	mvstack.push(model_trans); //save location of setting
	GLfloat g_length = 250.0f, g_height = 1.0f, g_width = 11.0f;

	model_trans = mvstack.pop(); //get loc of setting
	mvstack.push(model_trans); //save loc of setting

	//top of box
	srand(time(0));
	
	model_trans *= Translate(loc.x, loc.y + 5.0, loc.z);
	mvstack.push(model_trans); //save top loc
	model_trans *= Scale(g_length, g_height, g_width);
	model_view = view_trans * model_trans;
	set_colour(r1, g1, b1);
	drawCube();

	//draw top of box traps
	for (GLfloat i = 10.0f; i < 240.0f; i += 40.0f)
	{
		model_trans = mvstack.pop(); //get top loc of box
		mvstack.push(model_trans); //save top loc of box

		vec3 loc(i - 100.0f, -1.4f, 0.0f);
		drawMonsterTop(model_trans, view_trans, loc, 's');
	}
	mvstack.pop(); //get rid of top loc
	model_trans = mvstack.pop(); //get loc of setting
	mvstack.push(model_trans); //save loc of setting

	//bottom of box
	model_trans *= Translate(loc.x, loc.y - 5.0, loc.z);
	mvstack.push(model_trans); //save bottom loc
	model_trans *= Scale(g_length, g_height, g_width);
	model_view = view_trans * model_trans;
	set_colour(r2, g2, b2);
	drawCube();

	//draw bottom of box traps
	for (GLfloat i = 20.0f; i < 240.0f; i += 40.0f)
	{
		model_trans = mvstack.pop(); //get loc of bottom of box
		mvstack.push(model_trans); //save bottom of box

		vec3 loc(i - 100.0f, -1.4f, 0.0f);
		model_trans *= RotateX(-180.0f);
		drawMonsterTop(model_trans, view_trans, loc, 's');
	}
	
	mvstack.pop(); //get rid of bot loc
	model_trans = mvstack.pop(); //get loc of setting
	mvstack.push(model_trans); //save loc of setting

	//right of box
	model_trans *= Translate(loc.x, loc.y, loc.z + 6.0);
	mvstack.push(model_trans); //save right loc of box
	model_trans *= RotateX(90.0f);
	model_trans *= Scale(g_length, g_height, g_width);
	model_view = view_trans * model_trans;
	set_colour(r3, g3, b3);
	drawCube();

	//draw right of box traps
	for (GLfloat i = 30.0f; i < 220.0f; i += 40.0f)
	{
		model_trans = mvstack.pop(); //get loc of right of box
		mvstack.push(model_trans); //save right of box

		vec3 loc(i - 100.0f, -1.3f, 0.0f);
		model_trans *= RotateX(90.0f);
		drawMonsterTop(model_trans, view_trans, loc, 's');
	}

	mvstack.pop(); //get rid of right loc
	model_trans = mvstack.pop(); //get loc of setting
	mvstack.push(model_trans); //save loc of setting

	//left of box
	model_trans *= Translate(loc.x, loc.y, loc.z - 6.0);
	mvstack.push(model_trans); //save left of box
	model_trans *= RotateX(90.0f);
	model_trans *= Scale(g_length, g_height, g_width);
	model_view = view_trans * model_trans;
	set_colour(r4, g4, b4);
	drawCube();

	//draw left of box traps
	for (GLfloat i = 40.0f; i < 240.0f; i += 40.0f)
	{
		model_trans = mvstack.pop(); //get loc of left of box
		mvstack.push(model_trans); //save left of box

		vec3 loc(i - 100.0f, -1.3f, 0.0f);
		model_trans *= RotateX(-90.0f);
		drawMonsterTop(model_trans, view_trans, loc, 's');
	}

	mvstack.pop(); //get rid of left loc 
	mvstack.pop(); //get rid of box loc
}

void display(void)
{
    // Clear the screen with the background colour (set in myinit)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4 model_trans(1.0f);
    mat4 view_trans(1.0f);
    
    view_trans *= Translate(0.0f, 0.0f, -15.0f); //the same effect as zoom out
    
    // below deals with zoom in/out by mouse
    HMatrix r;
    Ball_Value(Arcball,r);
    mat4 mat_arcball_rot(
                         r[0][0], r[0][1], r[0][2], r[0][3],
                         r[1][0], r[1][1], r[1][2], r[1][3],
                         r[2][0], r[2][1], r[2][2], r[2][3],
                         r[3][0], r[3][1], r[3][2], r[3][3]);
    view_trans *= mat_arcball_rot;
    view_trans *= Scale(Zoom);
    
    glUniformMatrix4fv( uView, 1, GL_TRUE, model_view );
    
    mvstack.push(model_trans);//push, now identity is on the stack
    
   
/**************************************************************
   Your drawing/modeling starts here
***************************************************************/
	
	//setting up camera
	view_trans *= LookAt(eye, ref, up);
	
	//setting1
	vec3 loc1(0.0, 0.5, 0.0);
	drawSetting1(model_trans, view_trans, loc1);

	model_trans = mvstack.pop(); //get identity
	mvstack.push(model_trans); //save identity

	//setting2
	vec3 loc2(280.0, 0.0, 0.0);
	drawSetting2(model_trans, view_trans, loc2);

	model_trans = mvstack.pop(); //pop off identity
	mvstack.push(model_trans);
	
	//*** Bird Model 1*********
	vec3 locb1(bird_x, 0.0f, 0.0f);
	drawBird(model_trans, view_trans, locb1);
	//////////////////////////
	
	model_trans = mvstack.pop(); //get identity
	mvstack.push(model_trans); //save identity

	//*****Monster Model******
	vec3 locm(0.1f, 0.1f, 0.0f);
	drawMonster(model_trans, view_trans, locm);
	/////////////////////////////
	
	model_trans = mvstack.pop(); //get identity
	mvstack.push(model_trans); //save identity

	//******End Trap Model******
	vec3 loce(620.0f, 0.0f, 0.0f);
	drawMonsterTop(model_trans, view_trans, loce, 'e');
	
	//vec3 locm(0.1f, 0.1f, 0.0f);
	//drawMonster(model_trans, view_trans, locm);

	mvstack.pop();

	firstDisplayCall = false;
/**************************************************************
     Your drawing/modeling ends here
 ***************************************************************/
    
    glutSwapBuffers();
    if(Recording == 1)
        FrSaver.DumpPPM(Width, Height);
}


/*********************************************************
 **********************************************************
 **********************************************************
 
 PROC: idle()
 DOES: this gets called when nothing happens. 
       That's not true. 
       A lot of things happen can here.
       This is where you do your animation.
 
 MAKE YOUR CHANGES AND ADDITIONS HERE
 
 **********************************************************
 **********************************************************
 **********************************************************/
void idle(void)
{
    if( Animate == 1 )
    {
        // TM.Reset() ; // commenting out this will make the time run from 0
        // leaving 'Time' counts the time interval between successive calls to idleCB
        if( Recording == 0 )
            TIME = TM.GetElapsedTime() ;
        else
            TIME += 0.033 ; // save at 30 frames per second.

		//Your code starts here
		//ANIMATION
		//while bird is still on trail
		
		if (bird_x < 612.0f)
		{
			bird_x = 15.0 * TIME;
			//in case of unsmooth animation (teleports back to 255)
			if (bird_x > 612.0f)
			{
				bird_x = 612.0f;
			}
			bird_y = 1.0*sin(3.0*TIME);
			bird_z = 0.5*sin(1.0*TIME);
			//monster acts normally
			Monster_x = bird_x - 3.2f; //make monster follow bird
			Monster_y = -0.9f;
			Monster_z = 0.0;
			//std::cout << "bird_x" << bird_x << "\n";

		}
		
		//when bird is off trail
		if (bird_x >= 612.0f)
		{
			//bird should fly upwards
			bird_x = 612.0f;
			bird_y += 0.01 * TIME;
			//rotates up and down z axis
			GLfloat temp = sin(-5.0f * TIME);
			//if bird pointing straight up, then makes it stay pointing straight up
			//in the meantime bird gradually points straight up
			if (temp <= 0)
				temp *= (-1);
			if (temp >= 0.50f)
				bird_turn_rot = 0.93f;
			//std::cout << "*******temp " << temp << "\n";
			if (bird_turn_rot >= 0.93f)
				bird_turn = 70.0 * bird_turn_rot;
			else
				bird_turn = 70.0 * temp;
			//monster falls down
			Monster_y -= 0.03 * TIME; //not using 9.8 for gravity b/c too fast
			Monster_z = 0.0;

		}
		///////////////////////////////////////////

		//CAMERA ANGLES
		
		//side scroll monster pov
		else if ((TIME > 0 && TIME <= 4))
		{
			eye = Angel::vec4(bird_x + 10.0f * sin(1.0f *TIME), Monster_y + 2.0f * sin(1.0f * TIME), Monster_z + 5.0f, 1.0);
			ref = Angel::vec4(bird_x, Monster_y, Monster_z, 1.0);
			up = Angel::vec4(0.0, 1.0, 0.0, 1.0);
		}
		//360 degree camera pov
		else if (TIME > 4 && TIME <= 9)
		{
			eye = Angel::vec4(255.0 * cos(TIME), 30.0, 255.0 * sin(TIME), 1.0);
			ref = Angel::vec4(50.0, Monster_y, Monster_z, 1.0);
			up = Angel::vec4(0.0, 1.0, 0.0, 1.0);
		}
		//top down pov
		else if ((TIME > 9 && TIME <= 13))
		{
			eye = Angel::vec4(Monster_x, Monster_y + 3.0f, Monster_z , 1.0);
			ref = Angel::vec4(Monster_x + 2.0f, Monster_y, Monster_z, 1.0);
			up = Angel::vec4(1.0, 0.0, 0.0, 1.0);
		}
		//looking into setting2
		else if (TIME > 13 && TIME <= 18)
		{
			eye = Angel::vec4(180 + 2.0 * TIME, Monster_y, Monster_z, 1.0);
			ref = Angel::vec4(210.0, Monster_y, Monster_z, 1.0);
			up = Angel::vec4(0.0, 1.0, 0.0, 1.0);
		}
		//behind bird
		else if (TIME > 18 && TIME <= 22)
		{
			eye = Angel::vec4(bird_x, bird_y + 1.5f, Monster_z, 1.0);
			ref = Angel::vec4(bird_x + 10.0f, Monster_y, Monster_z, 1.0);
			up = Angel::vec4(1.0, 0.0, 0.0, 1.0);
		}
		//in front of bird
		else if ((TIME > 22 && TIME <= 25))
		{
			eye = Angel::vec4(bird_x + 10.0, Monster_y, Monster_z, 1.0);
			ref = Angel::vec4(bird_x, Monster_y, Monster_z, 1.0);
			up = Angel::vec4(0.0, 1.0, 0.0, 1.0);
		}
		//look at end
		else if ((TIME > 25 && TIME <= 28))
		{
			eye = Angel::vec4(615.0f, 5.0f, 10.0f, 1.0);
			ref = Angel::vec4(615.0f, 0.0f, Monster_z, 1.0);
			up = Angel::vec4(0.0, 1.0, 0.0, 1.0);
		}
		//end of tunnel to bird
		else if ((TIME > 28 && TIME <= 32))
		{
			eye = Angel::vec4(600.0f - (5.0 * TIME), 0.0f, 0.0f, 1.0);
			ref = Angel::vec4(bird_x, Monster_y, Monster_z, 1.0);
		}
		//in front of bird
		else if ((TIME > 32 && TIME <= 36))
		{
			eye = Angel::vec4(bird_x + 10.0, Monster_y - 2.0f, Monster_z, 1.0);
			ref = Angel::vec4(bird_x, Monster_y, Monster_z, 1.0);
			up = Angel::vec4(0.0, 1.0, 0.0, 1.0);
		}
		//behind bird
		else if (TIME > 36 && TIME <= 40)
		{
			eye = Angel::vec4(bird_x, Monster_y + 2.0f, Monster_z, 1.0);
			ref = Angel::vec4(bird_x + 10.0f, Monster_y, Monster_z, 1.0);
			up = Angel::vec4(1.0, 0.0, 0.0, 1.0);
		}
		//look at end
		else if (TIME > 40 && TIME <= 43)
		{
			eye = Angel::vec4(615.0f, 0.0f, 10.0f, 1.0);
			ref = Angel::vec4(615.0f, 0.0f, Monster_z, 1.0);
			up = Angel::vec4(0.0, 1.0, 0.0, 1.0);
		}
		//watch bird fly away
		else if (TIME > 43 && TIME <= 45)
		{
			eye = Angel::vec4(bird_x + 0.5, bird_y + 10.0f, 0.0f, 1.0);
			ref = Angel::vec4(bird_x, bird_y, 0.0f, 1.0);
			up = Angel::vec4(0.0, 1.0, 0.0, 1.0);
		}
		//Your code ends here

        printf("TIME %f\n", TIME) ;
        glutPostRedisplay() ;
    }
}

/*********************************************************
     PROC: main()
     DOES: calls initialization, then hands over control
           to the event handler, which calls 
           display() whenever the screen needs to be redrawn
**********************************************************/

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    // If your code fails to run, uncommenting these lines may help.
    //glutInitContextVersion(3, 2);
    //glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(Width,Height);
    glutCreateWindow(argv[0]);
    printf("GL version %s\n", glGetString(GL_VERSION));
    glewExperimental = GL_TRUE;
    glewInit();
    
    instructions();
    myinit(); //performs most of the OpenGL intialization
    
    
    glutKeyboardFunc( myKey );   //keyboard event handler
    glutMouseFunc(myMouseCB) ;   //mouse button event handler
    glutMotionFunc(myMotionCB) ; //mouse motion event handler
    
    glutReshapeFunc (myReshape); //reshape event handler
    glutDisplayFunc(display);    //draw a new scene
    glutIdleFunc(idle) ;         //when nothing happens, do animaiton

    
    glutMainLoop();

    TM.Reset() ;
    return 0;         // never reached
}




