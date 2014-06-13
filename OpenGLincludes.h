#ifndef OpenGLIncludes_h
#define OpenGLIncludes_h

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <GLFW/glfw3.h>
#endif

#endif