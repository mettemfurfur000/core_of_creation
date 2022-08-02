#include "fonts_lib.h"

void font_lib::init(int size_,int fontsize_)
{
	this->fontsize = fontsize_;
	
	this->size = size_;
	
	fonts = new ttf_pointer [size];
	fontnames = new char *[size];
}

void font_lib::resize(int new_size)
{
	//resizint fonts
	ttf_pointer *tmp = new ttf_pointer [size];
	for(int i=0;i<size;i++)
	{
		tmp[i].font = fonts[i].font;
	}
	
	delete[] this->fonts;
	this->fonts = new ttf_pointer [new_size];
	for(int i=0;i<new_size;i++)
	{
		this->fonts[i].font = 0;
	}
	for(int i=0;i<size;i++)
	{
		this->fonts[i].font = tmp[i].font;
	}
	
	delete[] tmp;
//resizing fontnames
	char **ctmp = new char *[size];
	
	for(int i=0;i<size;i++)
	{
		ctmp[i] = new char [strlen(this->fontnames[i])];
		strcpy(ctmp[i],this->fontnames[i]);
		delete[] this->fontnames[i];
	}
	
	delete[] this->fontnames;
	this->fontnames = new char *[new_size];
	for(int i=0;i<new_size;i++)
	{
		this->fontnames[i] = 0;
	}
	for(int i=0;i<size;i++)
	{
		fontnames[i] = new char [strlen(ctmp[i])];
		strcpy(this->fontnames[i],ctmp[i]);
		delete[] ctmp[i];
	}
	
	delete[] ctmp;
	
	this->size = new_size;
}

void font_lib::close_all()
{
	for(int i=0;i<size;i++)
	{
		if(fonts[i].font!=NULL)
		{
			printf("[font_lib][L] - Closing font %s on %p\n",fontnames[i],fonts[i].font);
			TTF_CloseFont(fonts[i].font);
		}
		if(fontnames[i])
		{
			delete[] fontnames[i];
		}
	}
}
	
int font_lib::font_load(char * filename,char * true_filename,int index)
{
	if(index >= size)
	{
		resize(size+4);
		printf("[font_lib][W] - Resizing lib... new size: %d\n",size);
	}
	
	fonts[index].font = TTF_OpenFont(filename, fontsize);
	
	if(fonts[index].font == NULL)
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
    printf("[font_lib][L] - Pointer Value: %p\n",fonts[index].font);
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