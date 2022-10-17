//
//  main4.cpp
//

#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "glut.h"
#include "Sleep.h"
#include "ObjModel.h"
#include "TextureManager.h"
#include "Obj.h"

using namespace std;
using namespace ObjLibrary;
using namespace TextureManager;

void init ();
void initDisplay ();
void keyboard (unsigned char key, int x, int y);
void update ();
void reshape (int w, int h);
void display ();
void drawAxes ();
void readFile();	// Function to read text file and store it into data structures
void printInfo();	// Function to print the text file to the command line taking the information from the data structures
void findMax();		// Function to find the maximum frame number in the text file
void sortVector();	// Function to sort a vector based on Object ID
void calculateAge();// Function to calculate the last frame the object will exist at

// add your global variables here
int age = 0;	// Global variable to keep track of the clock timer
int objectCounter = 0;	// Keeps track of how many objects there are
int key = 0;			// Key to lock and open certain statements
int errorKey = 0;		// Error handling key
int globalMax = 0;		// Used to hold the global maximum key frame

ObjModel object[10];	// Array of models to use to load .obj files
Obj mainObject;			// Used to hold the values to push into the vector
Keyframe frame;			// Used to hold the values to be pushed into the keyframe vector
vector<Obj> animation;	// Main object holding all the data

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

	// Call functions before init to load all information necessary to display objects
	readFile();
	calculateAge();
	findMax();
	printInfo();
	

	init();

	

	glutMainLoop();  // contains an infinite loop, so it never returns
	return 1;
}

void init ()
{
	// Load all the object files into an objectModel array
	for (int i = 0; i < objectCounter; i++)
	{
		object[i].load(animation[i].objFile);
	}
	
	initDisplay();
}

