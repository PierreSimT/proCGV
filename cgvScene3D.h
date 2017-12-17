#ifndef __CGVSCENE3D
#define __CGVSCENE3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include "cgvTexture.h"
#endif


class cgvScene3D {
	protected:

		float player1;
		float player2;

		double ballX;
		double ballY;

		double ballDirX;
		double ballDirY;
		int scoreP1;
		int scoreP2;
		int textureChosen;

		bool axes;

	public:

		void draw_quad(float div_x, float div_z, float _size_x, float _size_z);
		void drawBitmapText(char *string, float x, float y, float z);

		void drawRect(int x, int y, int width, int height);
		void drawNumber(int n,double x,double z);

		int textureChosen;
		double difficulty;
		// Default constructor and destructor
		cgvScene3D();
		~cgvScene3D();

		// Static methods

		// Methods
		// method with the OpenGL calls to render the scene
    void render();


	void drawPlayer1();
	void drawPlayer2();
	void drawBall();

	void movePlayer1(float value);
	void movePlayer2(float value);
	void ballMovement();
	////// Section D: include here the method/s to indicate the selected object

		bool get_axes() {return axes;};
		void set_axes(bool _axes){axes = _axes;};
};

#endif
