#include "event_handler.h"

main_looper::~main_looper()
{
	int size = W.menus.size();
	for(int i=0;i<size;i++)
	{
		W.menus[i].save("menusaves",W.menus[i].name);
	}
}

void main_looper::update_ui_state()
{
	SDL_Point last_mouse_pos;
	int tsize = this->W.menus.size();
	int bsize;
	
	last_mouse_pos.x = this->mouse_click.x;
	last_mouse_pos.x = this->mouse_click.y;
	
	for(int i=0;i<tsize;i++)
	{
		bsize = W.menus[i].buttons.size();
		for(int j=0;j<bsize;j++)
		{
			//if(SDL_PointInRect(last_mouse_pos,))
			if(edit_mode)
			{
				if(this->mouse_pressed)
				{
					
				}
			}
			//W.menus[i].buttons[j]
		}
	}
}

void main_looper::update_button(button* b)
{
	
}
    
void main_looper::move_evement(box* b)
{
	
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
				if(mouse_click.button & SDL_BUTTON_LMASK) printf("[main_looper] - Mouse left button pressed!\n");
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
        
        R.clear();
        
        R.render(&W);
        
        R.screen_update();
        		
		SDL_Delay(16);
    }
}







