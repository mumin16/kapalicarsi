#pragma once


struct PANEL
{
	std::string name;
	std::vector<double> data;
	std::vector<float> drawingline;
	float color[3];
};
class charting
{
private:


	// Create camera transformation
	glm::mat4 model = glm::mat4();
	glm::mat4 view = glm::mat4();
	glm::mat4 projection = glm::mat4();

	int pan = 0;
	int zoom = 0;
	int zoomstep = 0;




public:
	DataSource::PriceData chartpricedata;
	std::vector<float> drawingline;

	std::vector<PANEL> panels;


	void zoom_pan() {
		drawingline.clear();
		if (panels.size() != 0) for (size_t i = 0; i < panels.size(); i++) 	panels[i].drawingline.clear();
		//drawingLinReg.clear();
		//drawingoutRealUpperBand.clear();
		//drawingoutRealLowerBand.clear();
		//drawingoutMom.clear();
		//drawingoutRsi.clear();


		float step = (float)WindowWidth / (chartpricedata.close.size() - zoom);
		float j = 0;
		for (std::vector<int>::size_type i = 0; i < chartpricedata.close.size() - zoom; i++) {
			drawingline.push_back((float)j);	drawingline.push_back(chartpricedata.close[zoom + i]);
			for (size_t k = 0; k < panels.size(); k++){
				if (chartpricedata.close.size() - panels[k].data.size() <= i) 
				{ int bosluk= chartpricedata.close.size() - panels[k].data.size(); panels[k].drawingline.push_back((float)j);	panels[k].drawingline.push_back(panels[k].data[zoom + i- bosluk]); }
			}
			//drawingoutRealLowerBand.push_back((float)j);	drawingoutRealLowerBand.push_back(outRealLowerBand[zoom + i]);
			//drawingLinReg.push_back((float)j);	drawingLinReg.push_back(outLinReg[zoom + i]);
			//drawingoutMom.push_back((float)j);	drawingoutMom.push_back(outMom[zoom + i]);
			//drawingoutRsi.push_back((float)j);	drawingoutRsi.push_back(outRsi[zoom + i]);
			j += step;
		}

	}

	void render() {
		if (chartpricedata.close.size() == 0)return;


		//grid
		glViewport((GLfloat)0, (GLfloat)0, (GLfloat)WindowWidth, (GLfloat)WindowHeight );
		view = glm::lookAt(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::ortho(0.0f, (float)WindowWidth, (float)0, (float)WindowHeight, 0.0f, 100.0f);
		glUseProgram(shd.mProgram);
		shd.loadmvp(model, view, projection);
		std::vector <float> grid,hgrid;
		
		for (size_t i = WindowHeight / 20; i < WindowHeight; i= i+ WindowHeight/20)
		{
			grid.push_back(0);
			grid.push_back(i);
			grid.push_back((GLfloat)WindowWidth);
			grid.push_back(i);

		}

		for (size_t i = WindowWidth / 20; i < WindowWidth; i = i + WindowWidth / 20)
		{
			grid.push_back(i);
			grid.push_back(0);
			grid.push_back(i);
			grid.push_back((GLfloat)WindowHeight);
		}
		shd.loadcolor(glm::vec4(1.0, 1.0, 1.0, 0.1));
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, grid.data());
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINES, 0, grid.size());//h line


