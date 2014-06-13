#define _CRT_SECURE_NO_WARNINGS


#ifdef WIN32
	#include <Windows.h>
#endif
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctime>


#include "OpenGLincludes.h"
#include "Estruturas.h"
#include "QuadTree.h"
#include "Entity.h"
#include "Enemy.h"
#include "Utils.h"
#include "CollisionGrid.h"
//#include "SkyBox.h"
#include "math_3d.h"
#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "camera.h"
#include "GameManager.h"

using namespace std;

int width = 800;
int height = 600;
double midWindowX = width / 2;
double midWindowY = height / 2;

float Xposition = 500;
float Yposition = 40;
float Zposition = 500;

float Xrotation = 45;
float Yrotation = 180;
float Zrotation = 0;

float XRotationRadius;
float YRotationRadius;

bool functionKey = false;
bool keyStates[256];
bool rato[2];

bool WarpingMouse = false;
bool keySpecialStates[256];

bool first_person = true;
float cRadius = 200.0f; // our radius distance from our character

char s[100];
double t;

double currentTime, previousTime;
float fps;
int lastFps = 1;
float deltaTime = 0;

int frameCount = 0;
char texto[100];

int teclasEspeciais;

bool paused = true;

GLFWwindow* window;

GLuint SkyboxTexture[6];

Camera* m_pGameCamera;
PersProjInfo projeccao;

Player *player;
Camera *camera;
GameManager *gameManager;
Rect bounds = Rect(0, 0, 8192, 8192);

bool performance = true;

bool debugKeys[256];


void setOrthographicProjection() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	
}

void resetPerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}


GLfloat dark[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat normal[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat bright[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat pos2[4] = { 8192, 100.0, 8192, 1.0 };
GLfloat light1_ambient[] = { 0.8, 0.8, 0.8, 1.0 };

void Enables(void){

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);
// 	glFrontFace(GL_CW);
// 	glCullFace(GL_BACK);
// 	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, normal);
	glLightfv(GL_LIGHT1, GL_SPECULAR, normal);

	GLfloat mat_shininess = 5.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, normal);
	glMaterialfv(GL_FRONT, GL_AMBIENT, normal);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, normal);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	*/

	//glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient[0]);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, LightAmbient[0]);


	//glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 10);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LightAmbient[0]);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LightAmbient[0]);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LightAmbient[0]);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);


// 	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, bright);
// 
// 	glLightfv(GL_LIGHT0, GL_DIFFUSE, bright);
// 	glLightfv(GL_LIGHT0, GL_SPECULAR, bright);
 	//glLightfv(GL_LIGHT0, GL_POSITION, pos2);
// 
// 	glMaterialfv(GL_FRONT, GL_AMBIENT, bright);
// 	glMaterialfv(GL_FRONT, GL_DIFFUSE, dark);
// 	glMaterialfv(GL_FRONT, GL_SPECULAR, bright);
// 	glMateriali(GL_FRONT, GL_SHININESS, 128);

}

void Disables(void){

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);

}

void drawRectFloor(float x, float z, int w, int h, RGBf color){
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINE_STRIP);
		glVertex3f(x, 0.0f, z);
		glVertex3f(x, 0.0f, z + h);
		glVertex3f(x + w, 0.0f, z + h);
		glVertex3f(x + w, 0.0f, z);
		glVertex3f(x, 0.0f, z);
	glEnd();

}

void drawFilledRect(float x, float y, float z, int w, int h, RGBf color){
	
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + h);
	glVertex3f(x + w, y, z + h);
	glVertex3f(x + w, y, z);
	glEnd();

}

void DrawCube(float x, float y, float z){
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslated(x, y, z);
	glutSolidCube(5);
	glPopMatrix();
}

void DrawTree(QuadTree* tree)
{
	if (tree == NULL)
		return;

	drawRectFloor(tree->BoundingBox.x, tree->BoundingBox.y, tree->BoundingBox.w, tree->BoundingBox.h, RGBf(0.0, 1.0, 0.0));

	DrawTree(tree->northWest);
	DrawTree(tree->northEast);
	DrawTree(tree->southWest);
	DrawTree(tree->southEast);

}

