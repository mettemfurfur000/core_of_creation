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
	LuaState = luaL_newstate();
	luaL_openlibs(LuaState);
	
	getGlobalNamespace(LuaState)
		.beginClass<SDL_Color>("SDL_Color")
			.addProperty("r", &SDL_Color::r)
			.addProperty("g", &SDL_Color::g)
			.addProperty("b", &SDL_Color::b)
			.addProperty("a", &SDL_Color::a)
		.endClass()
			.beginClass<SDL_Point>("SDL_Point")
			.addProperty("x", &SDL_Point::x)
			.addProperty("y", &SDL_Point::y)
		.endClass()
		.beginClass<SDL_Rect>("SDL_Rect")
			.addProperty("x", &SDL_Rect::x)
			.addProperty("y", &SDL_Rect::y)
			.addProperty("h", &SDL_Rect::h)
			.addProperty("w", &SDL_Rect::w)
		.endClass()
		.beginClass<position>("position")
			.addProperty ("updated", &position::updated)
			.addProperty ("fixed_pos", &position::fixed_pos)
			.addProperty ("delta_mode", &position::delta_mode)
			.addProperty ("auto_center", &position::auto_center)
			.addProperty ("shape", &position::shape)
			.addProperty ("center", &position::center)
			.addProperty ("real_rect", &position::real_rect)
			.addProperty ("relative_rect", &position::relative_rect)
			.addProperty ("rel_perc_h", &position::rel_perc_h)
			.addProperty ("rel_perc_w", &position::rel_perc_w)
			.addProperty ("d_x", &position::d_x)
			.addProperty ("d_y", &position::d_y)
		.endClass()
		.beginClass<box>("box")
			.addProperty("position", &box::pos)
			.addProperty("moving",&box::moving)
			.addProperty("shown",&box::shown)
			.addProperty("color", &box::color)
			.addProperty("border_color",&box::border_color)
			.addProperty("border_th",&box::border_th)
		.endClass()
		.beginClass<text>("text")
			.addProperty("text_string", &text::text)
			.addProperty("font_name", &text::font_name)
			.addProperty("text_box", &text::text_box)
			.addProperty("font_size", &text::font_size)
			.addProperty("centered", &text::centered)
			.addProperty("updated", &text::updated)
		.endClass()
		.beginClass<button>("button")
			.addProperty("text_part", &button::text_part)
			.addProperty("pressed", &button::pressed)
			.addProperty("focused", &button::focused)
			.addProperty("locked", &button::locked)
			.addProperty("click", &button::click)
		.endClass();
}

void lua_worker::func_register(char * name, lua_CFunction f)
{
	lua_register(LuaState,name,f);
}

void lua_worker::dofile(const char * filename)
{
	printf("lets do '%s' file\n",filename);
	luaL_dofile(LuaState,filename);
}

void lua_worker::call(char * function)
{
	LuaRef f = getGlobal(LuaState, function);
	f();
}