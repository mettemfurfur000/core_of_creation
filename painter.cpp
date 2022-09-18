#include "painter.h"

painter::painter()
{
	printf("[PAINTER][L] - Initialization...\n");
	if(!basic_init())
	{
		printf("[PAINTER][E] - SDL2 Initialization Fail\n");
		exit(1004);
	}
	
	SDL_SetRenderDrawBlendMode(this->base_renderer,SDL_BLENDMODE_BLEND);
	
	block_tex_lib.init(128);
	block_tex_lib.base_renderer = this->base_renderer;
	block_tex_lib.automatic_load("blocks\\");
	
	Designer.normal_fonts.init(4,18);
	Designer.normal_fonts.automatic_load("fonts\\");
	
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
	
	this->Designer.normal_fonts.close_all();
	
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
	static SDL_Color black_color = {0,0,0,255};
	
	simple_fill(this->windowrect,black_color);
	
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

void painter::simple_fill(SDL_Rect r,SDL_Color c)
{
	SDL_SetRenderDrawColor(this->base_renderer,c.r,c.g,c.b,c.a);
	SDL_RenderFillRect(this->base_renderer,&r);
}

void painter::box_draw(box b)
{
	SDL_SetRenderDrawColor(this->base_renderer,b.border_color.r,b.border_color.g,b.border_color.b,b.border_color.a);
	SDL_RenderFillRect(this->base_renderer,&b.shape);
	
	b.shape.h -= b.border_th*2;
	b.shape.w -= b.border_th*2;
	b.shape.x += b.border_th;
	b.shape.y += b.border_th;
	
	SDL_SetRenderDrawColor(this->base_renderer,b.color.r,b.color.g,b.color.b,b.color.a);
	SDL_RenderFillRect(this->base_renderer,&b.shape);
}

void painter::rel_box_draw(SDL_Rect a,box b,int border_color_shift)
{
	if(border_color_shift)
	{
		b.border_color.r+=border_color_shift;
		b.border_color.g+=border_color_shift;
		b.border_color.b+=border_color_shift;
	}
	
	b.shape.x += a.x;
	b.shape.y += a.y;
	
	SDL_SetRenderDrawColor(this->base_renderer,b.border_color.r,b.border_color.g,b.border_color.b,b.border_color.a);
	SDL_RenderFillRect(this->base_renderer,&b.shape);
	
	b.shape.h -= b.border_th*2;
	b.shape.w -= b.border_th*2;
	b.shape.x += b.border_th;
	b.shape.y += b.border_th;
	
	SDL_SetRenderDrawColor(this->base_renderer,b.color.r,b.color.g,b.color.b,b.color.a);
	SDL_RenderFillRect(this->base_renderer,&b.shape);
}

void painter::rel_box_draw(SDL_Rect a,box b)
{
	b.shape.x += a.x;
	b.shape.y += a.y;
	
	SDL_SetRenderDrawColor(this->base_renderer,b.border_color.r,b.border_color.g,b.border_color.b,b.border_color.a);
	SDL_RenderFillRect(this->base_renderer,&b.shape);
	
	b.shape.h -= b.border_th*2;
	b.shape.w -= b.border_th*2;
	b.shape.x += b.border_th;
	b.shape.y += b.border_th;
	
	SDL_SetRenderDrawColor(this->base_renderer,b.color.r,b.color.g,b.color.b,b.color.a);
	SDL_RenderFillRect(this->base_renderer,&b.shape);
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

void painter::menu_draw(menu &Menu)
{
	Menu.update();
	
	box_draw(Menu.menu_box);
	rel_box_draw(Menu.menu_box.shape,Menu.drag_zone);
	
	for(int i=0;i<Menu.texts_size;i++)
	{
		if(Menu.texts[i]) text_draw(Menu,*Menu.texts[i]);
	}
	
	for(int i=0;i<Menu.buttons_size;i++)
	{
		if(Menu.buttons[i]) button_draw(Menu,*Menu.buttons[i]);
	}
}

void painter::text_draw(menu &Menu,text &Text)
{
	//special edition!
	if(Designer.edit_mode)
	{
		Designer.edit_move_object_in_menu(Menu.menu_box,Text.text_box);
	}
	//
	
	rel_box_draw(Menu.menu_box.shape,Text.text_box);
	
	//text rendering
	SDL_Color defcolor = {255,255,255,255};
	SDL_Rect dest = {Menu.menu_box.shape.x+Text.text_box.shape.x,Menu.menu_box.shape.y+Text.text_box.shape.y,0,0};
	
	SDL_Surface * tmp = TTF_RenderText_Blended_Wrapped(Text.font,Text.text.c_str(),defcolor,Text.text_box.shape.w);
	SDL_Texture * tmptex = SDL_CreateTextureFromSurface(this->base_renderer, tmp);
	
	SDL_QueryTexture(tmptex, NULL, NULL, &dest.w, &dest.h); //get height and width of texture, store in "dest" struct
	
	SDL_RenderCopy(this->base_renderer,tmptex,0,&dest);
	
	SDL_FreeSurface(tmp);
	SDL_DestroyTexture(tmptex);
}

void painter::button_draw(menu &Menu,button &Button)
{
	SDL_Rect t = Button.text_part.text_box.shape;

	t.x = Menu.menu_box.shape.x + Button.text_part.text_box.shape.x;
	t.y = Menu.menu_box.shape.y + Button.text_part.text_box.shape.y;
	
	//special edition!
	if(Designer.edit_mode)
	{
		Designer.edit_move_object_in_menu(Menu.menu_box,Button.text_part.text_box);
	}
	//
	
	this->Designer.button_update(Button,t);
	
	rel_box_draw(Menu.menu_box.shape,Button.text_part.text_box, Button.focused ? (Button.pressed ? 100 : 50) : 0);
	
	SDL_Color defcolor = {255,255,255,255};
	SDL_Surface * tmp = TTF_RenderText_Blended_Wrapped(Button.text_part.font, Button.text_part.text.c_str(), defcolor, Button.text_part.text_box.shape.w);
	
	SDL_Texture * tmptex = SDL_CreateTextureFromSurface(this->base_renderer, tmp);
	SDL_FreeSurface(tmp);
	
	int text_h,text_w;
	SDL_QueryTexture(tmptex, NULL, NULL, &text_w, &text_h);
	
	SDL_Rect dest = {
		Menu.menu_box.shape.x+Button.text_part.text_box.shape.x+(Button.text_part.text_box.shape.w/2-text_w/2),
		Menu.menu_box.shape.y+Button.text_part.text_box.shape.y+(Button.text_part.text_box.shape.h/2-text_h/2),
		0,
		0
		};
	
	dest.w = text_w;
	dest.h = text_h;
	
	SDL_RenderCopy(this->base_renderer,tmptex,0,&dest);
	SDL_DestroyTexture(tmptex);
}

void painter::menustack_draw()
{
	for(int i = 0;i<Designer.menustack_size;i++)
	{
		if(Designer.menustack[i]) menu_draw(*Designer.menustack[i]);
	}
}