#include "fonts_lib.h"

void font_lib::init(int size_,int fontsize_)
{
	this->fontsize = fontsize_;
	
	this->size = size_;
	
	fonts = new TTF_Font *[size];
	fontnames = new char *[size];
	
	for(int i=0;i<size;i++)
	{
		fonts[i] = NULL;
	}
}

void font_lib::resize(int new_size)
{
	TTF_Font **tmp = new TTF_Font *[size];
	for(int i=0;i<size;i++) // <
	{
		tmp[i] = this->fonts[i];
	}
	
	delete[] this->fonts;
	
	size = new_size;
	this->fonts = new TTF_Font *[size]; //resize
	
	for(int i=0;i<size;i++) // >
	{
		this->fonts[i] = tmp[i];
	}
	
	delete[] tmp;
}


font_lib::~font_lib()
{
	for(int i=0;i<size;i++)
	{
		if(fonts[i]!=NULL)
		{
			TTF_Font * tmp = this->fonts[i];
			TTF_CloseFont(tmp);
		}
		delete[] fontnames[i];
	}
}
	
int font_lib::font_load(char * filename,char * true_filename,int index)
{
	if(index > size)
	{
		resize(size+4);
		printf("[font_lib][W] - Resizing lib... new size: %d\n",size);
	}
	fonts[index] = TTF_OpenFont(filename, fontsize);
	
	if(fonts[index] == NULL)
	{
		printf("[font_lib][E] - Error while font loading: %s\n",SDL_GetError());
		return 1;
    }
    
    fontnames[index] = new char [strlen(true_filename)+1];
	strcpy(fontnames[index],true_filename);
    
    if(fontnames[index] == NULL)
	{
		printf("[font_lib][E] - Error while copying name of file\n");
		delete[] fontnames[index];
		return 1;
    }
    
    printf("[font_lib][L] - Normal loading %s font\n",fontnames[index]);
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