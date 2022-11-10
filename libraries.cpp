#include "libraries.h"

void texture_lib::setRenderer(SDL_Renderer * r)
{
	this->base_renderer = r;
}

texture_lib::texture_lib()
{
}

texture_lib::~texture_lib()
{
	for(int i=0;i<textures.size();i++)
	{
		SDL_DestroyTexture(textures[i]);
	}
}

void texture_lib::load_file(std::string full_filename)
{
	SDL_Surface * temporary_surface = SDL_LoadBMP_RW(SDL_RWFromFile(full_filename.c_str(), "rb"), 1); 
	//get bmp on surface from full path to file
	
	textures.push_back( SDL_CreateTextureFromSurface(this->base_renderer, temporary_surface)); 
	//load surface in graphic card memory and save pointer where it is
	
	SDL_FreeSurface(temporary_surface);
	//delete surface
}

void texture_lib::load_folder(std::string path)
{
	int counter = 0;
	std::string filename;
	std::string full_filename;

	std::string command = "dir /b " + path + " > tmp.txt"; //create list of files to load (genius)
	
	system( command.c_str() );
	
	std::fstream file_list;
	
	file_list.open("tmp.txt",std::fstream::in);
	
	if(file_list.is_open())
	{
		while(!file_list.eof())
		{
			file_list >> filename; //get filename
			if(filename.size() < 2) break; //end of file check
			
			full_filename = path + filename; //create full filename
			load_file(full_filename); //load
			counter++;
		}
	}
	
	printf("[tex_lib] - Loaded %d textures\n",counter);
	
	file_list.close();
}

SDL_Texture* texture_lib::getTexture(int index)
{
	return this->textures[index];
}

//0---0
// | |
//-----

// Font lib

//0---0
// | |
//-----

TTF_Font* font_lib::load_font(std::string path, std::string fontname, int fontsize)
{
	font tempfont;
	std::string full_path = path + fontname;
	
	TTF_Font * tempFontPtr = TTF_OpenFont(full_path.c_str(), fontsize);
	
	if(tempFontPtr)
	{
		tempfont.name = fontname;
		tempfont.size = fontsize;
		tempfont.ptr = tempFontPtr;
		
		fonts.push_back(tempfont);
		
		printf("[font_lib] - Success font loading(size - %d): %s )\n",fontsize,full_path.c_str());
		
		return tempFontPtr;
	}
	
	printf("[font_lib][E] - Font loading error(size - %d): %s )\n",fontsize,full_path.c_str());
	return 0;
}

void font_lib::load_folder(std::string path,int fontsize)
{
	int counter = 0;
	std::string filename;
	std::string full_filename;

	std::string command = "dir /b " + path + " > tmp.txt"; //create list of files to load (genius)
	
	system( command.c_str() );
	
	std::fstream file_list;
	
	file_list.open("tmp.txt",std::fstream::in);
	
	if(file_list.is_open())
	{
		while(!file_list.eof())
		{
			file_list >> filename; //get filename
			if(filename.size() < 2) break; //end of file check
			
			full_filename = path + filename; //create full filename
			this->load_font(full_filename,filename,fontsize); //load font
			counter++;
		}
	}
	
	printf("[tex_lib][L] - Loaded %d textures\n",counter);
	
	file_list.close();
}

TTF_Font* font_lib::getByNameAndSize(std::string name,int fontsize)
{
	int size = this->fonts.size();
	for(int i=0;i<size;i++)
	{
		if(fonts[i].name == name && fonts[i].size == fontsize) return fonts[i].ptr;
	}
	return 0;
}

TTF_Font* font_lib::load_font_from_default_folder(std::string fontname, int fontsize)
{
//	TTF_Font* tmp = getByNameAndSize(fontname,fontsize);
//	
//	if(tmp) return tmp;
//	
	return load_font("fonts\\",fontname,fontsize);
}

TTF_Font* font_lib::guarantee_font(std::string fontname, int fontsize)
{
	TTF_Font* tmp = getByNameAndSize(fontname,fontsize);
	
	return (tmp ? tmp : load_font("fonts\\",fontname,fontsize));
}

void font_lib::closeAll()
{
	int size = this->fonts.size();
	
	for(int i=0;i<size;i++) TTF_CloseFont(fonts[i].ptr);
}


