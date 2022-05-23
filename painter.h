#include <SDL.h>
#include <iostream>

#pragma once

//┌─────────────────────┐//
//│*    The Painter    *│//
//│                     │//
//│ *God of the grapgh* │//
//└─────────────────────┘//

class surface_array
{
public:
	SDL_Surface ** media_lib = NULL;
	
	short arr_size = 0;
	
	void arr_init(int size);
	void arr_resize(int new_size);
	void arr_free();
	
	void load_bmp(int index, char * file);
};

class texture_array
{
public:
	SDL_Texture ** texture_lib = NULL;
	
	short arr_size = 0;
	
	void arr_init(int size);
	void arr_resize(int new_size);
	void arr_free();
	
	void load_from_surface(int index, SDL_Renderer * renderer, SDL_Surface * surface);
	void load_bmp(int index, SDL_Renderer * renderer,char * file);
};

struct draw_object
{
	SDL_Rect rect;
	
	texture_array texture;
	short frame = 0;
	short step = 1;
	short state;
};

class painter
{
public:
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 480;
	
	SDL_Window * base_window = NULL;
	SDL_Surface * base_surface = NULL;
	SDL_Renderer * base_renderer = NULL;
	
	painter();
	~painter();
	
	bool basic_init();
	void quit();
	
	void lock(SDL_Surface * screen);
	void unlock(SDL_Surface * screen);
	void draw_texture(draw_object * obj,int frameshift);
	void clean();
	void update();
};