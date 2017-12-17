#include <cstdlib>
#include <stdio.h>
#include <sstream>

#include "cgvScene3D.h"

#include <iostream>

// Constructor methods -----------------------------------

GLfloat colour1[] = { 0.0f ,0.7f, 0.0f  };
GLfloat blue[] = { 0,0,1 };
GLfloat red[] = { 1,0,0 };
GLfloat white[] = { 1, 1, 1 };
GLfloat black[] = { 0,0,0 };
GLfloat c2[] = { 0.0f ,0.2f, 0.4f };

cgvScene3D::cgvScene3D(){
	axes = false;
	// Section B: initialize the attributes to control the degrees of freedom of the model
	play = false;
	scoreP1 = 0;
	scoreP2 = 0;

	ballDirX = 1;
	ballDirY = 0.0;

	ballY = 0;
	ballX = 0;

	player1 = 0;
	player2 = 0;

	difficulty = 0.002;
	textureChosen = 0;
	end = false;
	
	// Section D: initialize the attribute/s that identifies the select object and to colour it yellow
	nameSelected = -1;
}

cgvScene3D::~cgvScene3D() {}


// Public methods ----------------------------------------

void draw_axes(void) {
	GLfloat red[] = { 1,0,0,1.0 };
	GLfloat green[] = { 0,1,0,1.0 };
	GLfloat blue[] = { 0,0,1,1.0 };

	glBegin(GL_LINES);
	glMaterialfv(GL_FRONT, GL_EMISSION, red);
	glVertex3f(1000, 0, 0);
	glVertex3f(-1000, 0, 0);

	glMaterialfv(GL_FRONT, GL_EMISSION, green);
	glVertex3f(0, 1000, 0);
	glVertex3f(0, -1000, 0);

	glMaterialfv(GL_FRONT, GL_EMISSION, blue);
	glVertex3f(0, 0, 1000);
	glVertex3f(0, 0, -1000);
	glEnd();
}

void cgvScene3D::drawPlayer1() {

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, red);
	glTranslatef(0, 0, player1);
	glTranslatef(4.5, 0, 0);
	glScalef(0.25, 0.25, 2);
	glutSolidCube(1);
	glPopMatrix();

}


void cgvScene3D::drawPlayer2() {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, blue);
	glTranslatef(0, 0, player2);
	glTranslatef(-4.5, 0, 0);
	glScalef(0.25, 0.25, 2);
	glutSolidCube(1);
	glPopMatrix();
}

void cgvScene3D::drawBall() {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, white);
	glTranslatef(ballX, 0, ballY);
	glutSolidCube(0.25);
	glPopMatrix();
}


void cgvScene3D::render(void) {



	// lights
	GLfloat light0[4] = { 5.0,5.0,5.0,1 }; // point light source  
	glLightfv(GL_LIGHT0, GL_POSITION, light0); // this light is placed here and it remains still 
	glEnable(GL_LIGHT0);


	// create the model

	glPushMatrix(); // store the model matrices

	// draw the axes (disabled from start)
	if (axes) draw_axes();

	//glLightfv(GL_LIGHT0,GL_POSITION,light0); // the light is placed here and it moves with the scene
	float mesh_color[4] = { 1.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, mesh_color);

	if (nameSelected == 1)
		play = true;

	if (!play && nameSelected == -1) {
		glPushMatrix();
		glPushName(1);
		glTranslatef(-2.5, 0, -2.5);
		cgvTexture play("start.bmp");
		draw_quad(50, 50);
		glPopName();
		glPopMatrix();
	}
	else {

		//Top border
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);
		glTranslatef(0, 0, 4);
		glScalef(20, 0.5, 0.25);
		glutSolidCube(1);
		glPopMatrix();

		//Bottom border
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);
		glTranslatef(0, 0, -5);
		glScalef(20, 0.5, 0.5);
		glutSolidCube(1);
		glPopMatrix();

		//Delimiter
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);
		glTranslatef(0, 0, 5);
		glScalef(0.25, 0.5, 2);
		glutSolidCube(1);
		glPopMatrix();

		drawNumber(scoreP1, 1, 4.4);
		drawNumber(scoreP2, -1.5, 4.4);

		if (scoreP1 == 3) {
			end = true;
			difficulty = 0;
		}
		else if (scoreP2 == 3) {
			end = true;
			difficulty = 0;
		};

		drawPlayer1();
		drawPlayer2();
		if (!end) {
			drawBall();
		}
	}

	glPopMatrix(); // restore the modelview matrix 

}

