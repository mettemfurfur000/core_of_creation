#include <SDL.h>
#include <iostream>

#include "tex_lib.h"

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
	
	painter();
	~painter();
	
	bool basic_init();
	void quit();
	
	void dev_draw_all(tex_lib &source);
	
	void simple_draw(int x,int y,int w,int h,SDL_Texture * tex);
	
	void lock(SDL_Surface *screen);
	void unlock(SDL_Surface *screen);
	
	void clean();
	void update();
};