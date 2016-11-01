#include "charting.h"
#include <vector>
std::vector<double> close;
std::vector<double> outLinReg;
std::vector<double> outRealUpperBand;
std::vector<double> outRealMiddleBand;
std::vector<double> outRealLowerBand;
std::vector<double> outMom;
std::vector<double> outRsi;

std::vector<float> drawingline;
std::vector<float> drawingLinReg;
std::vector<float> drawingoutRealUpperBand;
std::vector<float> drawingoutRealLowerBand;
std::vector<float> drawingoutMom;
std::vector<float> drawingoutRsi;

int LinRegBegIdx, BBandBegIdx,outBegIdx, outNbElement;
int zoom = 0;
int zoomstep = 0;

#include "includes.h"

#include <algorithm>


//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <ta_lib\ta_libc.h>
#pragma comment(lib, "ta_lib")

void renderline() {
	glClear(GL_COLOR_BUFFER_BIT);
	ui->render();
	glViewport((GLfloat)0, (GLfloat)WindowHeight * 2 / 6, (GLfloat)WindowWidth, (GLfloat)WindowHeight * 4/6);
	view = glm::lookAt(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::ortho(0.0f, (float)WindowWidth, (float)*min_element(close.begin() + zoom, close.end()), (float)*max_element(close.begin() + zoom, close.end()), 0.0f, 100.0f);
	glUseProgram(shd.mProgram);
	shd.loadmvp(model, view, projection);


	shd.loadcolor(glm::vec4(255.0, 255.0, 255.0, 1.0));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, drawingline.data());
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0, drawingline.size()/2);//line draw


	///*Vertical for price data*/
	//std::vector <float> vertline;
	//vertline.push_back(0);
	//vertline.push_back((float)*min_element(close.begin() + zoom, close.end()));
	//vertline.push_back((GLfloat)WindowWidth);
	//vertline.push_back((float)*min_element(close.begin() + zoom, close.end()));
	//shd.loadcolor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertline.data());
	//glEnableVertexAttribArray(0);
	//glDrawArrays(GL_LINE_STRIP, 0, 4);//vertical line
	
	/*linreg*/
	int nodraw = 0;
	if (zoom<LinRegBegIdx) nodraw = (LinRegBegIdx - zoom);
	shd.loadcolor(glm::vec4(0.0, 255.0, 0.0, 1.0));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, drawingLinReg.data() + nodraw * 2);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0, (drawingLinReg.size() - nodraw * 2) / 2);//point draw



	/*BBANDS*/
	nodraw = 0;
	if (zoom<BBandBegIdx+ LinRegBegIdx) nodraw = (BBandBegIdx+ LinRegBegIdx - zoom);
	shd.loadcolor(glm::vec4(1.0, 0.0, 0.0, 1.0));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, drawingoutRealUpperBand.data() + nodraw * 2);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0, (drawingoutRealUpperBand.size() - nodraw * 2) / 2);//point draw
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, drawingoutRealLowerBand.data() + nodraw * 2);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0, (drawingoutRealLowerBand.size() - nodraw * 2) / 2);//point draw

	/*RSI*/																				  /*ROC*/
	glViewport((GLfloat)0, (GLfloat)WindowHeight * 1 / 6, (GLfloat)WindowWidth, (GLfloat)WindowHeight * 1 / 6);
	view = glm::lookAt(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::ortho(0.0f, (float)WindowWidth, (float)*min_element(outRsi.begin() + zoom, outRsi.end()), (float)*max_element(outRsi.begin() + zoom, outRsi.end()), 0.0f, 100.0f);
	glUseProgram(shd.mProgram);
	shd.loadmvp(model, view, projection);

	nodraw = 0;
	if (zoom<outBegIdx) nodraw = (outBegIdx - zoom);
	shd.loadcolor(glm::vec4(0.0, 1.0, 1.0, 1.0));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, drawingoutRsi.data() + nodraw * 2);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0, (drawingoutRsi.size() - nodraw * 2) / 2);//point draw

	/*MOM*/
	//glClear(GL_COLOR_BUFFER_BIT);
	glViewport((GLfloat)0, (GLfloat)0, (GLfloat)WindowWidth, (GLfloat)WindowHeight * 1/6);
	view = glm::lookAt(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::ortho(0.0f, (float)WindowWidth, (float)*min_element(outMom.begin() + zoom, outMom.end()), (float)*max_element(outMom.begin() + zoom, outMom.end()), 0.0f, 100.0f);
	glUseProgram(shd.mProgram);
	shd.loadmvp(model, view, projection);
	
	nodraw = 0;
	if (zoom<outBegIdx) nodraw = (outBegIdx - zoom);
	shd.loadcolor(glm::vec4(0.0, 0.0, 1.0, 1.0));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, drawingoutMom.data()+nodraw*2);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0, (drawingoutMom.size()-nodraw*2)/ 2 );//point draw


}
void zoom_pan() {
	drawingline.clear();
	drawingLinReg.clear();
	drawingoutRealUpperBand.clear();
	drawingoutRealLowerBand.clear();
	drawingoutMom.clear();
	drawingoutRsi.clear();
	
	
	float step = (float)WindowWidth / (close.size() - zoom);
	float j = 0;
	for (std::vector<int>::size_type i = 0; i < close.size() - zoom; i++) {
		drawingline.push_back((float)j);	drawingline.push_back(close[zoom + i]);
		drawingoutRealUpperBand.push_back((float)j);	drawingoutRealUpperBand.push_back(outRealUpperBand[zoom + i ]); 
		drawingoutRealLowerBand.push_back((float)j);	drawingoutRealLowerBand.push_back(outRealLowerBand[zoom + i ]); 
		drawingLinReg.push_back((float)j);	drawingLinReg.push_back(outLinReg[zoom + i]);
		drawingoutMom.push_back((float)j);	drawingoutMom.push_back(outMom[zoom + i]);
		drawingoutRsi.push_back((float)j);	drawingoutRsi.push_back(outRsi[zoom + i]);
		j += step;
	}

}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)	glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {

		if (zoom + zoomstep >= close.size())zoom = close.size()-30;
		else 
		{
			zoom += zoomstep;
		}
		zoom_pan();
	}

	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
		if (zoom - zoomstep < 0 )zoom = 0;
		else
		{
			zoom -= zoomstep;
		}
		zoom_pan();
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		if (zoom - (pan + 1) >= 0)pan += 1;
		else
		{
			pan = zoom;
		}
		zoom_pan();
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		if (zoom + (pan - 1) > 0)pan -= 1;
		else
		{
			pan = zoom;
		}
		zoom_pan();
	}

	if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) {
		zoom = 0;
		pan = 0;
		zoom_pan();

	}
		
}
void scrollcallback() {

}
int main() {



	if (!glfwInit())exit(EXIT_FAILURE);
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	WindowWidth = mode->width;
	WindowHeight = mode->height;
	glfwWindowHint(GLFW_MAXIMIZED, TRUE);
	window = glfwCreateWindow(WindowWidth, WindowHeight, "Kapali Carsi", NULL, NULL);
	
	if (!window)exit(EXIT_FAILURE);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	if (GLEW_OK != glewInit())exit(EXIT_FAILURE);
	//glfwSwapInterval(1);


	//glEnable(GL_PROGRAM_POINT_SIZE);

	if (FALSE == goog.download("GARAN","900"))exit(EXIT_FAILURE);
	close = goog.close;
	zoomstep = close.size() / 5;

	TA_RetCode retCode;
	//ind1.resize(close.size());



	//retCode =TA_MA(0,close.size()-1, close.data(),90,
	//	TA_MAType_SMA,
	//	&outBegIdx,
	//	&outNbElement,
	//	ind1.data());

	outLinReg.resize(close.size());
	outRealUpperBand.resize(close.size());
	outRealMiddleBand.resize(close.size());
	outRealLowerBand.resize(close.size());
	outMom.resize(close.size());
	outRsi.resize(close.size());
	
	retCode = TA_LINEARREG(0, close.size() - 1, close.data(), 130,&LinRegBegIdx,&outNbElement,outLinReg.data());
	std::rotate(outLinReg.rbegin(), outLinReg.rbegin() + LinRegBegIdx, outLinReg.rend());

	retCode =TA_BBANDS(0, outLinReg.size() - 1, outLinReg.data(), 25,2.03,	2.03, TA_MAType_SMA,
		&BBandBegIdx,
		&outNbElement,
		outRealUpperBand.data(),
		outRealMiddleBand.data(),
		outRealLowerBand.data());

	std::rotate(outRealUpperBand.rbegin(), outRealUpperBand.rbegin() + BBandBegIdx, outRealUpperBand.rend());
	std::rotate(outRealLowerBand.rbegin(), outRealLowerBand.rbegin() + BBandBegIdx, outRealLowerBand.rend());

	retCode =TA_MOM(0, close.size() - 1, close.data(), 90,
		&outBegIdx,
		&outNbElement,
		outMom.data());

	std::rotate(outMom.rbegin(), outMom.rbegin() + outBegIdx, outMom.rend());


	retCode = TA_RSI(0, close.size() - 1, close.data(), 90, &outBegIdx,
		&outNbElement, outRsi.data());
	std::rotate(outRsi.rbegin(), outRsi.rbegin() + outBegIdx, outRsi.rend());

	zoom_pan();

	//Dde = new dde();
	//Dde->DDERequest();
	//zoom_pan();


	shd.demoshader();	

	


	ui = new gui();

	while (!glfwWindowShouldClose(window))
	{

		glfwGetFramebufferSize(window, &WindowWidth, &WindowHeight);
		

		//Dde->DDERequest();

		zoom_pan();
		
		
		renderline();
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	delete ui;
	return 0;

}