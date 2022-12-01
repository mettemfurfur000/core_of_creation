#include "ui_struct.h"

SDL_Rect& operator+=(SDL_Rect &destination, SDL_Rect source)
{
	destination.x += source.x;
	destination.y += source.y;
	return destination;
}


void window::loadMainMenu()
{
	menu main_menu;
	main_menu.load("menusaves","MAIN_MENU");
	this->menus.push_back(main_menu);
}

menu* window::getMenuByName(std::string _name)
{
	int size = this->menus.size();
	for(int i = 0;i<size;i++)
	{
		if(menus[i].name == _name) return &menus[i];
	}
	return 0;
}

button::button()
{
	LuaState = luaL_newstate();
	luaL_openlibs(LuaState);
}

static int lua_get_mouse_x(lua_State* L)
{
	int t;
	lua_Number x;
	SDL_GetMouseState(&t,NULL);
	x = t;
	lua_pushnumber(L, x);
	return 1;
}

static int lua_get_mouse_y(lua_State* L)
{
	int t;
	lua_Number y;
	SDL_GetMouseState(NULL,&t);
	y = t;
	lua_pushnumber(L, y);
	return 1;
}

void register_things(lua_State* L)
{
	lua_register(L,"get_x",lua_get_mouse_x);
	lua_register(L,"get_y",lua_get_mouse_y);
	
	luabridge::getGlobalNamespace(L)
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

void button::load_lua_script()
{
	int ret = luaL_dofile(LuaState,this->scriptname.c_str());
	register_things(LuaState);
	
	if(ret != 0)
	{
		printf("Error: %s\n", lua_tostring(LuaState,ret));
	}
}
