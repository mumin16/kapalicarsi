#pragma once
#define GLEW_STATIC
#pragma comment(lib,"opengl32")
#pragma comment(lib,"glew32s")
#include <gl/glew.h>
#pragma comment(lib,"glfw3")
#include <GLFW\glfw3.h>
GLFWwindow* window;
int WindowWidth, WindowHeight;


#include "datasource.h"

#include "shading.h"
shading shd;

#include "luascript.h"
luascript* lua;

#include "charting.h"
charting* chart;

#include "gui.h"
gui* ui;





//#include "dde.h"
//dde* Dde;









