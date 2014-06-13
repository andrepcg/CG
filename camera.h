#ifndef CAMERA_H
#define	CAMERA_H

#include "Estruturas.h"
#include "math_3d.h"

class Player;

#include "Player.h"

class Camera
{
public:
	Camera();
	~Camera(void);
	void update();
	void mouseControl(GLFWwindow *win, double x, double y);
	void keyboardControl(unsigned char key, int x, int y);
	void render();

	PersProjInfo projeccao;

	Player *player;

	float xRotation = 45;
	float yRotation = 180;
	float angle;
	float radius = 100;

	int width = 800;
	int height = 600;
	double midWindowX = width / 2;
	double midWindowY = height / 2;

	bool firstPerson = true;

	const float HEAD_HEIGHT = 40.0f;
	const float MOUSE_VELOCITY = 0.30f;

private:
	
	bool warpingMouse;


};


#endif	/* CAMERA_H */

