#include "cgvTexture.h"

// Constructor and destructor methods

cgvTexture::cgvTexture(char *file) {
	unsigned char *image; 
	unsigned int width, height;

	// load the BMP image in memory
	image = loadBMPRaw(file, width, height, false);

	/* TODO: Section H: Add here the code to load the BMP image as a texture */
	/*
	- Enable GL_TEXTURE_2D
	- Generate the identifier of the texture and assign it to the attribute idTexture
	- Application mode of the texture
	- Parameters of the texture: repeat and filters
	- Specify the texture assigning it the image array.
	*/

	//1
	glEnable(GL_TEXTURE_2D);
	//2
	glGenTextures(1, &idTexture);
	//3
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//4
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//5
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image);
	



	delete[] image;

	
}

void cgvTexture::apply(void) {
  glBindTexture(GL_TEXTURE_2D, idTexture);
}

cgvTexture::~cgvTexture() {
  glDeleteTextures(1, &idTexture); 
  glDisable(GL_TEXTURE_2D);
}
