#include <SDL_ttf.h>
#include <iostream>

struct ttf_pointer
{
	TTF_Font * font = NULL;
};

class font_lib
{
public:
	ttf_pointer *fonts;
	
	char **fontnames;
	int size = 0;
	
	int fontsize = 0;
	
	SDL_Renderer * base_renderer = NULL;
	
	void init(int size,int fontsize);
	void close_all();
	
	void resize(int new_size);
	
	int font_load(char * filename,char * true_filename,int index);
	void automatic_load(char * path);
};