void cgvScene3D::render2(void) {
	glScalef(2, 1, 2);
	cgvTexture txt("player1.bmp");
	draw_quad(50, 50);
}

void cgvScene3D::render3(void) {
	glScalef(2, 1, 2);
	cgvTexture txt("player2.bmp");
	draw_quad(50, 50);
}


void cgvScene3D::movePlayer1(float value) {
	if (value > 0 && player1 < 3) {
		player1 += value;
	}
	if (value < 0 && player1 > -4) {
		player1 += value;
	}
}


void cgvScene3D::movePlayer2(float value) {
	if (value > 0 && player2 < 3) {
		player2 += value;
	}
	if (value < 0 && player2 > -4) {
		player2 += value;
	}
}

void cgvScene3D::ballMovement() {
	ballX += ballDirX * difficulty;
	ballY += ballDirY * difficulty;

	float racket_width = 0.25;
	float racket_height = 1.1;

	// hit by left racket?
	if (ballX < -4.5 + racket_width &&
		ballX > -4.5 &&
		ballY < player2 + racket_height &&
		ballY > player2 - racket_height) {
		// set fly direction depending on where it hit the racket
		// (t is 0.02 if hit at top, 0 at center, -0.02 at bottom)
		float t = ((ballY - player2) / (racket_height)) - 0.02;

		ballDirX = fabs(ballDirX); // force it to be positive
		ballDirY = t;
	}

	// hit by right racket?
	if (ballX < 4.5 &&
		ballX > 4.5 - racket_width &&
		ballY < player1 + racket_height &&
		ballY > player1 - racket_height) {
		// set fly direction depending on where it hit the racket
		// (t is 0.02 if hit at top, 0 at center, -0.02 at bottom)
		float t = ((ballY - player1) / (racket_height * 2)) - 0.02;
		ballDirX = -fabs(ballDirX); // force it to be negative
		ballDirY = t;
	}

	// hit left wall?
	if (ballX < -5) {
		++scoreP1;
		ballX = 0;
		ballY = 0;
		ballDirX = fabs(ballDirX); // force it to be positive
		ballDirY = 0;
	}

	// hit right wall?
	if (ballX > 5) {
		++scoreP2;
		ballX = 0;
		ballY = 0;
		ballDirX = -fabs(ballDirX); // force it to be negative
		ballDirY = 0;
	}

	// hit top wall?
	if (ballY > 3.7) {
		ballDirY = -0.3; // force it to be negative
	}

	// hit bottom wall?
	if (ballY < -4.7) {
		ballDirY = 0.3; // force it to be positive
	}

}

