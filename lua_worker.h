extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>
#include <LuaBridge/Vector.h>

template<typename T>
void lua_push_single_variable(lua_State* L,T* object,char* name)
{
	luabridge::push(L,object);
	lua_setglobal(L, name);
}

int lua_test();

class lua_worker
{
public:
	lua_State* L;
	
	lua_worker();
	~lua_worker();
	
	void loadfile_aschunk(std::string filename);
	
	luabridge::LuaRef getGlobal(const char* name);
};