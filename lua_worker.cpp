#include "lua_worker.h"

using namespace luabridge;

int lua_test() 
{
    lua_State* L = luaL_newstate();
    luaL_dofile(L, "script.lua");
    luaL_openlibs(L);
    
    lua_pcall(L, 0, 0, 0);
    
    LuaRef s = getGlobal(L, "testString");
    LuaRef n = getGlobal(L, "number");
    
    std::string luaString = s.cast<std::string>();
    int answer = n.cast<int>();
    
    std::cout << luaString << std::endl;
    std::cout << "And here's our number:" << answer << std::endl;
    return 0;
}

lua_worker::lua_worker()
{
	LuaState = luaL_newstate();
	luaL_openlibs(LuaState);
}

void lua_worker::lua_func_register(std::string name, lua_CFunction f)
{
	lua_register(LuaState,name.c_str(),f);
}

//void lua_worker::dofile(std::string filename)
//{
//	switch(luaL_dofile(LuaState,filename.c_str()))
//	{
//		case LUA_OK:
//			return;
//
//		case LUA_ERRSYNTAX:
//			printf("syntax error in %s !\n",filename.c_str());
//			break;
//		case LUA_ERRMEM:
//			printf("memory error with %s !\n",filename.c_str());
//			break;
//		case LUA_ERRFILE:
//			printf("error loading %s !\n",filename.c_str());
//			break;
//		default:
//			printf("something wrong with %s ...\n",filename.c_str());
//			break;
//	}
//}