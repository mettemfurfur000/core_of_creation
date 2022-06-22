#include "tex_lib.h"

tex_lib::~tex_lib()
{
	for(int i=0;i<this->size;i++)
	{
		SDL_DestroyTexture(textures[i]);
	}
}

void tex_lib::init(int size)
{
	textures = new SDL_Texture * [size];
	this->size = size;
	for(int i=0;i<this->size;i++)
	{
		textures[i] = NULL;
	}
}

void tex_lib::load_from_bmp(char * filename,int index)
{
	if(textures[index]!=NULL)
	{
		SDL_DestroyTexture(textures[index]);
		printf("[tex_lib][W] - Replacing texture #%d\n",index);
	}else{
		printf("[tex_lib][L] - Normal Loading Textures #%d\n",index);
	}
	
	SDL_Surface * tmpsrfc = SDL_LoadBMP_RW(SDL_RWFromFile(filename, "rb"), 1);
	
	textures[index] = SDL_CreateTextureFromSurface(this->base_renderer, tmpsrfc);
	
	SDL_FreeSurface(tmpsrfc);
}

void tex_lib::automatic_load(char * path)
{
	//*path can be 
	//blocks\\
	//ui\\menu\\
	//*
	
	char command[256];
	
	char c;
	
	sprintf(command,"dir /b %s > tmp.txt",path);
	system( command );
	
	FILE * list = fopen("tmp.txt","r");
	char filename[300];
	int index = 0;
	
	c = fgetc(list);
	if(list!=0&&!feof(list))
	{
		ungetc(c,list);
		while(!feof(list))
		{
			fgets(filename,300,list);
			if(filename != NULL &&filename[0] != 0)
				this->load_from_bmp(filename,index);
			else
				break;
			memset (filename,0,300);
			index++;
		}
	}
	printf("[tex_lib][L] - Loaded %d textures\n",index);
	fclose(list);
}