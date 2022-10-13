//
//  main4.cpp
//

#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include "ObjModel.h"
#include "glut.h"
#include "Sleep.h"

using namespace std;

void init ();
void initDisplay ();
void keyboard (unsigned char key, int x, int y);
void update ();
void reshape (int w, int h);
void display ();
void drawAxes ();

// add your global variables here



int main (int argc, char* argv[])
{
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Loading OBJ Models");
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	init();

	glutMainLoop();  // contains an infinite loop, so it never returns
	return 1;
}

void init ()
{
	initDisplay();
}

void initDisplay ()
{
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glColor3f(0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // on [ESC]
		exit(0); // normal exit
		break;
	}
}

void update ()
{
	// update your variables here
	
	sleep(1.0 / 60.0);
	glutPostRedisplay();
}

void reshape (int w, int h)
{
	glViewport (0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void display ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display at all

	glLoadIdentity();
	gluLookAt(2.0, 3.0, 4.0,  // (x, y, z) camera eye position
	          0.0, 0.0, 0.0,  // (x, y, z) camera look at position
	          0.0, 1.0, 0.0); // (x, y, z) camera up direction

	// camera is now set up - any drawing before here will display incorrectly

	// display positive X, Y, and Z axes near origin
	drawAxes();

	// draw a purple wireframe cube
	glColor3d(1.0, 0.0, 1.0);
	glPushMatrix();
		glTranslated(0.0, 0.0, 0.0);
		glRotated(45, 0.0, 1.0, 0.0);
		glScaled(1.0, 1.0, 1.0);
		glutWireCube(1.0);
	glPopMatrix();

	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

void drawAxes ()
{
	glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(2.0, 0.0, 0.0);
		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 2.0, 0.0);
		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 2.0);
	glEnd();
}
