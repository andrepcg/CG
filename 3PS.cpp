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
#include "Model.h"

using namespace std;

int width = 800;
int height = 600;
double midWindowX = width / 2;
double midWindowY = height / 2;


bool functionKey = false;
bool keyStates[512];
bool rato[2];

bool WarpingMouse = false;

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

GLuint texturas[6];

Camera* m_pGameCamera;
PersProjInfo projeccao;

Player *player;
Camera *camera;
GameManager *gameManager;
Rect bounds = Rect(0, 0, 8192, 8192);

bool performance = true;

bool debugKeys[256];


mar::Model *treeModel;
std::vector<XY> crates;

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
GLfloat sunPos[4] = { -1000, 500.0, -100, 0.0 };
GLfloat light1_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat lampPos[] = { 100, 100, 100, 1 };
GLfloat lampCorAmb[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat lampCorEsp[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lampCorDif[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat sunsetColor[4] = { 0.98, 0.834, 0.647, 1.0 };

const int groundTesselation = 128;

void Enables(void){

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);

 	glEnable(GL_CULL_FACE);

 	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, sunsetColor);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lampCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lampCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lampCorEsp);
// 	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
// 	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
// 	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);





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

void DrawQuadTree(QuadTree* tree)
{
	if (tree == NULL)
		return;

	drawRectFloor(tree->BoundingBox.x, tree->BoundingBox.y, tree->BoundingBox.w, tree->BoundingBox.h, RGBf(0.0, 1.0, 0.0));

	DrawQuadTree(tree->northWest);
	DrawQuadTree(tree->northEast);
	DrawQuadTree(tree->southWest);
	DrawQuadTree(tree->southEast);

}

vector<Tree> trees;
Mesh *arvore;
float terrainHeight[groundTesselation][groundTesselation];

void initGame(void){

	srand(time(NULL));

	for (int z = 0; z < groundTesselation; z++){
		for (int x = 0; x < groundTesselation; x++){
			terrainHeight[z][x] = (rand() / (float)RAND_MAX) * -10;
		}
	}

	
	player = new Player(XYZ(100, 0, 100));
	gameManager = new GameManager(bounds, player);

	player->setGrid(gameManager->collisionGrid);
	player->setQuadTree(gameManager->quad);

	camera = new Camera();
	camera->projeccao = projeccao;
	camera->player = player;

	player->camera = camera;

	char *nomesTex[] = { "Content/SunSetRight2048.png", "Content/SunSetLeft2048.png", "Content/SunSetUp2048.png", "Content/SunSetDown2048.png", "Content/SunSetFront2048.png", "Content/SunSetBack2048.png", "Content/crate.jpg", "Content/sand.jpg" };
	
	for (int i = 0; i < 8; i++)
		loadTexture(nomesTex[i], 0, texturas, i);

	treeModel = new mar::Model();
	treeModel->load("Content/models/", "arvore.obj", "arvore.mtl");

	arvore = new Mesh();
	arvore->LoadMesh("Content/models/arvore.obj");



	for (int i = 0; i < 100; i++){
		int x = randInt(0, gameManager->collisionGrid->gridCols);
		int z = randInt(0, gameManager->collisionGrid->gridRows);
		int ang = randInt(0, 180);
		trees.push_back(Tree(x * gameManager->collisionGrid->cellSize + gameManager->collisionGrid->cellSize / 2.0, z * gameManager->collisionGrid->cellSize + gameManager->collisionGrid->cellSize / 2.0, ang));
		gameManager->collisionGrid->setCell(x, z, BLOCKED);
	}

// 	for (int z = 0; z < bounds.h; z++){
// 		for (int x = 0; x < bounds.w; x++){
// 			float r = randInt(0, )
// 			if (r < 0.05)
// 				trees.push_back(Tree(x, z, randInt());
// 		}
// 	}
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

const char *jde = "(Z)JDE: %d";
const char *ff = "(X)FF: %d";
const char *crate = "(C)BOX: %d";


void DrawHUD(){
	Disables();
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);
	
	//renderBitmapString(10, 20, texto, RGBf(1.0f, 0.0f, 0.0f));

	//sprintf(pos, "X: %.2f, Y: %.2f, Z: %.2f", player->getPos().x, player->getPos().y, player->getPos().z);
	//renderBitmapString(10, 40, pos, RGBf(1.0f, 0.0f, 0.0f));


	// TODO hud adapta-se ao tamanho do ecra
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
	renderBitmapString(120, 590, updatet, RGBf(1.0f, 1.0f, 0.0f));

	sprintf(updatet, crate, player->itemInventory[BOX_CRATE]);
	renderBitmapString(210, 590, updatet, RGBf(1.0f, 1.0f, 0.0f));

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(width / 2.0 - 5, height / 2.0);
	glVertex2f(width / 2.0 + 5, height / 2.0);
	glVertex2f(width / 2.0, height / 2.0 - 5);
	glVertex2f(width / 2.0, height / 2.0 + 5);
	glEnd();
	
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

void drawTree(Tree t){

	glColor3f(0.133, 0.431, 0.04);
	glPushMatrix();
	
	if (!performance){
		glTranslatef(t.x, -15, t.z);
		glScalef(3.0, 3.0, 3.0);
		glRotatef(t.ang, 0.0, 1.0, 0.0);
		treeModel->render(false);
		//arvore->Render();
	}
	else{
		glTranslatef(t.x, 70, t.z);
		glScalef(1.0, 8.0, 1.0);
		glutSolidCube(20);
	}
	glPopMatrix();
}


void drawCollisionGrid(){

	GLfloat groundLevel = -10.0f;
	
	glColor4f(1, 0, 0, 0.4);
	
	
	for (int z = 0; z < gameManager->collisionGrid->gridRows; z++){
		for (int x = 0; x < gameManager->collisionGrid->gridCols; x++){
			if (gameManager->collisionGrid->getCell(x, z) == BLOCKED){
				drawFilledRect(x * gameManager->collisionGrid->cellSize, -10, z * gameManager->collisionGrid->cellSize, gameManager->collisionGrid->cellSize, gameManager->collisionGrid->cellSize, RGBf(1.0, 0.0, 0.0));
			}
		}
	}

}

void drawCrate(float x, float y, float z, float width, float height, float length){
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texturas[CRATE]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void meshFace(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat altura, GLfloat largura, int numDiv, GLfloat normalX, GLfloat normalY, GLfloat normalZ){
	int i, j;

	GLfloat meshLargura = (GLfloat)abs(largura) / numDiv;
	GLfloat meshAltura = (GLfloat)abs(altura) / numDiv;

	GLfloat textureLargura = (GLfloat)(20.0f / numDiv);
	GLfloat textureAltura = (GLfloat)(20.0f / numDiv);

	for (i = 0; i < numDiv; i++){
		for (j = 0; j < numDiv; j++){

			glNormal3f(normalX, normalY, normalZ);

			glBegin(GL_POLYGON);
			glTexCoord2f(0.0f + textureLargura*i, 0.0f + textureAltura*j);
			glVertex3f(posX + meshLargura*i, terrainHeight[i][j], posZ + meshAltura*j);

			glTexCoord2f(0.0f + textureLargura*(i), 0.0f + textureAltura*(j + 1));
			glVertex3f(posX + meshLargura*(i), terrainHeight[i][j + 1], posZ + meshAltura*(j + 1));

			glTexCoord2f(0.0f + textureLargura*(i + 1), 0.0f + textureAltura*(j + 1));
			glVertex3f(posX + meshLargura*(i + 1), terrainHeight[i + 1][j + 1], posZ + meshAltura*(j + 1));

			glTexCoord2f(0.0f + textureLargura*(i + 1), 0.0f + textureAltura*j);
			glVertex3f(posX + meshLargura*(i + 1), terrainHeight[i + 1][j], posZ + meshAltura*j);
			glEnd();

		}
	}
}



void drawGroundMesh(int tesselation){

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texturas[RELVA]);

	meshFace(-2048, -10, -2048, 12288, 12288, 32, 0, 1, 0);


}



void drawSkybox(float x, float y, float z, float width, float height, float length)
{
	glDisable(GL_LIGHTING);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, texturas[SKYFRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, texturas[SKYBACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, texturas[SKYLEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, texturas[SKYRIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, texturas[SKYUP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, texturas[SKYDOWN]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_LIGHTING);

}

void debugRender(){
	if (debugKeys[GLFW_KEY_B]){
		DrawQuadTree(gameManager->quad);
	}

	if (debugKeys[GLFW_KEY_N]){
		
	}
}


void render(){

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Enables();


	camera->render();
	//player->render();

	for (unsigned int i = 0; i < crates.size(); i++)
		drawCrate(crates[i].x, -10, crates[i].y, 64, 64, 64);

	gameManager->DrawEntities();
	gameManager->drawMission();

	// ground
	//glDisable(GL_LIGHTING);
	drawGroundMesh(groundTesselation);
	//glEnable(GL_LIGHTING);

 	//drawCollisionGrid();


	for (unsigned int i = 0; i < trees.size(); i++)
		drawTree(trees[i]);
	
	

	debugRender();
	
	drawSkybox(0, 0, 0, 20000, 20000, 20000);


	DrawHUD();
	
	glfwSwapBuffers(window);
}


void putCrate(){

	XY playerCell = XY((int)(player->getPos().x / gameManager->collisionGrid->cellSize), (int)(player->getPos().z / gameManager->collisionGrid->cellSize));

	crates.push_back(playerCell * gameManager->collisionGrid->cellSize);

	gameManager->collisionGrid->setCell(playerCell.x, playerCell.y, BLOCKED);

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

		if (key == GLFW_KEY_K && action == GLFW_RELEASE)
			camera->firstPerson = !camera->firstPerson;

		if (key == GLFW_KEY_1 && action == GLFW_RELEASE){	
			performance = !performance;
			gameManager->performance = !gameManager->performance;
		
		}

		if (key == GLFW_KEY_B && action == GLFW_RELEASE){
			debugKeys[GLFW_KEY_B] = !debugKeys[GLFW_KEY_B];
		}

		if (key == GLFW_KEY_N && action == GLFW_RELEASE){
			debugKeys[GLFW_KEY_N] = !debugKeys[GLFW_KEY_N];
			gameManager->drawCollisionCircle = !gameManager->drawCollisionCircle;
			gameManager->drawLOS = !gameManager->drawLOS;
		}

		if (key == GLFW_KEY_X && action == GLFW_RELEASE){
			player->eventKeys[GLFW_KEY_X] = true;
		}

		if (key == GLFW_KEY_Z && action == GLFW_RELEASE){
			player->eventKeys[GLFW_KEY_Z] = true;
		}

		if (key == GLFW_KEY_C && action == GLFW_RELEASE){
			if (player->itemInventory[BOX_CRATE] > 0){
				putCrate();
				player->itemInventory[BOX_CRATE]--;
			}
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

	

	projeccao.FOV = 75;
	projeccao.Height = height;
	projeccao.Width = width;
	projeccao.zFar = 40000;
	projeccao.zNear = 1;

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(projeccao.FOV, projeccao.Width / projeccao.Height, 1.0, projeccao.zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	// The following code is a fancy bit of math that is equivalent to calling:
	// gluPerspective(fieldOfView/2.0f, width/height , near, far);
	// We do it this way simply to avoid requiring glu.h
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
// 	GLfloat aspectRatio = (width > height) ? projeccao.Width / projeccao.Height : projeccao.Height / float(width);
// 	GLfloat fH = tan(float(projeccao.FOV / 360.0f * PI)) * projeccao.zNear;
// 	GLfloat fW = fH * aspectRatio;
// 	glFrustum(-fW, fW, -fH, fH, projeccao.zNear, projeccao.zFar);
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();

	

	//glfwSwapInterval(1); //vsync

	glEnable(GL_DEPTH_TEST); 
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);

}

void mouseControl(GLFWwindow *win, double x, double y){
	camera->mouseControl(win, x, y);

}

void mouseButton(GLFWwindow *win, int button, int action, int mods){
	if (action == GLFW_PRESS){
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			player->shoot();
	}
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

	initGL();
	initGame();


	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseControl);
	glfwSetMouseButtonCallback(window, mouseButton);
	

	while (!glfwWindowShouldClose(window)){
		
		update();
		render();


		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	gameManager->entities.clear();
	gameManager->quad->clear();

	exit(EXIT_SUCCESS);
}