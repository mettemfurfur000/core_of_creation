#include "renderer.h"

bool renderer::sdl_init()
{
	Uint32 flags = 0;//SDL_WINDOW_RESIZABLE;
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
		
	if(IMG_Init(63) == 0) //because
	{
		printf("[RENDERER][E] - IMG_Init Error: %s", SDL_GetError());
		return false;
	}
	
	this->base_window = SDL_CreateWindow( "CALAMITY dev0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->W.windowrect.w, this->W.windowrect.h, flags );
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
	F.closeAll();//free resources
	T.destroyAll();
	
	SDL_DestroyRenderer(base_renderer);
	base_renderer = NULL;

	SDL_DestroyWindow(base_window);
	base_window = NULL;
	
	SDL_StopTextInput();
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
	printf("[PAINTER] - Succesfully Quit!\n");
}

void renderer::move_pos_relative_2_rect(position *p,SDL_Rect rel)
{
	//position calc part
	if(p->fixed_pos)
	{
		//if fixed, just put it where he should be
		p->shape += rel;
		p->shape.x -= p->center.x;
		p->shape.y -= p->center.y;
	}else{
		if(p->delta_mode)
		{
			p->shape.x = rel.x + (rel.w + p->d_x)%rel.w - p->center.x;
			p->shape.y = rel.y + (rel.h + p->d_y)%rel.h - p->center.y;
		}else{
			p->shape.x = rel.x + (int)(rel.w * p->rel_perc_w) - p->center.x;
			p->shape.y = rel.y + (int)(rel.h * p->rel_perc_h) - p->center.y;
		}
	}
	//height&width calc part
	if(!p->fixed_size)
	{
		if(p->delta_size)
		{
			p->shape.h = (rel.h + p->d_h)%rel.h;
			p->shape.w = (rel.w + p->d_w)%rel.w;
		}else{
			p->shape.h = (int)(rel.h * p->rel_size_perc_h);
			p->shape.w = (int)(rel.w * p->rel_size_perc_w);
		}
	}
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
	if(m->copy_window_rect)
	{
		m->menu_box.pos.shape = this->W.windowrect;
	}
	
//first, render menu
	render(&m->menu_box,this->W.windowrect);
//images	
	tsize = m->images.size();

	for(int i=0;i<tsize;i++)
	{
		render(&m->images[i],m->menu_box.pos.shape);
	}
//texts
	tsize = m->texts.size();
	
	for(int i=0;i<tsize;i++)
	{
		render(&m->texts[i],true,m->menu_box.pos.shape);
	}
//buttons
	tsize = m->buttons.size();
	
	for(int i=0;i<tsize;i++)
	{
		render(&m->buttons[i],m->menu_box.pos.shape);
	}
}

void renderer::render(box* b)
{
	if(b->shown == false) return;
	SDL_Rect t = b->pos.shape;
	
	if(b->pos.auto_center)
	{
		b->pos.center.x = b->pos.shape.w/2;
		b->pos.center.y = b->pos.shape.h/2;
	}
	
	t.x -= b->pos.center.x;
	t.y -= b->pos.center.y;
	
	b->pos.real_rect = t;
	
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

void renderer::render(image* i, SDL_Rect rel_rect)
{
	if(i->shown == false) return;
	
	i->pos.relative_rect = rel_rect;

	if(!i->texture) //where pointer?
	{
		i->texture = this->T.guarantee_texture(i->filename);
	}
	
	position tpos = i->pos;
	
	//SDL_QueryTexture(i->texture, NULL, NULL, &tpos.shape.w, &tpos.shape.h);

	move_pos_relative_2_rect(&tpos,rel_rect);
	
	SDL_RenderCopy(this->base_renderer,i->texture,0,&tpos.shape);
	
	i->pos.real_rect = tpos.shape;
}

void renderer::render(box* b, SDL_Rect rel_rect)
{
	if(b->shown == false) return;
	
	b->pos.relative_rect = rel_rect;
	
	if(b->pos.auto_center)
	{
		b->pos.center.x = b->pos.shape.w/2;
		b->pos.center.y = b->pos.shape.h/2;
	}
	
	position tpos = b->pos;
	
	move_pos_relative_2_rect(&tpos,rel_rect);
	
	SDL_Rect t = tpos.shape;
	
	b->pos.real_rect = t;
	
	//rendering part
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
	if(b->shown == false) return;
	
	b->pos.relative_rect = rel_rect;
	
	if(b->pos.auto_center)
	{
		b->pos.center.x = b->pos.shape.w/2;
		b->pos.center.y = b->pos.shape.h/2;
	}
	
	position tpos = b->pos;
	
	move_pos_relative_2_rect(&tpos,rel_rect);
	
	SDL_Rect t = tpos.shape;
	
	b->pos.real_rect = t;
	
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
	if(b->text_part.text_box.shown == false) return;
	
	b->text_part.text_box.pos.relative_rect = rel_rect;
	
	render(&b->text_part.text_box, b->locked ? -20 : b->focused ? b->pressed ? 40 : 70 : 0 ,rel_rect);
	
	render(&b->text_part,false,rel_rect);
}

void renderer::render(text* t,bool do_render_box, SDL_Rect rel_rect)
{
	const static SDL_Color defcolor = {255,255,255,255};
	const static SDL_Color defcolor2 = {128,128,128,128};
	
	if(t->text_box.shown == false) return;
	
	if(t->text_box.pos.auto_center)
	{
		t->text_box.pos.center.x = t->text_box.pos.shape.w/2;
		t->text_box.pos.center.y = t->text_box.pos.shape.h/2;
	}
	
	t->text_box.pos.relative_rect = rel_rect;
	
	SDL_Rect strrect,tshape;
	
	if(!t->font) //where fucking pointer?
	{
		t->font = F.guarantee_font(t->font_name,t->font_size);
	}
	
	if(do_render_box) render(&t->text_box,t->selected ? 90 : 0, rel_rect); //render box (if asked)
		
	if(t->updated)
	{
		SDL_DestroyTexture(t->lazy_texture);
		
		t->lazy_texture = 0;
		
		t->updated = false;
	}
	
	if(!t->lazy_texture)
	{
		SDL_Surface * tmp;
		
		bool use_back = t->text.length() == 0 && t->use_background_text;
		
		tmp = TTF_RenderUTF8_Blended_Wrapped(
			t->font,
			( use_back ? t->background_text : t->text ).c_str(),
			( use_back ? defcolor2 : defcolor ),
			t->text_box.pos.shape.w
		);
		//text rendering
		t->lazy_texture = SDL_CreateTextureFromSurface(this->base_renderer, tmp);
		
		SDL_FreeSurface(tmp);
	}
	
	position tpos = t->text_box.pos;
	
	SDL_QueryTexture(t->lazy_texture, NULL, NULL, &tpos.shape.w, &tpos.shape.h);
	
	move_pos_relative_2_rect(&tpos,rel_rect);
	
	if(t->centered)
	{
		tpos.shape.x += (t->text_box.pos.shape.w - tpos.shape.w) / 2;
		tpos.shape.y += (t->text_box.pos.shape.h - tpos.shape.h) / 2;
	}
	
	SDL_RenderCopy(this->base_renderer,t->lazy_texture,0,&tpos.shape);
}

void renderer::clear()
{
	SDL_SetRenderDrawColor(this->base_renderer,0,0,0,255);
	SDL_RenderFillRect(this->base_renderer,&this->W.windowrect);
	
	SDL_RenderClear(this->base_renderer);
}

void renderer::screen_update()
{
	SDL_RenderPresent(this->base_renderer);
}