void initDisplay ()
{
	glClearColor(0.0, 1.0, 0.0, 0.0);	// Set color to green
	glColor3f(0.0, 1.0, 0.0);

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
	// Increment the time being held in age variable
	age++;
	// sleep function to make it 60fps
	sleep(1.0 / 60.0);
	// statement to cancel the program once the animation is complete
	if (age == globalMax + 2)
	{
		cout << "Animation Complete, program will exit in 5 seconds." << endl;
		while (1)
		{
			sleep(5);
			exit(0);
		}
	}
	

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
	gluLookAt(7.0, 100, 7.0,  // (x, y, z) camera eye position
	          0.0, 0.0, 0.0,  // (x, y, z) camera look at position
	          0.0, 1.0, 0.0); // (x, y, z) camera up direction

	// camera is now set up - any drawing before here will display incorrectly

	// display positive X, Y, and Z axes near origin
	drawAxes();
	
	// Activate ant texture
	// CREATIVE FEATURE
	glEnable(GL_TEXTURE_2D);
	TextureManager::activate("Ant.bmp");
	for (int i = 0; i < objectCounter; i++)
	{
		// If the frame number equals the current time and the current age is less than the objects age then we enter this statement
		// this if statement is for the entries that come from the text file, this is if we have the information to print already
		if (animation[i].groupFrames[animation[i].counter].frameNum == age && animation[i].age >= age)
		{
			// Code to draw the object with values put in for translation, rotation and scaling
			glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glTranslated(animation[i].groupFrames[animation[i].counter].x, animation[i].groupFrames[animation[i].counter].y, animation[i].groupFrames[animation[i].counter].z);
			glRotated(animation[i].groupFrames[animation[i].counter].rotationX, 1.0, 0.0, 0.0);
			glRotated(animation[i].groupFrames[animation[i].counter].rotationY, 0.0, 1.0, 0.0);
			glRotated(animation[i].groupFrames[animation[i].counter].rotationZ, 0.0, 0.0, 1.0);
			glScaled(animation[i].groupFrames[animation[i].counter].scaleX, animation[i].groupFrames[animation[i].counter].scaleY, animation[i].groupFrames[animation[i].counter].scaleZ);
			object[i].draw();
			glPopMatrix();
			// Increment counter only if it is not going to go outside the framecount of the specific object
			if (animation[i].counter < animation[i].frameCount - 1)
			{
				animation[i].counter++;
			}
			// Print object doesn't exist beforet the objects first key frame
			if (animation[i].groupFrames[0].frameNum == age)
			{
				cout << "Object: " << animation[i].objID << " " << " Frame: " << age << " Object Does Not Exist!" << "\n\n";
			}
			// Print out the current values all values as seen in the text file
			cout << "Object: " << animation[i].objID << " Frame: " << age << " " << animation[i].groupFrames[animation[i].counter].x << " " << animation[i].groupFrames[animation[i].counter].y << " "
				<< animation[i].groupFrames[animation[i].counter].z << " " << animation[i].groupFrames[animation[i].counter].rotationX << " " << animation[i].groupFrames[animation[i].counter].rotationY << " "
				<< animation[i].groupFrames[animation[i].counter].rotationZ << " " << animation[i].groupFrames[animation[i].counter].scaleX << " " << animation[i].groupFrames[animation[i].counter].scaleY << " "
				<< animation[i].groupFrames[animation[i].counter].scaleZ << "\n\n";
			// Print out the object doesn't exist after the last keyframe that it exists
			if (animation[i].age == age)
			{
				cout << "Object: " << animation[i].objID << " " << " Frame: " << age + 1 << " Object Does Not Exist!" << "\n\n";
			}
			// Use a key so the linear interpolation is only calculated once per pair of keyframes and not again and again.
			animation[i].key = 0;
		}
		else if (animation[i].age >= age)
		{	
			if (animation[i].counter < animation[i].frameCount)
			{
				// Statement used to calculate the linear interpolation
				if (animation[i].key == 0)
				{
					animation[i].xIncrement = (animation[i].groupFrames[animation[i].counter].x - animation[i].groupFrames[animation[i].counter - 1].x) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));
					animation[i].yIncrement = (animation[i].groupFrames[animation[i].counter].y - animation[i].groupFrames[animation[i].counter - 1].y) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));
					animation[i].zIncrement = (animation[i].groupFrames[animation[i].counter].z - animation[i].groupFrames[animation[i].counter - 1].z) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));
					animation[i].rotationXincrement = (animation[i].groupFrames[animation[i].counter].rotationX - animation[i].groupFrames[animation[i].counter - 1].rotationX) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));
					animation[i].rotationYincrement = (animation[i].groupFrames[animation[i].counter].rotationY - animation[i].groupFrames[animation[i].counter - 1].rotationY) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));
					animation[i].rotationZincrement = (animation[i].groupFrames[animation[i].counter].rotationZ - animation[i].groupFrames[animation[i].counter - 1].rotationZ) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));
					animation[i].scaleXincrement = (animation[i].groupFrames[animation[i].counter].scaleX - animation[i].groupFrames[animation[i].counter - 1].scaleX) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));
					animation[i].scaleYincrement = (animation[i].groupFrames[animation[i].counter].scaleY - animation[i].groupFrames[animation[i].counter - 1].scaleY) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));
					animation[i].scaleZincrement = (animation[i].groupFrames[animation[i].counter].scaleZ - animation[i].groupFrames[animation[i].counter - 1].scaleZ) / ((animation[i].groupFrames[animation[i].counter].frameNum) - (animation[i].groupFrames[animation[i].counter - 1].frameNum));

					animation[i].key = 1;
				}
			}
			// Changing the values using the linear interpolation caculated above
			animation[i].groupFrames[animation[i].counter - 1].x += animation[i].xIncrement;
			animation[i].groupFrames[animation[i].counter - 1].y += animation[i].yIncrement;
			animation[i].groupFrames[animation[i].counter - 1].z += animation[i].zIncrement;
			animation[i].groupFrames[animation[i].counter - 1].rotationX += animation[i].rotationXincrement;
			animation[i].groupFrames[animation[i].counter - 1].rotationY += animation[i].rotationYincrement;
			animation[i].groupFrames[animation[i].counter - 1].rotationZ += animation[i].rotationZincrement;
			animation[i].groupFrames[animation[i].counter - 1].scaleX += animation[i].scaleXincrement;
			animation[i].groupFrames[animation[i].counter - 1].scaleY += animation[i].scaleYincrement;
			animation[i].groupFrames[animation[i].counter - 1].scaleZ += animation[i].scaleZincrement;

			// Print out the current values of the objects
			cout << "Object: " << animation[i].objID << " Frame: " << age << " " << animation[i].groupFrames[animation[i].counter - 1].x << " " << animation[i].groupFrames[animation[i].counter - 1].y << " "
				<< animation[i].groupFrames[animation[i].counter - 1].z << " " << animation[i].groupFrames[animation[i].counter - 1].rotationX << " " << animation[i].groupFrames[animation[i].counter - 1].rotationY << " "
				<< animation[i].groupFrames[animation[i].counter - 1].rotationZ << " " << animation[i].groupFrames[animation[i].counter - 1].scaleX << " " << animation[i].groupFrames[animation[i].counter - 1].scaleY << " "
				<< animation[i].groupFrames[animation[i].counter - 1].scaleZ << "\n\n";

			// Draw the linear interpolated objects
			glPushMatrix();
			glColor3d(1.0, 1.0, 1.0);
			glTranslated(animation[i].groupFrames[animation[i].counter - 1].x, animation[i].groupFrames[animation[i].counter - 1].y, animation[i].groupFrames[animation[i].counter - 1].z);
			glRotated(animation[i].groupFrames[animation[i].counter - 1].rotationX, 1.0, 0.0, 0.0);
			glRotated(animation[i].groupFrames[animation[i].counter - 1].rotationY, 0.0, 1.0, 0.0);
			glRotated(animation[i].groupFrames[animation[i].counter - 1].rotationZ, 0.0, 0.0, 1.0);
			glScaled(animation[i].groupFrames[animation[i].counter - 1].scaleX, animation[i].groupFrames[animation[i].counter - 1].scaleY, animation[i].groupFrames[animation[i].counter - 1].scaleZ);
			object[i].draw();
			glPopMatrix();
			
		}
	}
	
	glDisable(GL_TEXTURE_2D);

	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

