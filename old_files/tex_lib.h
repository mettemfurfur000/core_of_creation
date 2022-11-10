#include <SDL.h>
#include <iostream>

#include "templates.hpp"

#pragma once

class tex_lib
{
public:
	SDL_Renderer * base_renderer = NULL;
	SDL_Texture ** textures;
	int size = 0;
	
	void init(int size);
	~tex_lib();
	
	void load_from_bmp(char * filename,int index);
	void automatic_load(char * path);
};