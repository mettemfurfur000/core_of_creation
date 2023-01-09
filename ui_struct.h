#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <single_include/nlohmann/json.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

#include "lua_worker.h"

SDL_Rect& operator+=(SDL_Rect &destination, SDL_Rect source);

void register_things(lua_State* L);

struct position
{
	//positioning
	bool updated = false;
	bool fixed_pos;
	bool delta_mode; //if false, used percentage of width and height
	bool auto_center;
	
	bool fixed_size;
	bool delta_size;
	
	float rel_size_perc_h;
	float rel_size_perc_w;
	int d_h;
	int d_w;
	
	float rel_perc_h;
	float rel_perc_w;
	int d_x;
	int d_y;
	
	SDL_Rect shape;
	SDL_Point center;
	//
	SDL_Rect real_rect;
	SDL_Rect relative_rect;
};

struct box
{
	position pos;
	
	//rendering
	bool shown = true;
	
	SDL_Color color;
	SDL_Color border_color;
	int border_th;
	
	//interactive editing
	bool moving = false;
};

struct image
{
	bool shown = true;
	
	position pos;
	std::string filename;
	
	SDL_Texture* texture = NULL;
};

struct text
{
	box text_box;
	
	std::string text;
	std::string font_name;
	
	int font_size = 12;
	
	bool centered = false;
	bool updated = false;
	bool editable = false;
	//
	TTF_Font * font = NULL;
	SDL_Texture * lazy_texture = NULL;
	bool selected = false;
};

struct button
{
	text text_part;
	
	std::string scriptname;
	
	bool pressed;
	bool focused;
	bool locked;
	bool click;
};

struct menu
{
	std::string name;
	
	box menu_box;

	bool resizable = false;
	bool movable = false;
	bool shown = true;
	bool copy_window_rect = false;
	
	std::vector<button> buttons;
	
	std::vector<text> texts;
	
	std::vector<image> images;
};

class window
{
public:
	SDL_Rect windowrect = {
		0,
		0,
		600,
		400
	};
	
	lua_worker LW;
		
	std::vector<menu> menus;
	
	menu* w_getMenu(std::string menu_name);
	bool w_loadMenu(std::string name);
};