void  cgvScene3D::drawNumber(int n, double x, double z) {

	switch (n) {

	case 0:
		glPushMatrix();

		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z);

		glVertex3f(x, 0, z + 0.1);

		glVertex3f(x + 0.5, 0, z + 0.1);

		glVertex3f(x + 0.5, 0, z);

		glEnd();
		//--------
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x + 0.5, 0, z + 0.1);

		glVertex3f(x + 0.5, 0, z + 0.5);

		glVertex3f(x + 0.4, 0, z + 0.5);

		glVertex3f(x + 0.4, 0, z + 0.1);

		glEnd();
		//-------
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x + 0.4, 0, z + 0.5);

		glVertex3f(x + 0, 0, z + 0.5);

		glVertex3f(x + 0., 0, z + 0.4);

		glVertex3f(x + 0.4, 0, z + 0.4);

		glEnd();

		//-------
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z + 0.5);

		glVertex3f(x, 0, z + 0.1);

		glVertex3f(x + 0.1, 0, z + 0.1);

		glVertex3f(x + 0.1, 0, z + 0.5);

		glEnd();


		glPopMatrix();
		break;

	case 1:

		glPushMatrix();
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z);

		glVertex3f(x + 0.1, 0, z);

		glVertex3f(x + 0.1, 0, z + 0.5);

		glVertex3f(x, 0, z + 0.5);

		glEnd();


		glPopMatrix();

		break;
	case 2:
		glPushMatrix();

		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z + 0.15);

		glVertex3f(x, 0, z + 0.25);

		glVertex3f(x + 0.5, 0, z + 0.25);

		glVertex3f(x + 0.5, 0, z + 0.15);

		glEnd();
		//-----
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x + 0.5, 0, z + 0.5);

		glVertex3f(x + 0, 0, z + 0.5);

		glVertex3f(x + 0., 0, z + 0.4);

		glVertex3f(x + 0.5, 0, z + 0.4);

		glEnd();

		//-------
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z + 0.5);

		glVertex3f(x, 0, z + 0.25);

		glVertex3f(x + 0.1, 0, z + 0.25);

		glVertex3f(x + 0.1, 0, z + 0.5);

		glEnd();
		//--------
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x + 0.5, 0, z + 0.15);

		glVertex3f(x + 0.5, 0, z - 0.15);

		glVertex3f(x + 0.4, 0, z - 0.15);

		glVertex3f(x + 0.4, 0, z + 0.15);

		glEnd();
		//-------
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z - 0.15);

		glVertex3f(x, 0, z - 0.05);

		glVertex3f(x + 0.5, 0, z - 0.05);

		glVertex3f(x + 0.5, 0, z - 0.15);

		glEnd();

		break;

	case 3:
		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z + 0.5);

		glVertex3f(x, 0, z - 0.1);

		glVertex3f(x + 0.1, 0, z - 0.1);

		glVertex3f(x + 0.1, 0, z + 0.5);

		glEnd();

		//------

		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z + 0.5);

		glVertex3f(x, 0, z + 0.4);

		glVertex3f(x + 0.5, 0, z + 0.4);

		glVertex3f(x + 0.5, 0, z + 0.5);

		glEnd();
		//------

		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z + 0.25);

		glVertex3f(x, 0, z + 0.15);

		glVertex3f(x + 0.5, 0, z + 0.15);

		glVertex3f(x + 0.5, 0, z + 0.25);

		glEnd();
		//------

		glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_EMISSION, colour1);

		glVertex3f(x, 0, z);

		glVertex3f(x, 0, z - 0.1);

		glVertex3f(x + 0.5, 0, z - 0.1);

		glVertex3f(x + 0.5, 0, z);

		glEnd();
		break;
	}

}

void cgvScene3D::selectedObject(int name) {
	nameSelected = name;
}

void cgvScene3D::draw_quad(float div_x, float div_z) {
	float ini_x = 0.0;
	float ini_z = 0.0;
	float size_x = 5.0;
	float size_z = 5.0;

	glNormal3f(0, 1, 0);

	float jumpX = size_x / div_x;
	float jumpZ = size_z / div_z;

	float xCounter = 0.0;
	float zCounter = 0.0;

	float textureTJump = 1 / div_x;
	float textureSJump = 1 / div_z;

	float textureTCounter = 0.0;
	float textureSCounter = 0.0;

	for (int i = 0; i < div_x; i++) {
		for (int j = 0; j < div_z; j++) {
			glBegin(GL_QUADS);
			glTexCoord2f(textureSCounter, textureTCounter);
			glVertex3f(xCounter, 0.0, zCounter);

			glTexCoord2f(textureSCounter, textureTCounter + textureTJump);
			glVertex3f(xCounter, 0.0, zCounter + jumpZ);

			glTexCoord2f(textureSCounter + textureSJump, textureTCounter + textureTJump);
			glVertex3f(xCounter + jumpX, 0.0, zCounter + jumpZ);

			glTexCoord2f(textureSCounter + textureSJump, textureTCounter);
			glVertex3f(xCounter + jumpX, 0.0, zCounter);

			glEnd();
			textureTCounter += textureTJump;
			zCounter += jumpZ;
		}
		textureTCounter = 0.0;
		zCounter = 0.0;
		textureSCounter += textureSJump;
		xCounter += jumpX;
	}

}