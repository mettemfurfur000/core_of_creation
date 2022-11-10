#include "renderer.h"

bool renderer::sdl_init()
{
	Uint32 flags = SDL_WINDOW_RESIZABLE;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "[RENDERER][E] - SDL_INIT_VIDEO Error: %s\n", SDL_GetError() );
		return false;
	}
	
	if( TTF_Init() < 0)
	{
		printf("[RENDERER][E] - SDL2_TTF Initialization Fail: %s\n",SDL_GetError());
		return false;
	}
	
	this->base_window = SDL_CreateWindow( "CALAMITY dev0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->windowrect.w, this->windowrect.h, flags );
	if( this->base_window == NULL )
	{
		printf( "[RENDERER][E] - SDL_CreateWindow Error: %s\n", SDL_GetError() );
		return false;
	}
	
	this->base_renderer = SDL_CreateRenderer(this->base_window, -1, SDL_RENDERER_ACCELERATED );
	if (base_renderer == NULL)
	{
		printf("[RENDERER][E] - SDL_CreateRenderer Error: %s", SDL_GetError() );
		return false;
	}
	
	SDL_StartTextInput();
	
	return true;
}

renderer::renderer()
{
	printf("[RENDERER] - Initialization...\n");
	if(!sdl_init())
	{
		printf("[RENDERER][E] - SDL2 Initialization Fail\n");
		exit(1004);
	}
	
	SDL_SetRenderDrawBlendMode(this->base_renderer,SDL_BLENDMODE_BLEND);
	
	T.setRenderer(this->base_renderer);
	T.load_folder("blocks\\");
	
	printf("[RENDERER] - Initialization Done!\n");
}

renderer::~renderer()
{
	F.closeAll();
	SDL_DestroyRenderer(base_renderer);
	base_renderer = NULL;

	SDL_DestroyWindow(base_window);
	base_window = NULL;
	
	SDL_StopTextInput();
	SDL_Quit();
	TTF_Quit();
	printf("[PAINTER] - Succesfully Quit!\n");
}

void renderer::render(window* w)
{
	int size = w->menus.size();
	for(int i=0;i<size;i++)
	{
		render(&w->menus[i]);
	}
}

void renderer::render(menu* m)
{
	int tsize;
	
	render(&m->menu_box);
	render(&m->drag_zone,m->menu_box.shape);
	
	tsize = m->texts.size();
	
	for(int i=0;i<tsize;i++)
	{
		render(&m->texts[i],true,m->menu_box.shape);
	}
	
	tsize = m->buttons.size();
	
	for(int i=0;i<tsize;i++)
	{
		render(&m->buttons[i],m->menu_box.shape);
	}
}

void renderer::render(box* b)
{
	SDL_Rect t = b->shape;
	
	SDL_SetRenderDrawColor(
		this->base_renderer,
		b->border_color.r,
		b->border_color.g,
		b->border_color.b,
		b->border_color.a
	);
	
	SDL_RenderFillRect(this->base_renderer,&t);

	t.h -= b->border_th*2;
	t.w -= b->border_th*2;
	t.x += b->border_th;
	t.y += b->border_th;
	
	SDL_SetRenderDrawColor(
		this->base_renderer,
		b->color.r,
		b->color.g,
		b->color.b,
		b->color.a
	);
	
	SDL_RenderFillRect(this->base_renderer,&t);
}

void renderer::render(box* b, SDL_Rect rel_rect)
{
	SDL_Rect t = b->shape;
	t += rel_rect;
	
	SDL_SetRenderDrawColor(
		this->base_renderer,
		b->border_color.r,
		b->border_color.g,
		b->border_color.b,
		b->border_color.a
	);
	
	SDL_RenderFillRect(this->base_renderer,&t);

	t.h -= b->border_th*2;
	t.w -= b->border_th*2;
	t.x += b->border_th;
	t.y += b->border_th;
	
	SDL_SetRenderDrawColor(
		this->base_renderer,
		b->color.r,
		b->color.g,
		b->color.b,
		b->color.a
	);
	
	SDL_RenderFillRect(this->base_renderer,&t);
}

void renderer::render(box* b, int color_shift, SDL_Rect rel_rect)
{
	SDL_Rect t = b->shape;
	t += rel_rect;
	
	SDL_Color t_color = b->border_color;
	
	if(color_shift)
	{
		t_color.r += color_shift;
		t_color.g += color_shift;
		t_color.b += color_shift;
	}
	
	SDL_SetRenderDrawColor(
		this->base_renderer,
		t_color.r,
		t_color.g,
		t_color.b,
		t_color.a
	);
	
	SDL_RenderFillRect(this->base_renderer,&t);
	
	t.h -= b->border_th*2;
	t.w -= b->border_th*2;
	t.x += b->border_th;
	t.y += b->border_th;
	
	SDL_SetRenderDrawColor(
		this->base_renderer,
		b->color.r,
		b->color.g,
		b->color.b,
		b->color.a
	);
	
	SDL_RenderFillRect(this->base_renderer,&t);
}

void renderer::render(button* b, SDL_Rect rel_rect)
{
	render(&b->text_part.text_box, b->locked * (b->focused*70 - b->pressed*40),rel_rect);
	
	render(&b->text_part,false,rel_rect);
}

void renderer::render(text* t,bool do_render_box, SDL_Rect rel_rect)
{
	const SDL_Color defcolor = {255,255,255,255};
	
	SDL_Rect strrect,tshape;
	
	if(!t->font) //where fucking pointer?
	{
		t->font = F.guarantee_font(t->font_name,t->font_size);
	}
	
	if(do_render_box) render(&t->text_box,rel_rect); //render box (if asked)

	if(!t->lazy_texture || t->updated)
	{
		//text rendering
		SDL_Surface * tmp = TTF_RenderText_Blended_Wrapped(
			t->font,
			t->text.c_str(),
			defcolor,
			t->text_box.shape.w
		);
		
		t->lazy_texture = SDL_CreateTextureFromSurface(this->base_renderer, tmp);
		
		SDL_FreeSurface(tmp);
	}
	
	tshape = t->text_box.shape;
	
	tshape += rel_rect;
	
	//take texture width and height
	SDL_QueryTexture(t->lazy_texture, NULL, NULL, &strrect.w, &strrect.h);
	
	//calcing position
	
	strrect.x = tshape.x;
	strrect.y = tshape.y;
	
	if(t->centered)
	{
		strrect.x += (tshape.w - strrect.w) / 2;
		strrect.y += (tshape.h - strrect.h) / 2;
	}
	
	SDL_RenderCopy(this->base_renderer,t->lazy_texture,0,&strrect);
}

void renderer::clear()
{
	SDL_SetRenderDrawColor(this->base_renderer,0,0,0,255);
	SDL_RenderFillRect(this->base_renderer,&this->windowrect);
	
	SDL_RenderClear(this->base_renderer);
}

void renderer::screen_update()
{
	SDL_RenderPresent(this->base_renderer);
}
