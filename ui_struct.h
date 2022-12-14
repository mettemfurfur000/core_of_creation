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
	bool fixed_pos = true;
	bool delta_mode = true; //if false, used percentage of width and height
	bool auto_center = true;
	
	SDL_Rect shape;
	SDL_Point center;
	
	float rel_perc_h = 0.0;
	float rel_perc_w = 0.0;
	int d_x = 0;
	int d_y = 0;
	//
	SDL_Rect real_rect;
	SDL_Rect relative_rect;
	bool drag = false;
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
	
	TTF_Font * font = NULL;
	SDL_Texture * lazy_texture = NULL;
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

struct window
{
	SDL_Rect windowrect = {
		0,
		0,
		600,
		400
	};
	
	lua_worker LW;
		
	std::vector<menu> menus;
};