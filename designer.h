#include <iostream>
#include <SDL.h>

#pragma once

struct text
{
	char * string = NULL;
	
	char * fontname = NULL;
	
	int pos_x;
	int pos_y;
	
	int length;
};

class button
{
	int pos_x;
	int pos_y;
	int size_x;
	int size_y;
	
	bool locked;
	bool focused;
	bool pressed;
public:
	void update();
	
	void setPos(int &x,int &y);
	void setSize(int &x,int &y);
	
	void lock();
	void unlock();
};

class menu
{
public:
	char * name = NULL;
	
	SDL_Rect shape;
	
	SDL_Color border_color;
	SDL_Color menu_color;
	
	int border_thickness;
	
	bool resizable = false;
	bool movable = true;
	bool shown = false;
	
	int num_of_buttons = 0;
	button * buttons = NULL;
	
	int num_of_texts = 0;
	text * texts = NULL;
};

class designer
{
private:
	void menu_data_fprintf(menu *Menu,FILE * file);
public:
	int menustack_size;
	menu **menustack;
	
	designer();
	
	void init(int start_size);
	void resize(int new_size);
	
	void create_menu(char * name,SDL_Rect shape,SDL_Color border_color,SDL_Color menu_color,int border_thickness);
	int delete_menu(char * name); //will return -1 if not exist
	
	void text_create(menu &Menu,int x,int y,int wrapLength,char *text);
	
	menu * get_menu(char * name);
	
	void save_menu(char * name,char * folder);
};