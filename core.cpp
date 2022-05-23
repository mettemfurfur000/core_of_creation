#include <SDL.h>
#include <stdio.h>

#include "librarian.h"
#include "painter.h"

int main(int argc, char* argv[])
{
	printf("%d\n",sizeof(painter));
	painter m;
	SDL_bool loopShouldStop = SDL_FALSE;

	draw_object box;
	box.texture.arr_init(1);
	box.texture.load_bmp(0,m.base_renderer,"textures\\boulder.bmp");
	
	box.rect.h=64;
	box.rect.w=64;
	box.rect.x=m.SCREEN_WIDTH/2-box.rect.h/2;
	box.rect.y=m.SCREEN_HEIGHT/2-box.rect.w/2;
	
	box.step = 1;
	
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
        m.draw_texture(&box,0);
        m.update();
        
        state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_X]) 
		{
		    printf("[L] - X key pressed, let's exit!.\n");
		    break;
		}
        SDL_Delay(16);
    }

    return 0;
}