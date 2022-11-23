#include "libraries.h"

void texture_lib::setRenderer(SDL_Renderer * r)
{
	this->base_renderer = r;
}

texture_lib::texture_lib()
{
}

void texture_lib::destroyAll()
{
	int size = this->textures.size();
	for(int i=0;i<size;i++)
	{
		SDL_DestroyTexture(textures[i].ptr);
	}
}

//return extension of file without dot(.) character
std::string get_extension(std::string from)
{
	std::size_t pos = from.find_last_of(".") + 1;	
	
	if(pos == std::string::npos)
		return "void";
		
	return from.substr(pos);
}

void delete_extension(std::string& str)
{
	std::size_t pos = str.find_last_of(".") - 1;
	str.erase(str.begin()+pos, str.end());
}

SDL_Texture* texture_lib::load_file(std::string path, std::string filename)
{
	std::string full_filename = path + filename;
	texture t;

//setting cool values fot t:
	delete_extension(filename);
	t.file_name = filename;

//load texture in graphic card memory and save pointer where it is
	t.ptr = IMG_LoadTexture(this->base_renderer, full_filename.c_str());

//loaded?
	if(t.ptr == 0)
	{
		printf("[tex_lib] - Texture loading error: %s\n",full_filename.c_str());
		return 0;
	}

//get width and height
	SDL_QueryTexture(t.ptr,NULL,NULL,&t.w,&t.h);

//push texture in library
	textures.push_back(t);
	return t.ptr;
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
		while(1)
		{
			file_list >> filename; //get filename
			
			if(file_list.eof()) break; //end of file check
			
			load_file(path,filename); //load
			counter++;
		}
	}
	
	printf("[tex_lib] - Loaded %d textures\n",counter);
	
	file_list.close();
}

SDL_Texture* texture_lib::getTexture(int index)
{
	return this->textures[index].ptr;
}

SDL_Texture* texture_lib::getByName(std::string name)
{
	int size = this->textures.size();
	for(int i=0;i<size;i++)
	{
		if(textures[i].file_name == name ) return this->textures[i].ptr;
	}
	return 0;
}


SDL_Texture* texture_lib::guarantee_texture(std::string name)
{
	SDL_Texture* tmp = getByName(name);
	
	return (tmp ? tmp : load_file("textures\\",name));
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
	}else{
		printf("[font_lib][E] - Font loading error(size - %d): %s ), loading default font\n",fontsize,full_path.c_str());
		return this->guarantee_font("default.fon",fontsize);
	}
	
	printf("[font_lib][E] - WHERE FUCKING DEFAULT FONT!? GO FUCK YOURSELF!\n");
	exit(1505);
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
		while(1)
		{
			file_list >> filename; //get filename
			if(file_list.eof()) break; //end of file check
			
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


