#include "fonts_lib.h"

void font_lib::init(int size,int fontsize)
{
	this->fontsize = fontsize;
	fonts = new TTF_Font * [size];
	size = size;
	for(int i=0;i<size;i++)
	{
		fonts[i] = NULL;
	}
}

void font_lib::resize(int new_size)
{
	TTF_Font ** tmp = new TTF_Font * [size];
	for(int i=0;i<size;i++) // <
	{
		tmp[i] = fonts[i];
	}
	
	delete[] fonts;
	
	size = new_size;
	fonts = new TTF_Font * [size]; //resize
	
	for(int i=0;i<size;i++) // >
	{
		fonts[i] = tmp[i];
	}
	
	delete[] tmp;
}


font_lib::~font_lib()
{
	for(int i=0;i<size;i++)
	{
		TTF_CloseFont(fonts[i]);
	}
}
	
void font_lib::font_load(char * filename,int index)
{
	fonts[index] = TTF_OpenFont(filename, fontsize);
	if(fonts[index] == NULL)
	{
		printf("[PAINTER][E] - Error while font loading: %s\n",SDL_GetError());
    } 
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
			if(index > size)
			{
				printf("[font_lib][W] - Fonts Overflow! ");
				resize(size+64);
				printf("New size - %d\n",size);
				return;
			}
			memset(filename,0,300);
			memset(full_filename,0,300);
			fscanf(list,"%s",filename);
			sprintf(full_filename,"%s%s",path,filename);
			
			if(full_filename[pathlen] != 0)
				this->font_load(full_filename,index);
			else
				break;
			
			index++;
		}
	}
	printf("[font_lib][L] - Loaded %d fonts\n",index);
	fclose(list);
}