void initGame(void){

	srand(time(NULL));

	
	player = new Player(XYZ(100, 0, 100));
	gameManager = new GameManager(bounds, player);

	player->setGrid(gameManager->collisionGrid);
	player->setQuadTree(gameManager->quad);

	camera = new Camera();
	camera->projeccao = projeccao;
	camera->player = player;

	player->camera = camera;

	char *texturas[6] = { "Content/SunSetRight2048.png", "Content/SunSetLeft2048.png", "Content/SunSetUp2048.png", "Content/SunSetDown2048.png", "Content/SunSetFront2048.png", "Content/SunSetBack2048.png" };
	
	for (int i = 0; i < 6; i++)
		loadTexture(texturas[i], 0, SkyboxTexture, i);
	
}

void DrawPlayer(){

	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0, -30, 0);
	glutSolidCube(40);
	glPopMatrix();

}




void renderBitmapString(float x, float y, const char *string, RGBf rgb){
	const char *c;
	glColor3f(rgb.r, rgb.g, rgb.b);
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);


}

char pos[100];
char updatet[100];

float updateT1, updateT2;

const char *vida = "Health: %.1f%%";
const char *vel =  "Speed:  %.1f";
const char *ataq = "Attack: %.1f";
const char *jmp =  "Jump:   %.1f";

const char *jde = "(Z) JDE: %d";
const char *ff = "(X) FF: %d";


void DrawHUD(){
	Disables();
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);
	
	//renderBitmapString(10, 20, texto, RGBf(1.0f, 0.0f, 0.0f));

	//sprintf(pos, "X: %.2f, Y: %.2f, Z: %.2f", player->getPos().x, player->getPos().y, player->getPos().z);
	//renderBitmapString(10, 40, pos, RGBf(1.0f, 0.0f, 0.0f));

	sprintf(updatet, "Update time: %.2f ms", updateT2 - updateT1);
	renderBitmapString(10, 40, updatet, RGBf(1.0f, 0.0f, 0.0f));

	sprintf(updatet, "Mission: %.2fs / %ds", gameManager->getMissionCompletedTime(), gameManager->missionTime);
	renderBitmapString(10, 60, updatet, RGBf(0.0f, 1.0f, 0.0f));

	sprintf(updatet, vida, player->getHealth());
	renderBitmapString(660, 530, updatet, RGBf(0.0f, 1.0f, 0.0f));

	sprintf(updatet, vel, player->BASE_MOVE_SPEED + player->speed_bonus);
	renderBitmapString(660, 550, updatet, RGBf(0.0f, 1.0f, 0.0f));

	sprintf(updatet, ataq, player->BASE_ATTACK_DAMAGE + player->attack_bonus);
	renderBitmapString(660, 570, updatet, RGBf(0.0f, 1.0f, 0.0f));

	sprintf(updatet, jmp, player->BASE_JUMP_V + player->jump_bonus);
	renderBitmapString(660, 590, updatet, RGBf(0.0f, 1.0f, 0.0f));

	sprintf(updatet, jde, player->itemInventory[JUMP_DROP_EXPLOSION]);
	renderBitmapString(10, 590, updatet, RGBf(1.0f, 1.0f, 0.0f));

	sprintf(updatet, ff, player->itemInventory[FORCE_FIELD]);
	renderBitmapString(110, 590, updatet, RGBf(1.0f, 1.0f, 0.0f));
	
	glPopMatrix();
	resetPerspectiveProjection();
}



void UpdateFPS(){
	currentTime = glfwGetTime();

	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	if (currentTime - lastFps > 1.0)	{

		sprintf(texto, "FPS: %d [Performance mode: %s]", frameCount, performance ? "on" : "off");

		glfwSetWindowTitle(window, texto);
		lastFps = currentTime;
		frameCount = 0;
	}
	frameCount++;

}



void updateGame(){

	updateT1 = glfwGetTime() * 1000;
	
	gameManager->update(deltaTime);
	

	updateT2 = glfwGetTime() * 1000;

}


void update(){

	UpdateFPS();


	if (!paused)
		updateGame();
	
	player->update(keyStates, deltaTime);
	
}


