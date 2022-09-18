#include <SDL_image.h>
#include <SDL_ttf.h>

#include "tex_lib.h"
#include "designer.h"

#pragma once

//┌─────────────────────┐//
//│*    The Painter    *│//
//│                     │//
//│* God of the grapgh *│//
//└─────────────────────┘//

class painter
{
public:
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 480;
	
	SDL_Rect windowrect;
	
	SDL_Window * base_window = NULL;
	SDL_Surface * base_surface = NULL;
	SDL_Renderer * base_renderer = NULL;
	
	designer Designer;
	
	tex_lib block_tex_lib;

	painter();
	
	bool basic_init();
	void quit();
	
	void dev_draw_all(tex_lib &source);
	
	void simple_draw(int x,int y,int w,int h,SDL_Texture *tex);
	void simple_fill(SDL_Rect r,SDL_Color c);
	void box_draw(box b);
	void rel_box_draw(SDL_Rect a,box b);
	void rel_box_draw(SDL_Rect a,box b,int border_color_shift);
	
	void menustack_draw();
	void menu_draw(menu &Menu);
	void text_draw(menu &Menu,text &Text);
	void button_draw(menu &Menu,button &Button);
	
	void lock(SDL_Surface *screen);
	void unlock(SDL_Surface *screen);
	
	void clean();
	void update();
};