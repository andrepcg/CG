#ifndef UTILS
#define UTILS

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif


#include <cmath>
#include <cstdlib>
#include <random>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "Estruturas.h"
#include "OpenGLincludes.h"

enum SkyTexture
{
	SKYRIGHT,
	SKYLEFT,
	SKYUP,
	SKYDOWN,
	SKYFRONT,
	SKYBACK,
	CRATE,
	RELVA
};



#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define GLCheckError() (glGetError() == GL_NO_ERROR)
#define INVALID_UNIFORM_LOCATION 0xffffffff
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define PI 3.141592654

XY normalizeVec(float x, float y);
float clamp(float value, float min, float max);
int randInt(int min, int max);
long currentTimeMillis();
bool ReadFile(const char*, std::string&);
void loadTexture(char *filename, int tipo, GLuint textureArray[], int index);
void DrawCircle(float cx, float cy, float h, float r, int num_segments, RGBf rgb);

#endif

