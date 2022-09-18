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

struct editable_position
{
	SDL_Point old_cursor;
	
	bool moving = false;
};

struct editable_text
{
	bool editing = false;
};

struct box
{
	SDL_Rect shape;
	
	int border_th;
	
	SDL_Color color;
	SDL_Color border_color;
	//
	editable_position ed_pos;
};

struct text
{
	box text_box;
	
	std::string text;
	std::string font_name;
	
	TTF_Font * font = NULL;
	//
	editable_text ed_txt;
};

struct button
{
	bool locked;
	bool focused;
	bool pressed;
	
	text text_part;
};

class menu
{
public:
	std::string name;
	
	box menu_box;
	box drag_zone;
	
//not saving values(
	SDL_Point mouse_press_pos;
	bool moving = false;
//)
	
	bool resizable = false;
	bool movable = true;
	bool shown = true;
	
	int buttons_size = 0;
	button** buttons = NULL;
	
	int texts_size = 0;
	text** texts = NULL;
	
	void texts_init(int size);
	void buttons_init(int size);
	
	void buttons_resize(int increment);
	void texts_resize(int increment);
	
	void update();
};

class designer
{
public:
	int menustack_size;
	
	bool edit_mode = false;
	
	font_lib normal_fonts;
	
	menu** menustack;
	
	designer();
	
	void edit_move_object_in_menu(box menubox,box &objbox);
	
	void init(int start_size);
	void resize(int increment);
	
	void create_menu(std::string,SDL_Rect shape,SDL_Color border_color,SDL_Color menu_color,int border_thickness);
	void new_menu(std::string name);
	int delete_menu(char * name); //will return -1 if not exist
	
	void button_update(button &b,SDL_Rect &real_pos);
	
	menu * get_menu(std::string name);
	
	void save_menu(std::string name,std::string folder);
	bool load_menu(std::string name,std::string folder);
};