		//zoom_pan();
		glViewport((GLfloat)0, (GLfloat)WindowHeight * panels.size() / (panels.size() + 1), (GLfloat)WindowWidth, (GLfloat)WindowHeight * 1 / (panels.size()+1));
		view = glm::lookAt(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::ortho(0.0f, (float)WindowWidth, (float)*min_element(chartpricedata.close.begin() + zoom, chartpricedata.close.end()), (float)*max_element(chartpricedata.close.begin() + zoom, chartpricedata.close.end()), 0.0f, 100.0f);
		glUseProgram(shd.mProgram);
		shd.loadmvp(model, view, projection);

			
		shd.loadcolor(glm::vec4(0.0, 1.0, 0.0, 1.0));
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, drawingline.data());
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINE_STRIP, 0, drawingline.size() / 2);//line draw
	




		if (panels.size() == 0)return;
		int j = 1;
		for (size_t i = 0; i < panels.size(); i++)
		{
		glViewport((GLfloat)0, (GLfloat)WindowHeight * (panels.size()-j) / (panels.size() + 1) , (GLfloat)WindowWidth, (GLfloat)WindowHeight * 1 / (panels.size() + 1));
		view = glm::lookAt(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::ortho(0.0f, (float)WindowWidth, (float)*min_element(panels[i].data.begin() + zoom, panels[i].data.end()), (float)*max_element(panels[i].data.begin() + zoom, panels[i].data.end()), 0.0f, 100.0f);
		glUseProgram(shd.mProgram);
		shd.loadmvp(model, view, projection);


		shd.loadcolor(glm::vec4(panels[i].color[0], panels[i].color[1], panels[i].color[2], 1.0));
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, panels[i].drawingline.data() );
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINE_STRIP, 0, panels[i].drawingline.size() / 2);//point draw
		++j;
		}
		//
		//glViewport((GLfloat)0, (GLfloat)0, (GLfloat)WindowWidth, (GLfloat)WindowHeight * 1 / 6);
		//view = glm::lookAt(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//projection = glm::ortho(0.0f, (float)WindowWidth, (float)*min_element(panels[0].data.begin() + zoom, panels[0].data.end()), (float)*max_element(panels[0].data.begin() + zoom, panels[0].data.end()), 0.0f, 100.0f);
		//glUseProgram(shd.mProgram);
		//shd.loadmvp(model, view, projection);

		//nodraw = 0;
		//if (zoom<outBegIdx) nodraw = (outBegIdx - zoom);
		//shd.loadcolor(glm::vec4(0.0, 0.0, 1.0, 1.0));
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, drawingoutMom.data() + nodraw * 2);
		//glEnableVertexAttribArray(0);
		//glDrawArrays(GL_LINE_STRIP, 0, (drawingoutMom.size() - nodraw * 2) / 2);//point draw


	}
	charting(DataSource::PriceData pricedata,int charttype)
	{
		chartpricedata = pricedata;
		zoomstep = chartpricedata.close.size() / 5;
		zoom_pan();
		shd.demoshader();
		lua = new luascript("C:/Users/x64/Desktop/ZeroBraneStudio/myprograms/indicators.lua");
		lua->setglobal("OPEN", pricedata.open);
		lua->setglobal("HIGH", pricedata.high);
		lua->setglobal("LOW", pricedata.low);
		lua->setglobal("CLOSE", pricedata.close);
		lua->setglobal("VOLUME", pricedata.volume);
		std::vector<double> median, typical, weighted;
		median.resize(pricedata.close.size());
		typical.resize(pricedata.close.size());
		weighted.resize(pricedata.close.size());
		for (size_t i = 0; i < pricedata.close.size(); i++)
		{
			median[i] = (pricedata.high[i] + pricedata.low[i]) / 2;
			typical[i] = (pricedata.high[i] + pricedata.low[i] + pricedata.close[i]) / 3;
			weighted[i] = (pricedata.high[i] + pricedata.low[i] + pricedata.close[i] + pricedata.close[i]) / 4;
		}
		lua->setglobal("MEDIAN", median);
		lua->setglobal("TYPICAL", typical);
		lua->setglobal("WEIGHTED", weighted);
	}
	void addPanel(PANEL panel)
	{
		panels.push_back(panel);
		zoom_pan();
		shd.demoshader();
	}
	virtual ~charting()
	{
		delete lua;
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {

			if (zoom + zoomstep >= chartpricedata.close.size())zoom = chartpricedata.close.size() - 30;
			else
			{
				zoom += zoomstep;
			}
			zoom_pan();
		}

		if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
			if (zoom - zoomstep < 0)zoom = 0;
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



};

