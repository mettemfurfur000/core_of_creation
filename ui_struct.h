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

//TODO
struct image
{
	
};

struct box
{
	SDL_Rect shape;
	
	SDL_Color color;
	SDL_Color border_color;
	
	int border_th;
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
	
	std::vector<button> buttons;
	
	std::vector<text> texts;

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
