#include <SDL.h>
#include <iostream>

#pragma once

class tex_lib
{
public:
	SDL_Texture ** textures;
	int size = 0;
	SDL_Renderer * base_renderer = NULL;
	
	void init(int size);
	void resize(int new_size);
	~tex_lib();
	
	void load_from_bmp(char * filename,int index);
	void automatic_load(char * path);
};