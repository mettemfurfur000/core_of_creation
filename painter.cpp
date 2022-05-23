#include "painter.h"

void surface_array::arr_init(int size)
{
	this->arr_size = size;
	this->media_lib = new SDL_Surface * [arr_size];
	
	for(int i=0;i<this->arr_size;i++)
	{
		this->media_lib[i] = NULL;
	}
}

void surface_array::arr_resize(int new_size)
{
	SDL_Surface ** Bmedia_lib = new SDL_Surface * [new_size];
	for(int i=0;i<this->arr_size;i++)
	{
		Bmedia_lib[i] = this->media_lib[i];
	}
	
	this->arr_free();
	this->arr_init(new_size);
	
	for(int i=0;i<this->arr_size;i++)
	{
		this->media_lib[i] = Bmedia_lib[i];
	}
	
	delete [] Bmedia_lib;
}

void surface_array::arr_free()
{
	for(int i=0;i<this->arr_size;i++)
	{
		SDL_FreeSurface(this->media_lib[i]);
	}
	delete [] this->media_lib;
	this->arr_size = 0;
}

void surface_array::load_bmp(int index, char * file)
{
	this->media_lib[index] = SDL_LoadBMP_RW(SDL_RWFromFile(file, "rb"), 1);
	if( this->media_lib[index] == NULL )
	{
		printf( "[PAINTER][E] SDL_LoadBMP_RW Error: %s (file:'%s')\n", SDL_GetError() , file );
	}
}

void texture_array::arr_init(int size)
{
	this->arr_size = size;
	this->texture_lib = new SDL_Texture * [arr_size];
	
	for(int i=0;i<this->arr_size;i++)
	{
		this->texture_lib[i] = NULL;
	}
}

void texture_array::arr_resize(int new_size)
{
	SDL_Texture ** Btexture_lib = new SDL_Texture * [new_size];
	for(int i=0;i<this->arr_size;i++)
	{
		Btexture_lib[i] = this->texture_lib[i];
	}
	
	this->arr_free();
	this->arr_init(new_size);
	
	for(int i=0;i<this->arr_size;i++)
	{
		this->texture_lib[i] = Btexture_lib[i];
	}
	
	delete [] Btexture_lib;
}

void texture_array::arr_free()
{
	for(int i=0;i<this->arr_size;i++)
	{
		SDL_DestroyTexture(this->texture_lib[i]);
	}
	delete [] this->texture_lib;
	this->arr_size = 0;
}

void texture_array::load_from_surface(int index, SDL_Renderer * renderer, SDL_Surface * surface)
{
	texture_lib[index] = SDL_CreateTextureFromSurface(renderer,surface);
	if(this->texture_lib[index] == NULL)
	{
		printf( "[PAINTER][E] - SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError() );
	}
}

void texture_array::load_bmp(int index, SDL_Renderer * renderer,char * file)
{
	SDL_Surface * temp = SDL_LoadBMP_RW(SDL_RWFromFile(file, "rb"), 1);
	if( temp == NULL )
	{
		printf( "[PAINTER][E] SDL_LoadBMP_RW Error: %s (file:'%s')\n", SDL_GetError() , file );
	}
	
	this->texture_lib[index] = SDL_CreateTextureFromSurface(renderer,temp);
	if(this->texture_lib[index] == NULL)
	{
		printf( "[PAINTER][E] - SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError() );
	}
}

painter::painter()
{
	printf("[PAINTER][L] - Initialization...\n");
	if(!this->basic_init())
	{
		printf("[PAINTER][E] - Initialization Fail\n");
	}
	printf("[PAINTER][L] - Initialization Done!\n");
}

painter::~painter()
{
	this->quit();
}

bool painter::basic_init()
{
	Uint32 flags = SDL_WINDOW_RESIZABLE;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "[PAINTER][E] - SDL_INIT_VIDEO Error: %s\n", SDL_GetError() );
		return false;
	}
	
	this->base_window = SDL_CreateWindow( "CALAMITY dev0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, flags );
	if( this->base_window == NULL )
	{
		printf( "[PAINTER][E] - SDL_CreateWindow Error: %s\n", SDL_GetError() );
		return false;
	}
	
	this->base_renderer = SDL_CreateRenderer(this->base_window, -1, SDL_RENDERER_ACCELERATED );
	if (base_renderer == NULL)
	{
		printf("[PAINTER][E] - SDL_CreateRenderer Error: %s", SDL_GetError() );
		return false;
	}

	this->base_surface = SDL_GetWindowSurface( this->base_window );

	return true;
}
	
void painter::quit()
{
	SDL_DestroyRenderer(base_renderer);
	base_renderer = NULL;
	SDL_FreeSurface(base_surface);
	base_surface = NULL;

	SDL_DestroyWindow(base_window);
	base_window = NULL;

	SDL_Quit();
	printf("[PAINTER][L] - Succesfully Quit!\n");
}
	
void painter::lock(SDL_Surface * screen)
{
	if(SDL_MUSTLOCK(screen))
	{
		if(SDL_LockSurface(screen)<0)
		{
			fprintf(stderr, "[PAINTER][E] - SDL_LockSurface Error: %s\n", SDL_GetError());
			return;
		}
   	}
}
	
void painter::unlock(SDL_Surface * screen)
{
	if(SDL_MUSTLOCK(screen))
	{
       	SDL_UnlockSurface(screen);
    }
}

void painter::clean()
{
	SDL_RenderClear(this->base_renderer);
}

void painter::update()
{
	SDL_RenderPresent(this->base_renderer);
}

void painter::draw_texture(draw_object * obj,int frameshift)
{
	SDL_RenderCopy(this->base_renderer, obj->texture.texture_lib[obj->frame/obj->step], NULL, &obj->rect);
	obj->frame+=frameshift;
	if(obj->frame >= obj->texture.arr_size*obj->step)
	{
		obj->frame = 0;
	}
}