#include "OpenGLincludes.h"

#include "camera.h"
#include "Utils.h"


Camera::Camera(){
	projeccao.FOV = 75;
	projeccao.Height = 600;
	projeccao.Width = 800;
	projeccao.zFar = 20000;
	projeccao.zNear = 1;
	projeccao.aspect = (width > height) ? projeccao.Width / projeccao.Height : projeccao.Height / projeccao.Width;

}

void Camera::render(){

	if (firstPerson){
		glRotatef(xRotation, 1.0, 0.0, 0.0);
		glRotatef(yRotation, 0.0, 1.0, 0.0);
		player->render();
	}
	else{
		glTranslatef(0.0f, 20.0f, -radius);
		glRotatef(xRotation, 1.0, 0.0, 0.0);
		player->render();
		glRotatef(yRotation, 0.0, 1.0, 0.0);
	}

	glTranslatef(-player->getPos().x, -player->getPos().y - HEAD_HEIGHT, -player->getPos().z);
}

void Camera::mouseControl(GLFWwindow *win, double x, double y){

	int DiffX = x - midWindowX;
	int DiffY = y - midWindowY;

	if (!warpingMouse) {

		warpingMouse = true;

		xRotation += (float)DiffY * MOUSE_VELOCITY;
		yRotation += (float)DiffX * MOUSE_VELOCITY;

		if (xRotation > 90) xRotation = 90;
		if (xRotation < -90) xRotation = -90;

		glfwSetCursorPos(win, midWindowX, midWindowY);

	}
	else {
		warpingMouse = false;
	}


}
