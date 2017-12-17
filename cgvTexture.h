#ifndef __CGVTEXTURE
#define __CGVTEXTURE

#include <GL/glut.h>
#include <stdio.h>

#include "bmp.h"

class cgvTexture {

	protected:
		// Attributes
		unsigned int idTexture; // texture identifier
		unsigned int height,      // height of the texture
			          width;     // weight of the texture

		
	public:
		// Default Constructors and destructor
		cgvTexture(char *file); // Texture from a BMP file
		~cgvTexture();

		// Methods
		void apply(void); // bind the texture as the active texture
};

#endif

