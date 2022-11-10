#include <vector>
#include <string>
#include <map>
#include <fstream>

#include <SDL_image.h>
#include <SDL.h>
#include <SDL_ttf.h>

#pragma once

class texture_lib
{
private:
	SDL_Renderer * base_renderer = NULL;
public:
	std::vector<SDL_Texture*> textures;
	
	texture_lib();
	~texture_lib();
	
	void load_file(std::string full_filename);
	void load_folder(std::string path);
	
	SDL_Texture* getTexture(int index);
	void setRenderer(SDL_Renderer * r);
};

struct font
{
	std::string name;
	int size;
	
	TTF_Font* ptr;
};

class font_lib
{
public:
	std::vector<font> fonts;
	
	void closeAll();
	
	TTF_Font* guarantee_font(std::string fontname, int fontsize);
	
	TTF_Font* load_font_from_default_folder(std::string fontname, int fontsize);
	
	TTF_Font* load_font(std::string path, std::string fontname, int fontsize);
	void load_folder(std::string path,int fontsize);
	
	TTF_Font* getByNameAndSize(std::string name,int fontsize);
};
