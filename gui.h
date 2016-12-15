#pragma once
#include <imgui/imgui.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_impl_glfw_gl3.cpp>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>//getcwd

static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};


static char Symbol[256] = ".DJI";
static int TimeFrames = 2;
static int ChartType = 0;
static int PriceType = 0;
static int MAType = 0;
static int IndPeriod = 15;
//macd
static int FastPeriod = 15;
static int SlowPeriod = 15;
static int SignalPeriod = 15;
//stoch.
static int FastK_Period = 15;
static int SlowK_Period = 15;
static int SlowK_MAType = 0;
static int SlowD_Period = 15;
static int SlowD_MAType = 0;

static bool Volumeenabled = false;
static bool Gridenabled = false;
static bool realaccountenabled = false;
static bool demoenabled = true;

static float indcolor[3] = { 0.0f,1.0f,0.0f };

class gui
{
public:
	gui();
	~gui();
	void render();

private:

};

gui::gui() 
{
	ImGui_ImplGlfwGL3_Init(window, true);
}

gui::~gui()
{
	ImGui_ImplGlfwGL3_Shutdown();
}

void gui::render() {
	ImGui_ImplGlfwGL3_NewFrame();
	/**/
	if (ImGui::BeginMainMenuBar())
	{
		//
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Google"))
			{
				
				ImGui::PushItemWidth(50);
				ImGui::InputText("Symbol", Symbol,256, ImGuiInputTextFlags_CharsUppercase);
				ImGui::SameLine();
				ImGui::Combo("TimeFrames", &TimeFrames, "M1\0M5\0M15\0M30\0M60\0D\0\0");
				ImGui::SameLine();
				std::vector<char*> tf = { "60","300","900","1800","3600","86400" };
				if (ImGui::Button("Load"))
				{
					DataSource::PriceData dummy;
					if (FALSE == DataSource::Google(dummy,Symbol, tf[TimeFrames]))return;
					delete chart;
					chart=new charting(dummy,0);
					memset(&Symbol, 0, sizeof(Symbol));

				}
				//memset(Symbol, 0, sizeof(Symbol));
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("MT4 *.csv")) 
			{
				ImGui::PushItemWidth(100);
				char searchDirectory[MAX_PATH];
				char csvfilename[MAX_PATH]="GBPJPY";
				

				_getcwd(searchDirectory, MAX_PATH);

				std::string file = searchDirectory;
				file.append("\\"); file.append(csvfilename); file.append(".csv");
				
				if (ImGui::Button("Open  ")) { 
					DataSource::PriceData dummy;
					if (FALSE == DataSource::Mt4Csv(dummy, file))return;
					delete chart;
					chart = new charting(dummy, 0);
				}
				ImGui::SameLine();
				ImGui::InputText("Directory", searchDirectory, MAX_PATH);
				ImGui::SameLine();
				ImGui::InputText("Filename", csvfilename, MAX_PATH);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("MT4 DDE"))
			{
				ImGui::PushItemWidth(50);
				if (ImGui::Button("Connect")) { printf("%s LOAD!\n", Symbol); }
				ImGui::SameLine();
				ImGui::InputText("Symbol", Symbol, 256, ImGuiInputTextFlags_CharsUppercase);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Metastock")) 
			{
				static char MetastockSearchDirectory[MAX_PATH] = "./msdir";
				static int listbox_item_current = 0;
				static DataSource::Metastock::Info info;
				if (ImGui::Button("Scan")) { info = { 0 }; DataSource::Metastock::GetInfo(MetastockSearchDirectory, info); }// ""'s mean is metastock files are in current directory.
				ImGui::SameLine();
				ImGui::InputText("Directory", MetastockSearchDirectory, MAX_PATH);
				
				if (ImGui::Button("Open")) { 
					DataSource::PriceData dummy;
					if (FALSE == DataSource::Metastock::BySymbol(dummy,info, info.symbols.at(listbox_item_current).c_str()))return;
					delete chart;
					chart = new charting(dummy, 0);
				}
				ImGui::SameLine();
				ImGui::Combo("Symbols", &listbox_item_current, vector_getter, static_cast<void*>(&info.symbols), info.symbols.size(), 40);
				ImGui::EndMenu();
			}


			ImGui::Separator();
			if (ImGui::BeginMenu("Help")) {ImGui::Text("http://veriaktar.blogspot.com.tr/");	ImGui::Separator();	ImGui::Text("youtube.com/veriaktar"); ImGui::EndMenu();	}
			if (ImGui::BeginMenu("About")) { ImGui::Text("MUMIN GULER");	ImGui::Separator();	ImGui::Text("mumin16@hotmail.com"); ImGui::EndMenu(); }
			if (ImGui::MenuItem("Exit")) { exit(EXIT_SUCCESS); }
			ImGui::EndMenu();
		}
		//
		if (ImGui::BeginMenu("Chart"))
		{
			ImGui::PushItemWidth(75);
			ImGui::Combo("TimeFrames", &TimeFrames, "D\0M1\0M5\0M15\0M30\0M60\0\0");
			ImGui::Combo("ChartType", &ChartType, "Line\0OHLC\0Candlestick\0\0");
			if (ImGui::MenuItem("Color scheme")) {}
			ImGui::MenuItem("Volume", "", &Volumeenabled);
			ImGui::MenuItem("Grid", "", &Gridenabled);
			ImGui::Separator();
			if (ImGui::MenuItem("Object List")) {}
			if (ImGui::MenuItem("Clear All")) {}
			ImGui::EndMenu();
		}
		//
		if (ImGui::BeginMenu("Draw"))
		{
			if (ImGui::MenuItem("Fibonacci")) {}
			if (ImGui::MenuItem("Gann")) {}
			if (ImGui::MenuItem("Line")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Shapes")) {}
			if (ImGui::MenuItem("Arrows")) {}
			if (ImGui::MenuItem("Text")) {}
			ImGui::EndMenu();
		}
		//
		if (ImGui::BeginMenu("Indicators"))
		{
			if (ImGui::MenuItem("Indicator Editor")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Rate Of Change"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add"))
				{
					if (chart){
						PANEL panel;
						panel.data= lua->function("ROC", PriceType, IndPeriod);
						panel.name = "ROC";
						panel.color[0] = indcolor[0]; panel.color[1] = indcolor[1]; panel.color[2] = indcolor[2];
						chart->addPanel(panel);
					}
				}
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::Combo("Price Type", &PriceType, "Close\0Open\0High\0Low\0Median\0Typical\0Weighted\0\0");
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Momentum"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add"))
				{
					if (chart) {
						PANEL panel;
						panel.data = lua->function("MOMENTUM", PriceType, IndPeriod);
						panel.name = "MOMENTUM";
						panel.color[0] = indcolor[0]; panel.color[1] = indcolor[1]; panel.color[2] = indcolor[2];
						chart->addPanel(panel);
					}
				}
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::Combo("Price Type", &PriceType, "Close\0Open\0High\0Low\0Median\0Typical\0Weighted\0\0");
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Average True Range"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) 
				{ 
					if (chart) {
						PANEL panel;
						panel.data = lua->function("ATR",  IndPeriod);
						panel.name = "ATR";
						panel.color[0] = indcolor[0]; panel.color[1] = indcolor[1]; panel.color[2] = indcolor[2];
						chart->addPanel(panel);
					}
				}
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Moving Avarage"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) 
				{ 
						//int BegIdx, NbElement;
						//std::vector<double> ind;
						//ind.resize(chart->chartpricedata.close.size());
						//TA_MA(0, chart->chartpricedata.close.size()-1, chart->chartpricedata.close.data(), IndPeriod,
						//(TA_MAType)MAType,
						//&BegIdx,
						//&NbElement,
						//ind.data());
						//std::rotate(ind.rbegin(), ind.rbegin() + BegIdx, ind.rend());
						//chart->rawMainPanel.push_back(ind);
						//chart->mainpanelcount += 1;
						//chart->BegIdx.push_back(BegIdx);
						//panel Panel;
						//Panel.color[0] = indcolor[0];
						//Panel.color[1] = indcolor[1];
						//Panel.color[2] = indcolor[2];
						//chart->panels.push_back(Panel);
						//chart->zoom_pan();
				
				}
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::Combo("Type", &MAType, "Sma\0Ema\0Wma\0Dema\0Tema\0Trima\0Kama\0Mama\0T3\0\0");
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("CCI"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) { printf("%s %i LOAD!\n", Symbol, TimeFrames); }
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("MACD"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) { printf("%s %i LOAD!\n", Symbol, TimeFrames); }
				ImGui::InputInt("FastPeriod", &FastPeriod, 1);
				ImGui::InputInt("SlowPeriod", &SlowPeriod, 1);
				ImGui::InputInt("SignalPeriod", &SignalPeriod, 1);
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Stochastic"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) { printf("%s %i LOAD!\n", Symbol, TimeFrames); }
				ImGui::InputInt("FastK_Period", &FastK_Period, 1);
				ImGui::InputInt("SlowK_Period", &SlowK_Period, 1);
				ImGui::Combo("SlowK_MAType", &SlowK_MAType, "Sma\0Ema\0Wma\0Dema\0Tema\0Trima\0Kama\0Mama\0T3\0\0");
				ImGui::InputInt("SlowD_Period", &SlowD_Period, 1);
				ImGui::Combo("SlowD_MAType", &SlowD_MAType, "Sma\0Ema\0Wma\0Dema\0Tema\0Trima\0Kama\0Mama\0T3\0\0");
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("RSI"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) { printf("%s %i LOAD!\n", Symbol, TimeFrames); }
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("ADX"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) { printf("%s %i LOAD!\n", Symbol, TimeFrames); }
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Williams' %R"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) { printf("%s %i LOAD!\n", Symbol, TimeFrames); }
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("BBANDS")) {}
			if (ImGui::MenuItem("Parabolic SAR")) {}
			if (ImGui::BeginMenu("Money Flow Index"))
			{
				ImGui::PushItemWidth(100);
				if (ImGui::Button("Add")) { printf("%s %i LOAD!\n", Symbol, TimeFrames); }
				ImGui::InputInt("Period", &IndPeriod, 1);
				ImGui::ColorEdit3("Color", indcolor);
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("On Balance Volume")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Custom"))
			{

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		//
		if (ImGui::BeginMenu("Test"))
		{
			if (ImGui::MenuItem("Test Editor")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Test1")) {}
			if (ImGui::MenuItem("Test2")) {}
			if (ImGui::MenuItem("Testn")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Custom"))
			{

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		//
		if (ImGui::BeginMenu("Explorer"))
		{
			if (ImGui::MenuItem("Explorer Editor")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Explorer1")) {}
			if (ImGui::MenuItem("Explorer2")) {}
			if (ImGui::MenuItem("Explorern")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Custom"))
			{

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		//
		if (ImGui::BeginMenu("Portfoy"))
		{
			if (ImGui::MenuItem("Report")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Buy")) {}
			if (ImGui::MenuItem("Sell")) {}
			if (ImGui::MenuItem("Close")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Login"))
			{
				if (ImGui::MenuItem("Demo Account", "", &demoenabled)) {
					realaccountenabled = !realaccountenabled;
				}
				if (ImGui::MenuItem("Real account", "", &realaccountenabled)) {
					demoenabled = !demoenabled;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
	ImGui::EndMainMenuBar();
	/**/

	}



	// Rendering
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	//ImVec4 clear_color = ImColor(114, 144, 154);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
}
