#include "event_handler.h"

main_looper::~main_looper()
{
	int size = W.menus.size();
	for(int i=0;i<size;i++)
	{
		W.menus[i].save("menusaves",W.menus[i].name);
	}
}

void main_looper::update_ui()
{
	SDL_Point mouse_pos;
	SDL_Point last_mouse_pos;
	
	int tsize;
	int bsize;
	//
	mouse_pos.x = mouse_motion.x;
	mouse_pos.y = mouse_motion.y;
	
	last_mouse_pos.x = last_motion.x;
	last_mouse_pos.y = last_motion.y;
	
	tsize = this->W.menus.size();
	for(int i=0;i<tsize;i++)
	{
		bsize = W.menus[i].buttons.size();
		for(int j=0;j<bsize;j++)
		{
			update_button(mouse_pos,&W.menus[i].buttons[j]);
		}
	}
	
	if(edit_mode && mouse_pressed)
	{
		tsize = this->W.menus.size();
		for(int i=0;i<tsize;i++)
		{
			bsize = W.menus[i].buttons.size();
			for(int j=0;j<bsize;j++)
			{
				edit_box(mouse_pos,last_mouse_pos,&W.menus[i].buttons[j].text_part.text_box);
			}
			
			bsize = W.menus[i].texts.size();
			for(int j=0;j<bsize;j++)
			{
				edit_box(mouse_pos,last_mouse_pos,&W.menus[i].texts[j].text_box);
			}
		}
	}
	this->last_motion = this->mouse_motion;
}

void main_looper::update_button(SDL_Point last_mouse_pos,button* b)
{
	if(b->locked) return;
	
	if(SDL_PointInRect(&last_mouse_pos,&b->text_part.text_box.pos.real_rect))
	{
		b->focused = true;
		if(this->mouse_pressed)
		{
			b->pressed = true;
			return;
		}
		b->pressed = false;
		return;
	}
	b->focused = false;
}

void main_looper::edit_box(SDL_Point mouse_pos, SDL_Point last_mouse_pos,box* b)
{
	if(!SDL_PointInRect(&mouse_pos,&b->pos.real_rect)) return;
	
	int dx = mouse_pos.x - last_mouse_pos.x;
	int dy = mouse_pos.y - last_mouse_pos.y;
	
	if(dx == 0 && dy == 0) return; //nothing to update
	
	if(b->pos.fixed_pos)
	{
		b->pos.shape.x += dx;
		b->pos.shape.y += dy;
		printf("new pos:%d %d\n",b->pos.shape.x,b->pos.shape.y);
	}else{
		if(b->pos.delta_mode)
		{
			b->pos.d_x += dx;
			b->pos.d_y += dy;
			printf("new delta:%d %d\n",b->pos.d_x,b->pos.d_y);
		}else{
			b->pos.perc_w += (float)dx / (b->pos.relative_rect.w);
			b->pos.perc_h += (float)dy / (b->pos.relative_rect.h);
			printf("new percent:%f %f\n",b->pos.perc_w,b->pos.perc_h);
		}
	}
}

void main_looper::handle_events()
{
	const Uint8 *state;
	while(SDL_PollEvent(&event)) //this loop handle all possible events!
	{
		switch(event.type)
		{
			case SDL_QUIT:
				doloop = false; //will stop main loop
				break;
				
			case SDL_WINDOWEVENT:
        		switch (event.window.event)
        		{
        			case SDL_WINDOWEVENT_SIZE_CHANGED:
        				SDL_GetWindowSize(R.base_window,&R.windowrect.w,&R.windowrect.h);
        				break;
				}
				break;
				
			case SDL_MOUSEMOTION:
				this->mouse_motion = event.motion;
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				this->mouse_click = event.button;
				/* if pressed */ 
				this->mouse_pressed = true;
				break;
				
			case SDL_MOUSEBUTTONUP:
				this->mouse_click = event.button;
				this->mouse_pressed = false;
				break;
				
			case SDL_KEYDOWN:
				this->key = event.key;
				state = SDL_GetKeyboardState(NULL);
				if(state[SDL_SCANCODE_X]) 
				{
				    printf("X Key Pressed.\n");
				    doloop = false;
				}
				break;
				
			case SDL_KEYUP:
				this->key = event.key;
				break;
		}
	}
}

void main_looper::init()
{
	W.loadMainMenu();
}

void main_looper::loop()
{
	while(doloop)
	{
		handle_events();
		
		update_ui();
        
        R.clear();
        
        R.render(&W);
        
        R.screen_update();
        		
		SDL_Delay(16);
    }
}







