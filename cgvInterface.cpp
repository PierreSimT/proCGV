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
	glutAddMenuEntry("No Texture", 4);
	glutAddMenuEntry("Field Texture", 5);
	glutAddMenuEntry("Sand Texture", 6);

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
	case 4:
		interface.scene.textureChosen = 0;
		break;
	case 5:
		interface.scene.textureChosen = 1;
		break;
	case 6:
		interface.scene.textureChosen = 2;
	}
	glutPostRedisplay(); // renew the content of the window
}



void cgvInterface::set_glutKeyboardFunc() {

	if (GetAsyncKeyState(0x57)) interface.scene.movePlayer1(0.0025);
	if (GetAsyncKeyState(0x49)) interface.scene.movePlayer2(0.0025);
	if (GetAsyncKeyState(0x53)) interface.scene.movePlayer1(-0.0025);
	if (GetAsyncKeyState(0x4B)) interface.scene.movePlayer2(-0.0025);
	if (GetAsyncKeyState(0x41)) interface.scene.set_axes(interface.scene.get_axes() ? false : true);
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
	GLuint impact_list[1024]; // array with the list of impacts when it is rendered in selection mode


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window and the z-buffer

														// set up the viewport
	glViewport(0, 0, interface.get_width_window(), interface.get_height_window());

	// Section D: check the mode before applying the camera and projection transformations,
	if (interface.mode == CGV_SELECT) {
		// Section D: if it is in the OpenGL selection mode pass the corresponding parameters to the camera
		interface.init_selection(1024, impact_list);
	}
	// Apply the camera and projection transformations according to its parameters and to the mode (selection or visualization)
	interface.camera.apply(interface.camType);

	// Render the scene
	interface.scene.render();

	if (interface.mode == CGV_SELECT) {
		// Section D: exit the selection mode and process the list of impacts
		interface.finish_selection(1024, impact_list);
		glutPostRedisplay();
	}
	else {
		// refresh the window
		glutSwapBuffers(); // it is used instead of glFlush(), to avoid flickering
	}
}

void cgvInterface::set_glutMouseFunc(GLint button, GLint state, GLint x, GLint y) {

	// TODO: Section D: check if the left button of the mouse has been clicked. See glutMouseFunc for details.  

	if (button == GLUT_LEFT_BUTTON)
		interface.pressed_button = true;
	// Section D: Store the button that has been pressed or released. If it has been clicked, then change to selection mode (CGV_SELECT)

	if (interface.pressed_button && state == GLUT_DOWN)
		interface.mode = CGV_SELECT;

	// Section D: Save the position of the pixel when the mouse was clicked

	interface.cursorX = x;
	interface.cursorY = y;

	glutPostRedisplay();


}

void cgvInterface::set_glutMotionFunc(GLint x, GLint y) {

	// TODO: Section E: if the mouse button is pressed and there is a selected object, then check the selected object 
	// and the position of the mouse to update the corresponding degree of freedom of the model accordingly 

	// Section E: Save the new position of the mouse

	// Section E: Redraw the content of the display window 

}

void cgvInterface::init_callbacks() {
	glutReshapeFunc(set_glutReshapeFunc);
	glutDisplayFunc(set_glutDisplayFunc);
	glutMouseFunc(set_glutMouseFunc);
	glutMotionFunc(set_glutMotionFunc);

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
	if (interface.scene.play)
		interface.scene.ballMovement();
	glutPostRedisplay();
}

void cgvInterface::init_selection(int SIZE_IMPACT_LIST, GLuint *impact_list) {
	// TODO: Section D: select the variable where the impacts will be stored. Use glSelectBuffer
	// Section D: Change to the OpenGL selection mode. See function glRenderMode
	// Section D: set the camera to selection mode with the required parameters to apply it
	// including the height and width of the selection area. 
	// Prove several alternatives to test the right size of this small area.  

	interface.mode = CGV_SELECT;
	glSelectBuffer(SIZE_IMPACT_LIST, impact_list);

	glRenderMode(GL_SELECT);

	interface.camera.setSelection(2, 2
		, interface.cursorX, interface.cursorY);
	interface.camera.apply(interface.camType);

}

int process_impacts(int num_impacts, GLuint *impact_list) {
	/* TODO: Section D: this function should return the code of the selected object, that can be different of the name assigned in the stack of names,
	If hierarchical names have used it has to be taken into account in this function to return a single code */

	// Section D: traverse the list of impacts with num_impacts impacts,
	// save the nearer to the observer (minimum Z)
	// At the beginning consider that minimum Z has a value of 0xffffffff (the maximum value represented by type GLuint)

	GLuint minimum = 0xffffffff;
	int name = -1;
	for (int i = 0; i < num_impacts; i++) {
		int numberNames = impact_list[i++];
		int minZ = impact_list[i++];
		int maxZ = impact_list[i++];
		if (minZ < minimum) {
			minimum = minZ;
			name = impact_list[i];
		}
		i += numberNames;
	}

	// Section D: From the information of the impact with a minimum Z, return the code corresponding to this object: as the scene 
	// is not saved in any data structure, to return the selected object use the names directly assigned to the objects 

	return name;
}

void cgvInterface::finish_selection(int SIZE_IMPACT_LIST, GLuint *impact_list) {

	// TODO 
	// Section D: Change to the OpenGL visualization mode and get the number of impacts  

	interface.mode = CGV_VISUALIZE;
	int numImpacts = glRenderMode(GL_RENDER);

	// Section D: if there are impacts, process them with the function process_impacts(int num_impacts, GLuint *impact_list);
	// obtaining the selected object, if any

	if (numImpacts > 0) {
		interface.selected_object = process_impacts(numImpacts, impact_list);
		interface.scene.selectedObject(interface.selected_object);
	}
	else {
		interface.scene.selectedObject(-1);
	}

	// Section D: the selection has finished, change to normal visualization

	interface.mode = CGV_VISUALIZE;

	// Section D: set up the camera with the parameters of the visualization mode

	interface.camera.setVisualization();
	interface.camera.apply(interface.camType);
}

