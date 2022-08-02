#include "creator.h"
#include "painter.h"

int main(int argc, char* argv[])
{
	painter m;
	creator c;
	designer d;
	SDL_bool loopShouldStop = SDL_FALSE;

	const Uint8 *state;
	SDL_Event event;
	
	SDL_Rect temp;
	
	temp.h = m.SCREEN_HEIGHT/2;
	temp.w = m.SCREEN_WIDTH/2;
	
	temp.x = m.SCREEN_WIDTH/2 - temp.w/2;
	temp.y = m.SCREEN_HEIGHT/2 - temp.h/2;
	
	SDL_Color bcol;
	bcol.r = 0;
	bcol.g = 255;
	bcol.b = 64;
	SDL_Color mcol;
	mcol.r = 1;
	mcol.g = 64;
	mcol.b = 64;
	
	d.create_menu("MAIN_MENU",temp,bcol,mcol,2);
	
	while (!loopShouldStop)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    loopShouldStop = SDL_TRUE;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                	SDL_GetWindowSize(m.base_window,&m.SCREEN_WIDTH,&m.SCREEN_HEIGHT);
                	break;
            }
        }
        m.clean();
        
        m.menustack_draw(d.menustack,d.menustack_size);
        
        m.update();
        SDL_Delay(15);
        
        state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_X]) 
		{
		    printf("[L] - X key pressed, let's exit!.\n");
		    break;
		}
		if (state[SDL_SCANCODE_W]) 
		{
		    d.menustack[0]->shape.y--;
		}
		if (state[SDL_SCANCODE_S]) 
		{
		    d.menustack[0]->shape.y++;
		}
		if (state[SDL_SCANCODE_A]) 
		{
		    d.menustack[0]->shape.x--;
		}
		if (state[SDL_SCANCODE_D]) 
		{
		    d.menustack[0]->shape.x++;
		}
    }
    m.quit();
    return 0;
}