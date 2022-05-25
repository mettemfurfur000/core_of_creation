#include <SDL.h>
#include <stdio.h>

#include "librarian.h"
#include "painter.h"

int main(int argc, char* argv[])
{
	painter m;
	SDL_bool loopShouldStop = SDL_FALSE;

	const Uint8 *state;
	SDL_Event event;
	
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
        SDL_Delay(15);
        m.update();
        
        state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_X]) 
		{
		    printf("[L] - X key pressed, let's exit!.\n");
		    break;
		}
    }
    return 0;
}