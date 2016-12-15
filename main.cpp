
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "includes.h"



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)	glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (chart)chart->key_callback(window, key, scancode, action, mods);
	
}

int main(int argc, char **argv) {
	

	if (!glfwInit())exit(EXIT_FAILURE);
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	WindowWidth = mode->width;
	WindowHeight = mode->height;
	glfwWindowHint(GLFW_MAXIMIZED, TRUE);


	window = glfwCreateWindow(WindowWidth, WindowHeight, "Grandbazaar", NULL, NULL);

	if (!window)exit(EXIT_FAILURE);
	glfwMakeContextCurrent(window);
	glewExperimental = TRUE;
	glewInit();

	ui = new gui();
	glfwSetKeyCallback(window, key_callback);

	//printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

	//DataSource::PriceData googleprice;
	//DataSource::Google(googleprice, ".DJI", "900");

	//DataSource::PriceData mt4csvprice;
	//DataSource::Mt4Csv(mt4csvprice, "GBPJPY.csv");
	//	
	//DataSource::Metastock::Info info;
	//DataSource::Metastock::GetInfo("./msdir", info);// ""'s mean is metastock files are in current directory.
	//DataSource::PriceData metastockprice;
	//DataSource::Metastock::BySymbol(metastockprice, info, "ADANA");

	//chart = new charting();

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window))
	{

		glfwGetFramebufferSize(window, &WindowWidth, &WindowHeight);

		ui->render();
		if(chart)chart->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	delete chart;
	delete ui;
	return 1;

}
