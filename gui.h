#pragma once
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include <nuklear.h>
#include <nuklear_glfw_gl3.h>


class gui
{
public:
	gui();
	~gui();
	void render();
private:
	struct nk_context *ctx;
	struct nk_color background;
	struct nk_color PriceColor;

};

gui::gui()
{
	ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
	/* Load Fonts: if none of these are loaded a default font will be used  */
	{struct nk_font_atlas *atlas;
	nk_glfw3_font_stash_begin(&atlas);
	/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
	/*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
	/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
	/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
	/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
	/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
	nk_glfw3_font_stash_end();
	/*nk_style_set_font(ctx, &droid->handle);*/}

	/* style.c */
	/*set_style(ctx, THEME_WHITE);*/
	/*set_style(ctx, THEME_RED);*/
	/*set_style(ctx, THEME_BLUE);*/
	/*set_style(ctx, THEME_DARK);*/

	background = nk_rgb(0, 0, 0);
	PriceColor = nk_rgb(0, 255, 0);
}

gui::~gui()
{
	nk_glfw3_shutdown();
}


void gui::render() {
	nk_glfw3_new_frame();


	/*mainmenu*/
	struct nk_panel menubar,filemenu,viewmenu,chartsmenu, toolsmenu, windowmenu, helpmenu;
	static int  show_openoffline, show_google, show_mt4dde, show_help, show_about = nk_false;

	nk_begin(ctx,  "MENUBAR", nk_rect(0, 0, WindowWidth, 25), 0);
	

	nk_menubar_begin(ctx);
	nk_layout_row_begin(ctx, NK_STATIC, 10, 7);
	nk_layout_row_push(ctx, 60);
	if (nk_menu_begin_label(ctx,  "File", NK_TEXT_LEFT, nk_vec2(100, 150)))
	{

		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_menu_item_label(ctx, "Open Offline", NK_TEXT_LEFT))
			show_openoffline = nk_true;
		if (nk_menu_item_label(ctx, "Google", NK_TEXT_LEFT))
			show_google = nk_true;
		if (nk_menu_item_label(ctx, "MT4 DDE", NK_TEXT_LEFT))
			show_mt4dde = nk_true;
		if (nk_menu_item_label(ctx, "Help", NK_TEXT_LEFT))
			show_help = nk_true;
		if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
			show_about = nk_true;
		if (nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT))
			exit(EXIT_SUCCESS);

		nk_menu_end(ctx);
	}
	//nk_layout_row_end(ctx);


	if (nk_menu_begin_label(ctx,  "Chart", NK_TEXT_LEFT, nk_vec2(100, 250)))
	{

		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_menu_item_label(ctx, "Line", NK_TEXT_LEFT))
			bool line = nk_true;
		if (nk_menu_item_label(ctx, "OHLC", NK_TEXT_LEFT))
			bool ohlc = nk_true;
		if (nk_menu_item_label(ctx, "Candlesticks", NK_TEXT_LEFT))
			bool candle = nk_true;
		if (nk_menu_item_label(ctx, "Volume", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Grid", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Color scheme", NK_TEXT_LEFT))
			bool bcolor = nk_true;
		if (nk_menu_item_label(ctx, "Object list", NK_TEXT_LEFT))
			bool olist = nk_true;
		if (nk_menu_item_label(ctx, "Clean All", NK_TEXT_LEFT))
			bool olist = nk_true;
		nk_menu_end(ctx);
	}

	if (nk_menu_begin_label(ctx, "Draw", NK_TEXT_LEFT, nk_vec2(100, 150)))
	{

		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_menu_item_label(ctx, "Line", NK_TEXT_LEFT))
			bool testeditor = nk_true;
		if (nk_menu_item_label(ctx, "Fibonacci", NK_TEXT_LEFT))
			bool test1 = nk_true;
		if (nk_menu_item_label(ctx, "Gann", NK_TEXT_LEFT))
			bool testn = nk_true;
		if (nk_menu_item_label(ctx, "Shapes", NK_TEXT_LEFT))
			bool testn = nk_true;
		if (nk_menu_item_label(ctx, "Arrows", NK_TEXT_LEFT))
			bool testn = nk_true;
		if (nk_menu_item_label(ctx, "Text", NK_TEXT_LEFT))
			bool testn = nk_true;
		nk_menu_end(ctx);
	}

	if (nk_menu_begin_label(ctx,  "F(x)", NK_TEXT_LEFT, nk_vec2(150, 400)))
	{

		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_menu_item_label(ctx, "F(x) Editor", NK_TEXT_LEFT))
			bool test = nk_true;
		if (nk_menu_item_label(ctx, "Moving Average", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "CCI", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "MACD", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Stochastic", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "RSI", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Momentum", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "ADX", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "ATR", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Williams' %R", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Standard Deviation", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "BBANDS", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Parabolic SAR", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Money Flow Index", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "On Balance Volume", NK_TEXT_LEFT))
			bool add = nk_true;
		if (nk_menu_item_label(ctx, "Custom", NK_TEXT_LEFT))
			bool add = nk_true;
	nk_menu_end(ctx);
	}



	if (nk_menu_begin_label(ctx,  "Test", NK_TEXT_LEFT, nk_vec2(100, 150)))
	{

		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_menu_item_label(ctx, "Test Editor", NK_TEXT_LEFT))
			bool testeditor = nk_true;
		if (nk_menu_item_label(ctx, "Test1", NK_TEXT_LEFT))
			bool test1 = nk_true;
		if (nk_menu_item_label(ctx, "Testn", NK_TEXT_LEFT))
			bool testn = nk_true;

		nk_menu_end(ctx);
	}

	if (nk_menu_begin_label(ctx,  "Explorer", NK_TEXT_LEFT, nk_vec2(100, 150)))
	{

		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_menu_item_label(ctx, "Exp.Editor", NK_TEXT_LEFT))
			bool expeditor = nk_true;
		if (nk_menu_item_label(ctx, "Explorer1", NK_TEXT_LEFT))
			bool exp1 = nk_true;
		if (nk_menu_item_label(ctx, "Explorern", NK_TEXT_LEFT))
			bool expn = nk_true;

		nk_menu_end(ctx);
	}


	nk_menubar_end(ctx);
	nk_end(ctx);




	/* Draw */
	{float bg[4];
	nk_color_fv(bg, background);
	glClearColor(bg[0], bg[1], bg[2], bg[3]);
	nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
	 }
}