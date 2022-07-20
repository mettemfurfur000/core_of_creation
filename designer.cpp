#include "designer.h"

/*
	bool locked;
	bool focused;
	bool pressed;
*/

void button::update()
{
	int x,y;
	SDL_PumpEvents();
	Uint32 state = SDL_GetMouseState(&x,&y);
	
	if(x >= pos_x && x <= pos_x + size_x)
	{
		if(y >= pos_y && y <= pos_y + size_y)
		{
			focused = true;
			if((state & SDL_BUTTON_LMASK) != 0) 
			{
				pressed = true;
				return;
			}
			pressed = false;
		}
	}
	focused = false;
}