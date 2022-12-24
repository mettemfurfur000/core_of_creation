#include <vector>
#include <string>
#include <map>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#pragma once

struct texture
{
	std::string file_name;
	int w;
	int h;
	
	SDL_Texture* ptr;
};

class texture_lib
{
private:
	SDL_Renderer * base_renderer = NULL;
public:
	std::vector<texture> textures;
	
	texture_lib();
	void destroyAll();
	
	SDL_Texture* load_file(std::string path, std::string filename);
	void load_folder(std::string path);
	
	SDL_Texture* getTexture(int index);
	SDL_Texture* getByName(std::string name);
	SDL_Texture* guarantee_texture(std::string name);
	
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

	TTF_Font* load_font(std::string path, std::string fontname, int fontsize);
	void load_folder(std::string path,int fontsize);
	
	TTF_Font* getByNameAndSize(std::string name,int fontsize);
};
