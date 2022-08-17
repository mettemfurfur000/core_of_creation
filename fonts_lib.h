#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "templates.hpp"

class font_lib
{
public:
	TTF_Font** fonts = NULL;
	std::string* fontnames = NULL;
	
	int size = 0;
	int fontsize = 0;
	
	SDL_Renderer * base_renderer = NULL;
	
	void init(int size,int fontsize);
	void close_all();
	void resize();
	
	int font_load(char * filename,char * true_filename,int index);
	void automatic_load(char * path);
	
	TTF_Font * GetByName(char * name);
	std::string GetByPointer(TTF_Font * pointer);
};