#include "painter.h"

painter::painter()
{
	printf("[PAINTER][L] - Initialization...\n");
	if(!basic_init())
	{
		printf("[PAINTER][E] - SDL2 Initialization Fail\n");
		exit(1004);
	}
	
	block_tex_lib.init(128);
	block_tex_lib.base_renderer = this->base_renderer;
	block_tex_lib.automatic_load("blocks\\");
	
	normal_fonts.init(4,18);
	normal_fonts.automatic_load("fonts\\");
	
	windowrect.x = 0;
	windowrect.y = 0;
	windowrect.h = SCREEN_HEIGHT;
	windowrect.w = SCREEN_WIDTH;
	
	printf("[PAINTER][L] - Initialization Done!\n");
}

bool painter::basic_init()
{
	Uint32 flags = SDL_WINDOW_RESIZABLE;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "[PAINTER][E] - SDL_INIT_VIDEO Error: %s\n", SDL_GetError() );
		return false;
	}
	
	if( TTF_Init() < 0)
	{
		printf("[PAINTER][E] - SDL2_TTF Initialization Fail: %s\n",SDL_GetError());
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
	
	this->normal_fonts.close_all();
	
	SDL_Quit();
	TTF_Quit();
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
	box_draw(windowrect,0,0,0,0);
	
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

void painter::box_draw(SDL_Rect &box,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
	SDL_SetRenderDrawColor(this->base_renderer,r,g,b,a);
	SDL_RenderFillRect(this->base_renderer,&box);
}

void painter::box_draw(SDL_Rect &box,SDL_Color &color)
{
	SDL_SetRenderDrawColor(this->base_renderer,color.r,color.g,color.b,color.a);
	SDL_RenderFillRect(this->base_renderer,&box);
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

void painter::menu_draw(menu Menu)
{
	box_draw(Menu.shape,Menu.border_color);
	
	SDL_Rect temp = Menu.shape;
	
	temp.h -= Menu.border_thickness*2;
	temp.w -= Menu.border_thickness*2;
	temp.x += Menu.border_thickness;
	temp.y += Menu.border_thickness;
	
	box_draw(temp,Menu.menu_color);
}

void painter::menustack_draw(menu **mstack,int size)
{
	for(int i = 0;i<size;i++)
	{
		if(mstack[i]!=NULL) menu_draw(*mstack[i]);
	}
}