void drawCollisionGrid()
{
	GLfloat extent = gameManager->collisionGrid->boundingBox.w; // How far on the Z-Axis and X-Axis the ground extends
	GLfloat stepSize = gameManager->collisionGrid->cellSize;  // The size of the separation between points
	GLfloat groundLevel = -10.0f;
	
	glColor4f(1, 0, 0, 0.4);
	
// 	glBegin(GL_LINES);
// 	for (int i = 0; i < collisionGrid->gridCols; i++)
// 	{
// 		glVertex3f(i * stepSize, groundLevel, 0);
// 		glVertex3f(i * stepSize, groundLevel, extent);
// 
// 		glVertex3f(0, groundLevel, i * stepSize);
// 		glVertex3f(extent, groundLevel, i * stepSize);
// 
// 	}
// 	glEnd();
	
	for (int z = 0; z < gameManager->collisionGrid->gridRows; z++){
		for (int x = 0; x < gameManager->collisionGrid->gridCols; x++){
			if (gameManager->collisionGrid->getCell(x, z) == BLOCKED)
				drawFilledRect(x * gameManager->collisionGrid->cellSize, -10, z * gameManager->collisionGrid->cellSize, gameManager->collisionGrid->cellSize, gameManager->collisionGrid->cellSize, RGBf(1.0, 0.0, 0.0));
		}
	}

}


void drawSkybox(float x, float y, float z, float width, float height, float length)
{

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYFRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYBACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYLEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYRIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYUP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYDOWN]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

}

void debugRender(){
	if (debugKeys[GLFW_KEY_B]){
		DrawTree(gameManager->quad);
	}

	if (debugKeys[GLFW_KEY_N]){
		
	}
}

void render(){

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Enables();


	player->render();

	
	gameManager->DrawEntities();
 	drawFilledRect(0, -11, 0, 8192, 8192, RGBf(0.5, 0.5, 0.5));
 	drawCollisionGrid();


	gameManager->drawMission();

	debugRender();
	

	drawSkybox(0, 0, 0, 20000, 20000, 20000);


	// TODO desenhar palavras tem problemas de performance
	DrawHUD();
	
	glfwSwapBuffers(window);
}



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else{

		if (action == GLFW_PRESS)
			keyStates[key] = true;

		if (action == GLFW_RELEASE)
			keyStates[key] = false;

		if (key == GLFW_KEY_P && action == GLFW_RELEASE)
			paused = !paused;

		if (key == GLFW_KEY_1 && action == GLFW_RELEASE){	
			performance = !performance;
			gameManager->performance = !gameManager->performance;
		
		}

		if (key == GLFW_KEY_B && action == GLFW_RELEASE){
			// TODO draw quadtree
			debugKeys[GLFW_KEY_B] = !debugKeys[GLFW_KEY_B];
		}

		if (key == GLFW_KEY_N && action == GLFW_RELEASE){
			// TODO draw enemy LOS and collision circle
			debugKeys[GLFW_KEY_N] = !debugKeys[GLFW_KEY_N];
			gameManager->drawCollisionCircle = !gameManager->drawCollisionCircle;
			gameManager->drawLOS = !gameManager->drawLOS;
		}

// 		if (key == GLFW_KEY_K && action == GLFW_RELEASE)
// 			player->camera->firstPerson = !player->camera->firstPerson;

	}
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void initGL(){
	float ratio;
	ratio = width / (float)height;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, ratio, 1.0, 20000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/

	projeccao.FOV = 75;
	projeccao.Height = height;
	projeccao.Width = width;
	projeccao.zFar = 20000;
	projeccao.zNear = 1;

	// The following code is a fancy bit of math that is equivalent to calling:
	// gluPerspective(fieldOfView/2.0f, width/height , near, far);
	// We do it this way simply to avoid requiring glu.h
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat aspectRatio = (width > height) ? projeccao.Width / projeccao.Height : projeccao.Height / float(width);
	GLfloat fH = tan(float(projeccao.FOV / 360.0f * PI)) * projeccao.zNear;
	GLfloat fW = fH * aspectRatio;
	glFrustum(-fW, fW, -fH, fH, projeccao.zNear, projeccao.zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	

	

	//glfwSwapInterval(1); //vsync

	glEnable(GL_DEPTH_TEST); 
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	// TODO desactivado porque escurece o ecra
	glEnable(GL_TEXTURE_2D);


}

void mouseControl(GLFWwindow *win, double x, double y){
	camera->mouseControl(win, x, y);

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	
	//glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		cout << "glewInit error: " << glewGetErrorString(err) << endl;
		exit(EXIT_FAILURE);
	}

	glfwSetCursorPos(window, midWindowX, midWindowY);

	initGame();
	initGL();

	

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseControl);
	

	while (!glfwWindowShouldClose(window)){
		
		update();
		render();


		glfwPollEvents();
	}

	gameManager->entities.clear();
	gameManager->quad->clear();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}