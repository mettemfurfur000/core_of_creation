#include <SDL_ttf.h>
#include <iostream>

class font_lib
{
public:
	TTF_Font **fonts;
	char **fontnames;
	int size = 0;
	
	int fontsize = 0;
	
	SDL_Renderer * base_renderer = NULL;
	
	void init(int size,int fontsize);
	void resize(int new_size);
	~font_lib();
	
	int font_load(char * filename,char * true_filename,int index);
	void automatic_load(char * path);
};