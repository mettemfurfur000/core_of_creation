#include "fonts_lib.h"

void font_lib::init(int size,int fontsize)
{
	this->fontsize = fontsize;
	this->size = size;
	t_init(this->fonts,size);
	this->fontnames = new std::string [size];
}

void font_lib::resize()
{
	t_resize(this->fonts,this->size,4);
	t_resize(this->fontnames,this->size,4);
}

void font_lib::close_all()
{
	for(int i=0;i<size;i++)
	{
		if(fonts[i])
		{
			printf("[font_lib][L] - Closing font %s on %p\n",fontnames[i],fonts[i]);
			TTF_CloseFont(fonts[i]);
		}
		if(fontnames[i].length())
		{
			fontnames[i].clear();
		}
	}
	delete[] fontnames;
}
	
int font_lib::font_load(char * filename,char * true_filename,int index)
{
	if(index >= size)
	{
		resize();
		printf("[font_lib][W] - Resizing lib... new size: %d\n",size);
	}
	
	fonts[index] = TTF_OpenFont(filename, fontsize);
	
	if(fonts[index] == NULL)
	{
		printf("[font_lib][E] - Error while loading font '%s': %s\n",filename,SDL_GetError());
		return 1;
    }
    ;
    this->fontnames[index] = true_filename;
    
    if(fontnames[index].length()==0)
	{
		printf("[font_lib][E] - Error while copying name of file\n");
		this->fontnames[index].clear();
		return 1;
    }
    
    printf("[font_lib][L] - %p : %s font\n",fonts[index],fontnames[index].c_str());
    return 0;
}

void font_lib::automatic_load(char * path)
{
	char command[256];
	
	char c;
	
	sprintf(command,"dir /b fonts\\ > tmp.txt");
	system( command );
	
	FILE * list = fopen("tmp.txt","r");
	char filename[300];
	char full_filename[300];
	
	int index = 0;
	
	int pathlen = strlen(path);
	
	c = fgetc(list);
	if(list!=0&&!feof(list))
	{
		ungetc(c,list);
		while(!feof(list))
		{
			memset(filename,0,300);
			memset(full_filename,0,300);
			fscanf(list,"%s",filename);
			sprintf(full_filename,"%s%s",path,filename);
			
			if(full_filename[pathlen] != 0)
			{
				if(this->font_load(full_filename,filename,index)) 
					index--;
			}
			else break;
			
			index++;
		}
	}
	printf("[font_lib][L] - Loaded %d fonts\n",index);
	fclose(list);
}

TTF_Font * font_lib::GetByName(std::string name)
{
	int i = 0;
	
	while(this->fontnames[i] == name && i < this->size) i++;
	
	if(!(i<this->size)) return NULL;
	
	return this->fonts[i];
}

std::string font_lib::GetByPointer(TTF_Font * pointer)
{
	int i = 0;
	
	while(pointer!=this->fonts[i]&&i<this->size) i++;
	
	if(!(i<this->size)) return NULL;
	
	return this->fontnames[i];
}