#include "lua_worker.h"

using namespace luabridge;

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
	luaL_dofile(L,("scripts//" + filename).c_str());
}

luabridge::LuaRef lua_worker::getGlobal(const char* name)
{
	return luabridge::getGlobal(L,name);
}