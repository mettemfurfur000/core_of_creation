extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "ui_struct.h"
#include "renderer.h"

int lua_test();

class lua_worker
{
public:
	lua_State* LuaState;

	lua_worker();
	
	void func_register(char * name, lua_CFunction f);
	
	void dofile(const char * filename);
	
	void call(char * function);
};