#include "lua_worker.h"

using namespace luabridge;

int lua_test() 
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "script.lua");
    
    LuaRef s = getGlobal(L, "testString");
    LuaRef n = getGlobal(L, "number");
    
    std::string luaString = s.cast<std::string>();
    int answer = n.cast<int>();
    
    std::cout << luaString << std::endl;
    std::cout << "And here's our number:" << answer << std::endl;
    
    LuaRef f = getGlobal(L, "func");
    std::cout << f(1,1) << std::endl;
    std::cout << f(3,5) << std::endl;
    std::cout << f(-5,5) << std::endl;
    
    LuaRef gg = getGlobal(L, "get_dir");
    
    gg();
    return 0;
}

lua_worker::lua_worker()
{
	L = luaL_newstate();
    luaL_openlibs(L);
}

lua_worker::~lua_worker()
{
	lua_close(L);
}

void lua_worker::loadfile_aschunk(std::string filename)
{
	luaL_dofile(L,filename.c_str());
}

luabridge::LuaRef lua_worker::getGlobal(const char* name)
{
	return luabridge::getGlobal(L,name);
}