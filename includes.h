#pragma once
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#if defined(_MSC_VER) && defined(_WIN64)
#pragma comment(lib,"x64/glew32s")
#pragma comment(lib,"x64/glfw3")
#pragma comment(lib,"opengl32")
#pragma comment(lib,"x64/libcurl")
#pragma comment(lib,"Ws2_32")
#else 
#pragma comment(lib,"glew32s")
#pragma comment(lib,"glfw3")
#pragma comment(lib,"opengl32")
#pragma comment(lib,"libcurl")
#pragma comment(lib,"Ws2_32")
#endif // _MSC_VER && _WIN64


//Wldap32.lib

#define GLEW_STATIC
#include <GL\glew.h>
#include "shading.h"
shading shd;
#include "googlefinance.h"
googlefinance goog;
#include <GLFW\glfw3.h>
GLFWwindow* window;
int WindowWidth, WindowHeight;

#include "gui.h"
gui* ui;

#include "dde.h"
dde* Dde;




int pan=0;


// Create camera transformation
glm::mat4 model = glm::mat4();
glm::mat4 view = glm::mat4();
glm::mat4 projection = glm::mat4();


