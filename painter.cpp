#include "painter.h"

painter::painter()
{
	printf("[PAINTER][L] - Initialization...\n");
	if(!this->basic_init())
	{
		printf("[PAINTER][E] - SDL2 Initialization Fail\n");
		exit(1004);
	}
	this->block_tex_lib.init(128);
	this->block_tex_lib.base_renderer = this->base_renderer;
	
	this->block_tex_lib.automatic_load("blocks\\");
	
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
	
void painter::lock(SDL_Surface *screen)
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
	
void painter::unlock(SDL_Surface *screen)
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

void painter::simple_draw(int x,int y,int w,int h,SDL_Texture * tex)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.h = h;
	r.w = w;
	SDL_RenderCopy(this->base_renderer,tex,NULL,&r);
}

void painter::dev_draw_all(tex_lib &source)
{
	int seg_x = this->SCREEN_WIDTH/16;
	int seg_y = this->SCREEN_HEIGHT/16;
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<16;j++)
		{
			if(j+i*16 > source.size) return;
			simple_draw(i*seg_x,j*seg_y,seg_x,seg_y,source.textures[j+i*16]);
		}
	}	
}