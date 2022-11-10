#include "event_handler.h"

main_looper::~main_looper()
{
	int size = W.menus.size();
	for(int i=0;i<size;i++)
	{
		W.menus[i].save("menusaves",W.menus[i].name);
	}
}

void main_looper::handle_events()
{
	while(SDL_PollEvent(&event)) //this loop handle all possible events!
	{
		if(event.type == SDL_QUIT)
		{
			doloop = false; //will stop main loop
		}
		
		if(event.type == SDL_WINDOWEVENT_RESIZED)
		{
			SDL_GetWindowSize(R.base_window,&R.windowrect.w,&R.windowrect.h);
		}
		
		if(event.type == SDL_MOUSEMOTION)
		{
			this->mouse_pos.x = event.motion.x;
			this->mouse_pos.y = event.motion.y;
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
        		
		SDL_Delay(20);
    }
}