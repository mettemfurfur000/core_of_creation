#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <json.hpp>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include "fonts_lib.h"

#pragma once

using json = nlohmann::json;

struct text
{
	std::string text;
	std::string font_name;
	
	TTF_Font * font = NULL;
	
	int pos_x;
	int pos_y;
};

class button
{
public:
	SDL_Rect shape;
	
	bool locked;
	bool focused;
	bool pressed;
	
	std::string text;
	std::string font_name;
	
	SDL_Color color;
	
	TTF_Font * font = NULL;

	void update();
	void update(SDL_Rect real_pos);
};

class menu
{
public:
	std::string name;
	
	SDL_Rect shape;
	
	SDL_Color border_color;
	SDL_Color menu_color;
	
	int border_thickness;
	
	bool resizable = false;
	bool movable = true;
	bool shown = false;
	
	int buttons_size = 0;
	button** buttons = NULL;
	
	int texts_size = 0;
	text** texts = NULL;
	
	void texts_init(int size);
	void buttons_init(int size);
	
	void buttons_resize(int increment);
	void texts_resize(int increment);
};

class designer
{
public:
	int menustack_size;
		
	font_lib normal_fonts;
	
	menu** menustack;
	
	designer();
	
	void init(int start_size);
	void resize(int increment);
	
	void create_menu(std::string,SDL_Rect shape,SDL_Color border_color,SDL_Color menu_color,int border_thickness);
	void new_menu(std::string name);
	int delete_menu(char * name); //will return -1 if not exist
	
	void text_create(std::string menuname,int x,int y,std::string _text,std::string font);
	void button_create(std::string menuname,std::string str,std::string font_name,SDL_Rect shape,SDL_Color color);
	
	menu * get_menu(std::string name);
	
	void save_menu(std::string name,std::string folder);
	void load_menu(std::string name,std::string folder);
};