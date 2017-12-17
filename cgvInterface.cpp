#include <cstdlib>
#include <stdio.h>

#include "cgvInterface.h"

extern cgvInterface interface; // the callbacks must be static and this object is required to access to the variables of the class                   // ellos a las variables de la clase

							   // Constructor and destructor methods -----------------------------------

cgvInterface::cgvInterface() :camType(CGV_PARALLEL) {
	//// Section D: initialize the attributes to select objects by list of impacts
	mode = CGV_VISUALIZE;
}

cgvInterface::~cgvInterface() {}


// Public methods ----------------------------------------
void cgvInterface::create_world(void) {
	//Changed the camera view to the top
	camera = cgvCamera(cgvPoint3D(0, 8.0, 0), cgvPoint3D(0, 0, 0), cgvPoint3D(0, 0, 1),
		1 * 5, 1 * 5, 0.1, 200);

}

void cgvInterface::configure_environment(int argc, char** argv,
	int _width_window, int _height_window,
	int _pos_X, int _pos_Y,
	string _title)
{
	// initialization of the interface variables																	
	width_window = _width_window;
	height_window = _height_window;

	// initialization of the display window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(_width_window, _height_window);
	glutInitWindowPosition(_pos_X, _pos_Y);
	glutCreateWindow(_title.c_str());

	create_menu();

	glEnable(GL_DEPTH_TEST); // enable the removal of hidden surfaces by using the z-buffer
							 //Modified to put the colour black
	glClearColor(0, 0, 0, 0.0); // define the background color of the window

	glInitNames();

	glEnable(GL_LIGHTING); // enable the lighting of the scene
	glEnable(GL_NORMALIZE); // normalize the normal vectors required by the lighting computation. 

	create_world(); // create the world (scene) to be rendered in the window
}

void cgvInterface::init_rendering_loop() {
	glutMainLoop(); // initialize the visualization loop of OpenGL
}

void cgvInterface::create_menu() {
	int menu_id = glutCreateMenu(menuHandle);

	glutAddMenuEntry("Easy", 1);
	glutAddMenuEntry("Medium", 2);
	glutAddMenuEntry("Difficult", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


}
void cgvInterface::menuHandle(int value)
{

	switch (value) {
	case 1:
		interface.scene.difficulty = 0.002;
		break;
	case 2:
		interface.scene.difficulty = 0.004;
		break;
	case 3:
		interface.scene.difficulty = 0.006;
		break;
	}
	glutPostRedisplay(); // renew the content of the window
}



void cgvInterface::set_glutKeyboardFunc() {

	if (GetAsyncKeyState(0x57)) interface.scene.movePlayer1(0.0025);
	if (GetAsyncKeyState(0x49)) interface.scene.movePlayer2(0.0025);
	if (GetAsyncKeyState(0x53)) interface.scene.movePlayer1(-0.0025);
	if (GetAsyncKeyState(0x4B)) interface.scene.movePlayer2(-0.0025);
	//if (GetAsyncKeyState(0x41)) interface.scene.set_axes(interface.scene.get_axes() ? false : true);
	if (GetAsyncKeyState(VK_ESCAPE)) exit(1);
}

void cgvInterface::set_glutReshapeFunc(int w, int h) {
	// dimension of the viewport with a new width and a new height of the display window 

	// store the new values of the viewport and the display window. 
	interface.set_width_window(w);
	interface.set_height_window(h);

	// Set up the kind of projection to be used
	interface.camera.apply(interface.camType);

}

void cgvInterface::set_glutDisplayFunc() {


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window and the z-buffer

														// set up the viewport
	glViewport(0, 0, interface.get_width_window(), interface.get_height_window());

	// Apply the camera and projection transformations according to its parameters and to the mode (selection or visualization)
	interface.camera.apply(interface.camType);

	// Render the scene
	if (interface.scene.end == false) {
		interface.scene.render();
	}
	if (interface.scene.end == true && interface.scene.getScorep1()==3) {
		interface.camera= cgvCamera(cgvPoint3D(0, -100.0, 0), cgvPoint3D(5.5, 0, 5), cgvPoint3D(0, 0, 1),
			1 * 5, 1 * 5, 0.1, 200);
		interface.scene.render2();
	}
	if (interface.scene.end == true && interface.scene.getScorep2() == 3) {
		interface.camera = cgvCamera(cgvPoint3D(0, -100.0, 0), cgvPoint3D(5.5, 0, 5), cgvPoint3D(0, 0, 1),
			1 * 5, 1 * 5, 0.1, 200);
		interface.scene.render3();
	}


	glutSwapBuffers(); // it is used instead of glFlush(), to avoid flickering	
}

void cgvInterface::init_callbacks() {
	glutReshapeFunc(set_glutReshapeFunc);
	glutDisplayFunc(set_glutDisplayFunc);
	glutVisibilityFunc(interface.visible);
}

void cgvInterface::visible(int iVis)
{
	if (iVis == GLUT_VISIBLE)
		glutIdleFunc(interface.idle);
	else
		glutIdleFunc(NULL);
	return;
}

void cgvInterface::idle(void)
{
	set_glutKeyboardFunc();
	interface.scene.ballMovement();
	glutPostRedisplay();
}