#include <SDL.h>
#include <iostream>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "tex_lib.h"
#include "fonts_lib.h"

#pragma once

//┌─────────────────────┐//
//│*    The Painter    *│//
//│                     │//
//│ *God of the grapgh* │//
//└─────────────────────┘//

class painter
{
public:
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 480;
	
	SDL_Window * base_window = NULL;
	SDL_Surface * base_surface = NULL;
	SDL_Renderer * base_renderer = NULL;
	
	tex_lib block_tex_lib;
	
	font_lib normal_fonts;
	
	painter();
	~painter();
	
	bool basic_init();
	void quit();
	
	void dev_draw_all(tex_lib &source);
	
	void simple_draw(int x,int y,int w,int h,SDL_Texture * tex);
	void box_draw(SDL_Rect &box,Uint8 r,Uint8 g,Uint8 b,Uint8 a);
	
	void simple_text(char * text,int x,int y,int size);
	
	void lock(SDL_Surface *screen);
	void unlock(SDL_Surface *screen);
	
	void clean();
	void update();
};