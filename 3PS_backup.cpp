#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <stdlib.h>
#include <ctime>
#include <boost/thread/thread.hpp>

#include "OpenGLincludes.h"
#include "Estruturas.h"
#include "QuadTree.h"
#include "Entity.h"
#include "Utils.h"

using namespace std;

int width = 800;
int height = 600;

float Xposition = 500;
float Yposition = 40;
float Zposition = 500;

float Xrotation = 45;
float Yrotation = 180;
float Zrotation = 0;

float XRotationRadius;
float YRotationRadius;

float speedBoost = 0.0f;
float MoveVelocity = 0.35f;
float MouseVelocity = 0.30f;

float CubePositionsX[20];
float CubePositionsZ[20];


bool functionKey = false;
bool keyStates[256];
bool rato[2];

bool WarpingMouse = false;
bool keySpecialStates[256];

bool first_person = true;
float cRadius = 100.0f; // our radius distance from our character

char s[30];
double t;

int currentTime, previousTime;
float fps;
int lastFps = 1;
int deltaTime = 0;

int frameCount = 0;
char texto[20];

float Pi = 3.141592654f;

int teclasEspeciais;

QuadTree *quad;
vector<Entity*> entities;
bool paused = true;


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

void Enables(void){

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

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

void CubePositions(void){

	for (int i = 0; i < 20; i++){

		CubePositionsX[i] = 10 * i;
		CubePositionsZ[i] = 0;

	}

}

void init(void){

	glClearColor(0.0, 0.0, 0.0, 0.0);

	srand(time(NULL));

	entities.resize(200);

	CubePositions();
	//glutSetCursor(GLUT_CURSOR_NONE);

	quad = new QuadTree(Rect(0, 0, 8192, 8192));
	quad->clear();

	Entity *e;

	for (int i = 0; i < 200; i++){
		e = new Entity(rand() % quad->BoundingBox.w, rand() % quad->BoundingBox.h, quad);
		entities[i] = e;
	}

	for (int i = 0; i < entities.size(); i++)
		quad->insert(entities[i]);



}

void DrawCubes(void) {

	for (int i = 0; i < 20; i++){

		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslated(-CubePositionsX[i + 1] * 10, 0, -CubePositionsZ[i + 1] * 10);
		glutSolidCube(40);
		glPopMatrix();

	}

}

void DrawEntities() {

	for (int i = 0; i < entities.size(); i++){
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslated(entities[i]->pos.x, 0, entities[i]->pos.y);
		glutSolidCube(20);
		glPopMatrix();

		//drawRectFloor(entities[i]->repelF.x, entities[i]->repelF.y, entities[i]->repelF.w, entities[i]->repelF.h, RGBf(1.0, 0.0, 0.0));
	}

}

void DrawPlayer(){

	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidCube(20);

}

void moveCamera(void) {

	YRotationRadius = (Yrotation / 180 * Pi);

	if (keyStates['w']){
		XRotationRadius = (Xrotation / 180 * Pi);
		Xposition += float(sin(YRotationRadius)) * (MoveVelocity + speedBoost) * deltaTime;
		Zposition -= float(cos(YRotationRadius)) * (MoveVelocity + speedBoost) * deltaTime;
	}

	if (keyStates['s']){
		XRotationRadius = (Xrotation / 180 * Pi);
		Xposition -= float(sin(YRotationRadius)) * (MoveVelocity + speedBoost) * deltaTime;
		Zposition += float(cos(YRotationRadius)) * (MoveVelocity + speedBoost) * deltaTime;
	}

	if (keyStates['a']){
		Xposition -= float(cos(YRotationRadius)) * (MoveVelocity + speedBoost) * deltaTime;
		Zposition -= float(sin(YRotationRadius)) * (MoveVelocity + speedBoost) * deltaTime;
	}

	if (keyStates['d']){
		Xposition += float(cos(YRotationRadius)) * (MoveVelocity + speedBoost) * deltaTime;
		Zposition += float(sin(YRotationRadius)) * (MoveVelocity + speedBoost) * deltaTime;
	}

	if (keyStates['e'])
		Yposition += MoveVelocity * deltaTime;

	if (keyStates['q'])
		Yposition -= MoveVelocity * deltaTime;


}


void renderBitmapString(float x, float y, const char *string, RGBf rgb){
	const char *c;
	glColor3f(rgb.r, rgb.g, rgb.b);
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);


}

char pos[60];
char updatet[40];

int updateT1, updateT2;

void DrawHUD(){
	Disables();
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);
	
	renderBitmapString(10, 20, texto, RGBf(1.0f, 0.0f, 0.0f));

	sprintf(pos, "X: %.2f, Y: %.2f, Z: %.2f", Xposition, Yposition, Zposition);
	renderBitmapString(10, 40, pos, RGBf(1.0f, 0.0f, 0.0f));

	sprintf(pos, "xRot: %.2f, yRot: %.2f, zRot: %.2f", Xrotation, Yrotation, Zrotation);
	renderBitmapString(10, 60, pos, RGBf(1.0f, 0.0f, 0.0f));

	sprintf(updatet, "Update time: %d ms", updateT2 - updateT1);
	renderBitmapString(10, 80, updatet, RGBf(1.0f, 0.0f, 0.0f));
	
	glPopMatrix();
	resetPerspectiveProjection();
}