void drawAxes ()
{
	glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(10.0, 0.0, 0.0);
		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 10.0, 0.0);
		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 10.0);
	glEnd();
}

// Function to read file
void readFile()
{
	// Local variables used to hold the values read from the text file
	int objID = 0;
	int frameNum = 0;
	float x, y, z, rotationX, rotationY, rotationZ, scaleX, scaleY, scaleZ;
	string objFile;
	string check;
	char ch;

	fstream my_file;
	my_file.open("animation1.txt", ios::in);
	if (!my_file) {
		cout << "No such file";
	}
	else {
		while (1) {
			my_file >> ch;
			check += ch;
			if (my_file.eof())
			{
				break;
			}
			// If initial word is OBJECT then we will store values in an object
			if (check == "OBJECT")
			{
				my_file >> objID >> objFile;
				mainObject.objID = objID;
				mainObject.objFile = objFile;
				animation.push_back(mainObject);
				check = "";
				objectCounter++;
			}
			// If initial word is KEYFRAME then we will store values in a frame obejct
			if (check == "KEYFRAME")
			{
				// Calls sort function if the formatting of the text file has things out of order it will order them for easier indexing in my vector for frames
				if (key == 0)
				{
					sortVector();
					key++;
				}
				my_file >> objID >> frameNum >> x >> y >> z >> rotationX >> rotationY >> rotationZ >> scaleX >> scaleY >> scaleZ;
				frame.objID = objID;
				frame.frameNum = frameNum;
				frame.x = x;
				frame.y = y;
				frame.z = z;
				frame.rotationX = rotationX;
				frame.rotationY = rotationY;
				frame.rotationZ = rotationZ;
				frame.scaleX = scaleX;
				frame.scaleY = scaleY;
				frame.scaleZ = scaleZ;

				// This loop is used if frames are out of order then we will look through each object to see which object id matches
				for (int i = 0; i < objectCounter; i++)
				{
					if (frame.objID == animation[i].objID)
					{
						// Error handling if a key frame is specified out of order returning the spot so the user can find it easily in their text file
						// If this isn't the first frame stored and the current frame number being looked at is less than the previous frame number logged for that object
						// CREATIVE FEATURE I think sending the frame number and object for user experience is simple but still a creative feature that I was not requested to do
						if ((animation[i].frameCount - 1 >= 0) && (animation[i].groupFrames[animation[i].frameCount - 1].frameNum > frameNum))
						{
							cout << "Key frame is greater than the next key frame. INVALID FORMAT. Object: " << animation[i].objID << " Frame: " << animation[i].groupFrames[animation[i].frameCount - 1].frameNum << "\n\n";
							exit(0);
						}
						// Otherwise we push the frame into the object and increment the frame count which keeps track of how many frames there are for that object
						else
						{
							animation[i].groupFrames.push_back(frame);
							animation[i].frameCount++;
							break;
						}
					}
					// If we checked every object in the for loop and the object id didn't match any of them then we send an error
					else if(i + 1 == objectCounter)
					{
						cout << "Object has not been specified. INVALID FORMAT. Object: " << objID << " Frame: " << frameNum << "\n\n";
						exit(0);
					}
				}

				// Reset string used to check if OBJECT or KEYFRAME
				check = "";
			}
		}
	}
	my_file.close();
}

// Function used to print the contents of the text file
void printInfo()
{
	// Loop through object printing out all frames and objects
	for (int i = 0; i < objectCounter; i++)
	{
		for (int j = 0; j < animation[i].frameCount; j++)
		{
				cout << animation[i].groupFrames[j].objID << " " << animation[i].groupFrames[j].frameNum << " " << animation[i].groupFrames[j].x << " " << animation[i].groupFrames[j].y << " " << animation[i].groupFrames[j].z
					<< " " << animation[i].groupFrames[j].rotationX << " " << animation[i].groupFrames[j].rotationY << " " << animation[i].groupFrames[j].rotationZ << " " << animation[i].groupFrames[j].scaleX << " "
					<< animation[i].groupFrames[j].scaleY << " " << animation[i].groupFrames[j].scaleZ << "\n\n";
		}
	}
}

// Find max to know when to stop program
void findMax()
{
	for (int i = 0; i < objectCounter; i++)
	{
		for (int j = 0; j < animation[i].frameCount; j++)
		{
			if (animation[i].groupFrames[j].frameNum > globalMax)
			{
				globalMax = animation[i].groupFrames[j].frameNum;
			}
		}
	}
}

// Sort vector for easier assigning of frames to the current index's for the objects based on ID's
void sortVector()
{
	sort(animation.begin(), animation.end(), [](const Obj& a, const Obj& b) { return (a.objID < b.objID);});
}

// Calculate the last key frame of each object
void calculateAge()
{
	for (int i = 0; i < objectCounter; i++)
	{
		animation[i].age = animation[i].groupFrames[animation[i].frameCount - 1].frameNum;
	}
}