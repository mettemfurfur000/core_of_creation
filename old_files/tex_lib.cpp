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
	t_init(this->textures,size);
	this->size = size;
}

void tex_lib::resize()
{
	t_resize(this->textures,this->size,32);
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

/*
path can be 

"blocks\\"
"ui\\menu\\"

or something idk

*/

void tex_lib::automatic_load(char * path)
{
	char command[256];
	
	char c;
	
	sprintf(command,"dir /b %s > tmp.txt",path);
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
				printf("[tex_lib][W] - Textures Overflow! ");
				resize();
				printf("New size - %d\n",size);
				return;
			}
			memset(filename,0,300);
			memset(full_filename,0,300);
			fscanf(list,"%s",filename);
			sprintf(full_filename,"%s%s",path,filename);
			
			if(full_filename[pathlen] != 0)
				this->load_from_bmp(full_filename,index);
			else
				break;
			
			index++;
		}
	}
	printf("[tex_lib][L] - Loaded %d textures\n",index);
	fclose(list);
}