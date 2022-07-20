#include "creator.h"
#include "painter.h"

#pragma once

class text
{
public:
	char * txt;
	
	int pos_x;
	int pos_y;
	
	int size;
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
	
	void update();
};

class menu
{
public:
	int pos_x;
	int pos_y;
	int size_x;
	int size_y;
	
	bool resizable;
	bool movable;
	
	int state;
	
	int num_of_buttons = 0;
	button * buttons;
	
	int num_of_texts = 0;
	text * texts;
};

class designer
{
public:
	int menustack_size;
	menu * menustack;
	
	void render_all();
};