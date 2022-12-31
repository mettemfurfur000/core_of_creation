#include "event_handler.h"

bool check_edit_mode_file()
{
	FILE* f = fopen("edit_mode.txt","rb");
	
	if(!f) return false;
	
	char buf[256];
	
	fgets(buf,256,f);
	
	fclose(f);
	
	return !strcmp("91082359823159825698723539875623895235",buf);
}

event_handler::~event_handler()
{
	int size = R.W.menus.size();
	for(int i=0;i<size;i++)
	{
		saveMenu(&R.W.menus[i],"menusaves",R.W.menus[i].name);
	}
}

void event_handler::catch_box()
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
				
				if(R.W.menus[i].buttons[j].text_part.editable == false) return;
				
				if(this->mouse_click.button == SDL_BUTTON_LEFT)
				{
					edit_text_source = &R.W.menus[i].buttons[j].text_part;
					
					SDL_StartTextInput();
				}
				
				return;
			}
		}
		bsize = R.W.menus[i].texts.size();
		for(int j=0;j<bsize;j++)
		{
			if(SDL_PointInRect(&mouse,&R.W.menus[i].texts[j].text_box.pos.real_rect))
			{
				this->edit_pos = &R.W.menus[i].texts[j].text_box.pos;
				
				if(R.W.menus[i].texts[j].editable == false) return;
				
				if(this->mouse_click.button == SDL_BUTTON_LEFT)
				{
					edit_text_source = &R.W.menus[i].texts[j];
					
					SDL_StartTextInput();
				}
				
				return;
			}
		}
		bsize = R.W.menus[i].images.size();
		for(int j=0;j<bsize;j++)
		{
			if(SDL_PointInRect(&mouse,&R.W.menus[i].images[j].pos.real_rect))
			{
				this->edit_pos = &R.W.menus[i].images[j].pos;
				
				if(edit_text_source) edit_text_source = NULL;

				if(SDL_IsTextInputActive) SDL_StopTextInput();
				
				return;
			}
		}
	}
	
	//no poses find
	if(edit_pos) edit_pos = NULL;
	
	if(edit_text_source) edit_text_source = NULL;

	if(SDL_IsTextInputActive) SDL_StopTextInput();
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
	
	tsize = R.W.menus.size();
	for(int i=0;i<tsize;i++)
	{
		bsize = R.W.menus[i].buttons.size();
		for(int j=0;j<bsize;j++)
		{
			update_button(mouse_pos,&R.W.menus[i].buttons[j]);
		}
	}
	
	if(!edit_mode) return;
	
	delta.x = mouse_motion.x - last_motion.x;
	delta.y = mouse_motion.y - last_motion.y;
	
	if(edit_pos) move_box_edit_mode(mouse_pos,delta);
	
	last_motion = mouse_motion;
}

void event_handler::update_button(SDL_Point last_mouse_pos, button* b)
{
	if(b->locked) return;
	
	if(!SDL_PointInRect(&last_mouse_pos,&b->text_part.text_box.pos.real_rect))
	{
		b->focused = false;
		return;
	}
	b->focused = true;
	//for single click registrating, not the button holding
	if(!mouse_pressed)
	{
		b->pressed = false;
		return;
	}
	
	b->click = !b->pressed;
	
	b->pressed = true;
	
	if(!b->click)
	{
		return;
	}
	//
	int t = b->scriptname.size();
	
	if(t < 5) return;
	
	std::string sub = b->scriptname.substr(0,t-4);
	std::string funcname = sub + "_bclk";
	
	luabridge::LuaRef f = R.W.LW.getGlobal(funcname.c_str());
	
	*b = f(last_mouse_pos,b,&R.W);
}

void event_handler::move_box_edit_mode(SDL_Point mouse_pos, SDL_Point delta)
{
	if(!edit_pos) return;

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
				catch_box();
				break;
				
			case SDL_MOUSEBUTTONUP:
				this->mouse_click = event.button;
				this->mouse_pressed = false;
				edit_pos = 0;
				break;
				
			case SDL_KEYDOWN:
				this->key = event.key;
				state = SDL_GetKeyboardState(NULL);
				
				if(!edit_text_source) break;
				
				if(state[SDL_SCANCODE_BACKSPACE])
				{
					if(edit_text_source->text.size() == 0) break;
					
					edit_text_source->text.pop_back();
					
					edit_text_source->updated = true;
				}
				
				if( state[SDL_SCANCODE_C] && SDL_GetModState() & KMOD_CTRL )
                {
                	SDL_SetClipboardText( edit_text_source->text.c_str() );
                }
                
                if( state[SDL_SCANCODE_V] && SDL_GetModState() & KMOD_CTRL )
                {
                	edit_text_source->text = SDL_GetClipboardText();
                	
                    edit_text_source->updated = true;
                }
				
				break;
				
			case SDL_KEYUP:
				this->key = event.key;
				break;
				
			case SDL_TEXTINPUT:
                if(!edit_text_source) break;

                edit_text_source->text += event.text.text;
                
                edit_text_source->updated = true;
                break;
		}
	}
}

void event_handler::init()
{
	edit_mode = check_edit_mode_file();
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