#include "ui_struct.h"

SDL_Rect& operator+=(SDL_Rect &destination, SDL_Rect source)
{
	destination.x += source.x;
	destination.y += source.y;
	return destination;
}

int window::l_getMenu(lua_State* L)
{
	const char* c_name = luaL_checkstring(L, 1);
	
	if(!c_name)
	{
		lua_pushnil(L);
		return 1;
	}
	
	std::string menu_name = c_name;
	
	int size = menus.size();
	
	for(int i=0; i<size; i++)
	{
		if(menus[i].name == menu_name)
		{
			luabridge::push(L,menus[i]);
			return 1; 
		}
	}
	
	lua_pushnil(L);
	
	return 1;
}

void register_things(lua_State* L)
{
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
		.beginClass<position>("pos")
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
			.addProperty("pos", &box::pos)
			.addProperty("moving",&box::moving)
			.addProperty("shown",&box::shown)
			.addProperty("color", &box::color)
			.addProperty("border_color",&box::border_color)
			.addProperty("border_th",&box::border_th)
		.endClass()
		.beginClass<image>("image")
			.addProperty("shown", &image::shown)
			.addProperty("position",&image::pos)
			.addProperty("filename",&image::filename)
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
		.endClass()
		.beginClass<menu>("menu")
			.addProperty("name", &menu::name)
			.addProperty("menu_box", &menu::menu_box)
			.addProperty("resizable", &menu::resizable)
			.addProperty("movable", &menu::movable)
			.addProperty("shown", &menu::shown)
			.addProperty("copy_window_rect", &menu::copy_window_rect)
			.addProperty("buttons", &menu::buttons)
			.addProperty("texts", &menu::texts)
			.addProperty("images", &menu::images)
		.endClass()
		.beginClass<window>("window")
			.addProperty("windowrect", &window::windowrect)
			.addProperty("menus",&window::menus)
			.addFunction("getMenu",&window::l_getMenu)
		.endClass();
}