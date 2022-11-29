#include "event_handler.h"

main_looper::~main_looper()
{
	int size = W.menus.size();
	for(int i=0;i<size;i++)
	{
		W.menus[i].save("menusaves",W.menus[i].name);
	}
}

void main_looper::catch_box_to_edit()
{
	int tsize;
	int bsize;
	
	SDL_Point mouse;
	mouse.x = this->mouse_motion.x;
	mouse.y = this->mouse_motion.y;

	tsize = this->W.menus.size();
	for(int i=0;i<tsize;i++)
	{
		bsize = W.menus[i].buttons.size();
		for(int j=0;j<bsize;j++)
		{
			if(SDL_PointInRect(&mouse,&W.menus[i].buttons[j].text_part.text_box.pos.real_rect))
			{
				this->edit_box = &W.menus[i].buttons[j].text_part.text_box;
				edit_box->pos.drag = true;
				return;
			}
		}
		bsize = W.menus[i].texts.size();
		for(int j=0;j<bsize;j++)
		{
			if(SDL_PointInRect(&mouse,&W.menus[i].texts[j].text_box.pos.real_rect))
			{
				this->edit_box = &W.menus[i].texts[j].text_box;
				edit_box->pos.drag = true;
				return;
			}
		}
	}
	edit_box = 0;
}

void main_looper::update_ui()
{
	static SDL_Point mouse_pos;
	static SDL_Point delta;
	
	static int tsize;
	static int bsize;
	//
	mouse_pos.x = mouse_motion.x;
	mouse_pos.y = mouse_motion.y;

	delta.x = mouse_motion.x - last_motion.x;
	delta.y = mouse_motion.y - last_motion.y;
	
	tsize = W.menus.size();
	for(int i=0;i<tsize;i++)
	{
		bsize = W.menus[i].buttons.size();
		for(int j=0;j<bsize;j++)
		{
			update_button(mouse_pos,&W.menus[i].buttons[j]);
		}
	}
	
	if(edit_mode && edit_box) //if edit mode is ON and edit_box pointer not null (exist!)
	{
		move_box_edit_mode(mouse_pos,delta);
	}
	
	last_motion = mouse_motion;
}

void main_looper::update_button(SDL_Point last_mouse_pos, button* b)
{
	if(b->locked) return;
	
	if(SDL_PointInRect(&last_mouse_pos,&b->text_part.text_box.pos.real_rect))
	{
		b->focused = true;
		if(this->mouse_pressed)
		{
			b->click = !b->pressed;
			
			b->pressed = true;
			
			if(b->click)
			{
				printf("single click on - %s\n",b->text_part.text.c_str());
				L.dofile(b->scriptname.c_str());
			
				luabridge::push(L.LuaState,b);
				lua_setglobal(L.LuaState, "button");
				L.call("button_click");
			} 
			
			return;
		}
		b->pressed = false;
		return;
	}
	b->focused = false;
}

void main_looper::move_box_edit_mode(SDL_Point mouse_pos, SDL_Point delta)
{
	if(!edit_box->pos.drag || !edit_box) return;

	if(delta.x == 0 && delta.y == 0) return; //nothing to update
	
	if(edit_box->pos.fixed_pos)
	{
		edit_box->pos.shape.x += delta.x;
		edit_box->pos.shape.y += delta.y;
	}else{
		if(edit_box->pos.delta_mode)
		{
			edit_box->pos.d_x += delta.x;
			edit_box->pos.d_y += delta.y;
		}else{
			edit_box->pos.rel_perc_w += (float)delta.x / (edit_box->pos.relative_rect.w);
			edit_box->pos.rel_perc_h += (float)delta.y / (edit_box->pos.relative_rect.h);
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
				this->mouse_pressed = true;
				catch_box_to_edit();
				break;
				
			case SDL_MOUSEBUTTONUP:
				this->mouse_click = event.button;
				this->mouse_pressed = false;
				edit_box = 0;
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







