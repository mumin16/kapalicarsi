#pragma once

#pragma comment(lib,"lua51")
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <vector>

enum APPLIED_PRICE { PRICE_CLOSE = 0 ,PRICE_OPEN = 1, PRICE_HIGH = 2, PRICE_LOW = 3, PRICE_MEDIAN = 4, PRICE_TYPICAL = 5, PRICE_WEIGHTED = 6 };

class luascript
{
	lua_State*	L;
public:

	luascript(const char* luafile)
	{
		// initialize Lua 
		L = lua_open();
		// load Lua base libraries 
		luaL_openlibs(L);
		// load the script 
		luaL_dofile(L, luafile);
	}

	virtual ~luascript()
	{
		lua_close(L);
	}

	void setglobal(const char* varname, std::vector<double> value) {
		lua_newtable(L);              // table
		for (int i = 0; i<value.size(); i++) {
			lua_pushnumber(L, i + 1);    // table,key
			lua_pushnumber(L, value[i]); // table,key,value
			lua_settable(L, -3);         // table
		}
		lua_setglobal(L, varname);
	}
	//roc-momentum
	std::vector<double> function(const char* functionname, int param1, int param2) {
		std::vector<double> ret;

		/* push functions and arguments */
		/* function to be called */
		lua_getglobal(L, functionname); 
		 /* push 1nd argument */
		lua_pushnumber(L, param1);
		/* push 2nd argument */
		lua_pushnumber(L, param2);
		//call
		lua_pcall(L, 2, 1, 0);//2 param 1 return
		if (lua_isnil(L, -1)) {
			return ret;
		}

		//return
		lua_pushnil(L);
		while (lua_next(L, -2)) {
			ret.push_back((double)lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
		return ret;
	}
	//atr
	std::vector<double> function(const char* functionname, int param1) {
		std::vector<double> ret;

		/* push functions and arguments */
		/* function to be called */
		lua_getglobal(L, functionname);
		/* push 1nd argument */
		lua_pushnumber(L, param1);
		//call
		lua_pcall(L, 1, 1, 0);//1 param 1 return
		if (lua_isnil(L, -1)) {
			return ret;
		}

		//return
		lua_pushnil(L);
		while (lua_next(L, -2)) {
			ret.push_back((double)lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
		return ret;
	}
	//std::vector<double> function(const char* functionname) {
	//	std::vector<double> ret;

	//	/* push functions and arguments */
	//	lua_getglobal(L, functionname);  /* function to be called */
	//									 //push 1st argument 



	//									 //call
	//	lua_pcall(L, 0, 1, 0);//0 param 1 return
	//	if (lua_isnil(L, -1)) {
	//		return ret;
	//	}


	//	//return
	//	lua_pushnil(L);
	//	while (lua_next(L, -2)) {
	//		ret.push_back((double)lua_tonumber(L, -1));
	//		lua_pop(L, 1);
	//	}
	//	return ret;
	//}
	//std::vector<double> function(const char* functionname, int param1) {
	//	std::vector<double> ret;

	//	/* push functions and arguments */
	//	lua_getglobal(L, functionname);  /* function to be called */
	//									 //push 1st argument 

	//									 /* push 1nd argument */
	//	lua_pushnumber(L, param1);

	//	//call
	//	lua_pcall(L, 1, 1, 0);//1 param 1 return
	//	if (lua_isnil(L, -1)) {
	//		return ret;
	//	}


	//	//return
	//	lua_pushnil(L);
	//	while (lua_next(L, -2)) {
	//		ret.push_back((double)lua_tonumber(L, -1));
	//		lua_pop(L, 1);
	//	}
	//	return ret;
	//}

	//std::vector<double> function(const char* functionname, std::vector<double> param1, int param2) {
	//	std::vector<double> ret;

	//	/* push functions and arguments */
	//	lua_getglobal(L, functionname);  /* function to be called */
	//									 //push 1st argument 
	//	lua_newtable(L);              // table
	//	for (int i = 0; i<param1.size(); i++) {
	//		lua_pushnumber(L, i + 1);    // table,key
	//		lua_pushnumber(L, param1[i]); // table,key,value
	//		lua_settable(L, -3);         // table
	//	}

	//	/* push 2nd argument */
	//	lua_pushnumber(L, param2);

	//	//call
	//	lua_pcall(L, 2, 1, 0);//2 param 1 return
	//	if (lua_isnil(L, -1)) {
	//		return ret;
	//	}


	//	//return
	//	lua_pushnil(L);
	//	while (lua_next(L, -2)) {
	//		ret.push_back((double)lua_tonumber(L, -1));
	//		lua_pop(L, 1);
	//	}
	//	return ret;
	//}
	//std::vector<double> function(const char* functionname, int param1, int param2) {
	//	std::vector<double> ret;

	//	/* push functions and arguments */
	//	lua_getglobal(L, functionname);  /* function to be called */
	//									 //push 1st argument 


	//									 /* push 1nd argument */
	//	lua_pushnumber(L, param1);

	//	/* push 2nd argument */
	//	lua_pushnumber(L, param2);

	//	//call
	//	lua_pcall(L, 2, 1, 0);//3 param 1 return
	//	if (lua_isnil(L, -1)) {
	//		return ret;
	//	}


	//	//return
	//	lua_pushnil(L);
	//	while (lua_next(L, -2)) {
	//		ret.push_back((double)lua_tonumber(L, -1));
	//		lua_pop(L, 1);
	//	}
	//	return ret;
	//}
	//std::vector<double> function(const char* functionname, std::vector<double> param1, int param2, double param3) {
	//	std::vector<double> ret;

	//	/* push functions and arguments */
	//	lua_getglobal(L, functionname);  /* function to be called */
	//									 //push 1st argument 
	//	lua_newtable(L);              // table
	//	for (int i = 0; i<param1.size(); i++) {
	//		lua_pushnumber(L, i + 1);    // table,key
	//		lua_pushnumber(L, param1[i]); // table,key,value
	//		lua_settable(L, -3);         // table
	//	}

	//	/* push 2nd argument */
	//	lua_pushnumber(L, param2);

	//	/* push 3nd argument */
	//	lua_pushnumber(L, param3);

	//	//call
	//	lua_pcall(L, 3, 1, 0);//3 param 1 return
	//	if (lua_isnil(L, -1)) {
	//		return ret;
	//	}


	//	//return
	//	lua_pushnil(L);
	//	while (lua_next(L, -2)) {
	//		ret.push_back((double)lua_tonumber(L, -1));
	//		lua_pop(L, 1);
	//	}
	//	return ret;
	//}

	//std::vector<double> function(const char* functionname, int param1, std::vector<double> param2) {
	//	std::vector<double> ret;

	//	/* push functions and arguments */
	//	lua_getglobal(L, functionname);  /* function to be called */

	//									 /* push 1nd argument */
	//	lua_pushnumber(L, param1);

	//	//push 2st argument 
	//	lua_newtable(L);              // table
	//	for (int i = 0; i<param2.size(); i++) {
	//		lua_pushnumber(L, i + 1);    // table,key
	//		lua_pushnumber(L, param2[i]); // table,key,value
	//		lua_settable(L, -3);         // table
	//	}



	//	//call
	//	lua_pcall(L, 2, 1, 0);//2 param 1 return
	//	if (lua_isnil(L, -1)) {
	//		return ret;
	//	}


	//	//return
	//	lua_pushnil(L);
	//	while (lua_next(L, -2)) {
	//		ret.push_back((double)lua_tonumber(L, -1));
	//		lua_pop(L, 1);
	//	}
	//	return ret;
	//}
};

