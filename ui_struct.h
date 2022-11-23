#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <single_include/nlohmann/json.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

#pragma once

SDL_Rect& operator+=(SDL_Rect &destination, SDL_Rect source);

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

//TODO
struct image
{
	position pos;
	std::string name;
	SDL_Texture* texture;
};

struct text
{
	box text_box;
	
	std::string text;
	
	std::string font_name;
	
	TTF_Font * font = NULL;
	
	int font_size = 12;
	
	bool centered = false;
	
	//for renderer
	bool updated = false;
	
	SDL_Texture * lazy_texture = NULL;
};

struct button
{
	text text_part;
	
	bool pressed;
	bool focused;
	bool locked;
};

class menu
{
public:
	std::string name;
	
	box menu_box;
	box drag_zone;

	bool resizable = false;
	bool movable = false;
	bool shown = true;
	bool copy_window_rect = false;
	
	std::vector<button> buttons;
	
	std::vector<text> texts;
	
	std::vector<image> images;

	void save(std::string folder,std::string filename);
	bool load(std::string folder,std::string filename);
};

class window
{
public:
	std::vector<menu> menus;
	
	void loadMainMenu();
	menu* getMenuByName(std::string _name);
};