void UpdateFPS(){
	currentTime = glutGet(GLUT_ELAPSED_TIME);

	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	if (currentTime - lastFps > 1000)	{
		sprintf(texto, "FPS: %d", frameCount);
		lastFps = currentTime;
		frameCount = 0;
	}
	frameCount++;

}



void updateGame(){

	
	updateT1 = glutGet(GLUT_ELAPSED_TIME);
	quad->clear();

	for (int i = 0; i < entities.size(); i++){
		quad->insert(entities[i]);
	}
	

	for (int i = 0; i < entities.size(); i++)
		entities[i]->update(XY(Xposition, Zposition), deltaTime);


	updateT2 = glutGet(GLUT_ELAPSED_TIME);

}


void keyUpdate(){
	if (keyStates['f']){

	}
}

void update(){

	keyUpdate();

	UpdateFPS();

	if (!paused)
		updateGame();

	moveCamera();

}

void updateThread(){
	cout << "Ca estou eu" << endl;

	while (true){
		UpdateFPS();

		if (!paused)
			updateGame();

		moveCamera();
	}
}


void render(){

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (first_person){
		glRotatef(Xrotation, 1.0, 0.0, 0.0);
		glRotatef(Yrotation, 0.0, 1.0, 0.0);
	}
	else{
		glTranslatef(0.0f, -20.0f, -cRadius);
		glRotatef(Xrotation, 1.0, 0.0, 0.0);
		DrawPlayer();
		glRotatef(Yrotation, 0.0, 1.0, 0.0);
	}

	glTranslated(-Xposition, -Yposition, -Zposition);

	Enables();
	DrawCubes();
	
	//DrawTree(quad);

	/*
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < entities.size(); i++)
		glVertex3f(entities[i]->pos.x, 0.0f, entities[i]->pos.y);
	glEnd();
	*/

	DrawEntities();

	DrawHUD();
	
	glutSwapBuffers();
}


void display(void){

	update();
	render();
	glutPostRedisplay();
}

void Reshape(int w, int h){

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 20000.0);
	glMatrixMode(GL_MODELVIEW);

}

void NormalKeyPress(unsigned char key, int x, int y){

	keyStates[key] = true;

	switch (key) {
		case 'p':
			paused = !paused;
			break;

		case 'f':
			functionKey = true;
			break;

		case 27:
			entities.clear();
			quad->clear();
			exit(0);
			break;
	}

	glutPostRedisplay();

}

void NormalKeyRelease(unsigned char key, int x, int y) {

	keyStates[key] = false;

	switch (key) {
		case 'f':
			functionKey = false;
			break;
	}
	glutPostRedisplay();

}

void PassiveMouseMotion(int x, int y){

	int CenterX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int CenterY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	int DiffX = x - CenterX;
	int DiffY = y - CenterY;

	if (!WarpingMouse) {

		WarpingMouse = true;

		Xrotation += (float)DiffY * MouseVelocity;
		Yrotation += (float)DiffX * MouseVelocity;

		if (Xrotation > 90) Xrotation = 90;
		if (Xrotation < -90) Xrotation = -90;

		glutWarpPointer(CenterX, CenterY);
		
	}
	else {
		WarpingMouse = false;
	}

	glutPostRedisplay();

}

void update(int value){
	t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	int time = (int)t;
	sprintf(s, "TIME : %2d Sec", time);
	glutTimerFunc(1000, update, 0);
	glutPostRedisplay();
}

void keySpecial(int key, int x, int y) {
	keySpecialStates[key] = true;
	glutPostRedisplay();
}

void keySpecialUp(int key, int x, int y) {
	keySpecialStates[key] = false;
	glutPostRedisplay();
}

void windowMouse(int btn, int state, int x, int y)
{
	/*
	if (btn == GLUT_LEFT_BUTTON)
		mouseBtnPressed = "Left";
	else if (btn == GLUT_RIGHT_BUTTON)
		mouseBtnPressed = "Right";
	*/
	if (state == GLUT_DOWN)
		rato[btn] = true;
	else if (state == GLUT_UP)
		rato[btn] = false;

	glutPostRedisplay();
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Blocks!");

	init();

	//boost::thread t1(updateThread);

	glutDisplayFunc(display);
	//glutIdleFunc(display);
	glutReshapeFunc(Reshape);


	glutKeyboardFunc(NormalKeyPress);
	glutKeyboardUpFunc(NormalKeyRelease);
	glutSpecialFunc(keySpecial);
	glutSpecialUpFunc(keySpecialUp);
	glutMouseFunc(windowMouse);
	glutPassiveMotionFunc(PassiveMouseMotion);

	

	//glutTimerFunc(25, update, 0);
	glutMainLoop();


	

	return 0;

}