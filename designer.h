#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string>

#include "fonts_lib.h"

#pragma once

struct text
{
	char * string = NULL;
	int strsize;
	
	TTF_Font * font = NULL;
	int fontnamesize;
	
	int pos_x;
	int pos_y;
};

class button
{
public:
	int pos_x;
	int pos_y;
	int size_x;
	int size_y;
	
	bool locked;
	bool focused;
	bool pressed;
	
	char* string;
	
	int strsize;

	void update();
};

class menu
{
public:
	char * name = NULL;
	int namesize;
	
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
	
	void create_menu(char * name,SDL_Rect shape,SDL_Color border_color,SDL_Color menu_color,int border_thickness);
	void new_menu(char * name);
	int delete_menu(char * name); //will return -1 if not exist
	
	void text_create(char* menuname,int x,int y,char *_text,char *font);
	
	menu * get_menu(char * name);
	
	void save_menu(char * name,char * folder);
	void load_menu(char * name,char * folder);
};