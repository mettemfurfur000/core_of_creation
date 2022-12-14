#include "event_handler.h"

event_handler::~event_handler()
{
	int size = R.W.menus.size();
	for(int i=0;i<size;i++)
	{
		saveMenu(&R.W.menus[i],"menusaves",R.W.menus[i].name);
	}
}

void event_handler::catch_box_to_edit()
{
	int tsize;
	int bsize;
	
	SDL_Point mouse;
	mouse.x = this->mouse_motion.x;
	mouse.y = this->mouse_motion.y;

	tsize = this->R.W.menus.size();
	for(int i=0;i<tsize;i++)
	{
		bsize = R.W.menus[i].buttons.size();
		for(int j=0;j<bsize;j++)
		{
			if(SDL_PointInRect(&mouse,&R.W.menus[i].buttons[j].text_part.text_box.pos.real_rect))
			{
				this->edit_pos = &R.W.menus[i].buttons[j].text_part.text_box.pos;
				edit_pos->drag = true;
				return;
			}
		}
		bsize = R.W.menus[i].texts.size();
		for(int j=0;j<bsize;j++)
		{
			if(SDL_PointInRect(&mouse,&R.W.menus[i].texts[j].text_box.pos.real_rect))
			{
				this->edit_pos = &R.W.menus[i].texts[j].text_box.pos;
				edit_pos->drag = true;
				return;
			}
		}
		bsize = R.W.menus[i].images.size();
		for(int j=0;j<bsize;j++)
		{
			if(SDL_PointInRect(&mouse,&R.W.menus[i].images[j].pos.real_rect))
			{
				this->edit_pos = &R.W.menus[i].images[j].pos;
				edit_pos->drag = true;
				return;
			}
		}
	}
	edit_pos = NULL;
}

void event_handler::update_ui()
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
	
	tsize = R.W.menus.size();
	for(int i=0;i<tsize;i++)
	{
		bsize = R.W.menus[i].buttons.size();
		for(int j=0;j<bsize;j++)
		{
			update_button(mouse_pos,&R.W.menus[i].buttons[j]);
		}
	}
	
	if(edit_mode && edit_pos) //if edit mode is ON and edit_pos pointer not null (exist!)
	{
		move_box_edit_mode(mouse_pos,delta);
	}
	
	last_motion = mouse_motion;
}

void event_handler::update_button(SDL_Point last_mouse_pos, button* b)
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
				int t = b->scriptname.size();
				std::string sub = b->scriptname.substr(0,t-4);
				std::string funcname = sub + "_bclk";
				
				luabridge::LuaRef f = this->R.W.LW.getGlobal(funcname.c_str());
				
				*b = f(last_mouse_pos.x,last_mouse_pos.y,b);
			} 
			
			return;
		}
		b->pressed = false;
		return;
	}
	b->focused = false;
}

void event_handler::move_box_edit_mode(SDL_Point mouse_pos, SDL_Point delta)
{
	if(!edit_pos) return;
	if(!edit_pos->drag) return;

	if(delta.x == 0 && delta.y == 0) return; //nothing to update
	
	if(edit_pos->fixed_pos)
	{
		edit_pos->shape.x += delta.x;
		edit_pos->shape.y += delta.y;
	}else{
		if(edit_pos->delta_mode)
		{
			edit_pos->d_x += delta.x;
			edit_pos->d_y += delta.y;
		}else{
			edit_pos->rel_perc_w += (float)delta.x / (edit_pos->relative_rect.w);
			edit_pos->rel_perc_h += (float)delta.y / (edit_pos->relative_rect.h);
		}
	}
}

void event_handler::handle_events()
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
        				SDL_GetWindowSize(R.base_window,&R.W.windowrect.w,&R.W.windowrect.h);
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
				edit_pos = 0;
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

void event_handler::init()
{
	register_things(this->R.W.LW.L);
	menu main_menu;
	loadMenu(&R.W,&main_menu,"menusaves","MAIN_MENU");
	R.W.menus.push_back(main_menu);
}

void event_handler::loop()
{
	while(doloop)
	{
		handle_events();
		
		update_ui();
        
        R.clear();
        
        R.render(&R.W);
        
        R.screen_update();
        		
		SDL_Delay(16);
    }
}