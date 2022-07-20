#include <SDL_ttf.h>
#include <iostream>

class font_lib
{
public:
	TTF_Font ** fonts;
	int size = 0;
	
	int fontsize = 0;
	
	SDL_Renderer * base_renderer = NULL;
	
	void init(int size,int fontsize);
	void resize(int new_size);
	~font_lib();
	
	void font_load(char * filename,int index);
	void automatic_load(char * path);
};