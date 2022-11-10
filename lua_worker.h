extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>
#include <iostream>
#include <string>

int lua_test();

class lua_worker
{
private:
	lua_State* LuaState;
public:
	lua_worker();
	
	void lua_func_register(std::string name, lua_CFunction f);
	
	void dofile(std::string filename);
};