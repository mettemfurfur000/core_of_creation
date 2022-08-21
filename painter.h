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
	void box_draw(SDL_Rect &box,Uint8 r,Uint8 g,Uint8 b,Uint8 a);
	void box_draw(SDL_Rect &box,SDL_Color &color);
	void draw_cool_box(SDL_Rect box,SDL_Color color,int border_w,SDL_Color border_color);
	void draw_cool_box(SDL_Rect box,SDL_Color color,int border_w,int border_shading);
	void draw_cool_box(SDL_Rect menu_box,SDL_Rect box,SDL_Color color,int border_w,int border_shading);
	
	void menustack_draw();
	void menu_draw(menu &Menu);
	void text_draw(menu &Menu,text &Text);
	void button_draw(menu &Menu,button &Button);
	
	void lock(SDL_Surface *screen);
	void unlock(SDL_Surface *screen);
	
	void clean();
	void update();
};