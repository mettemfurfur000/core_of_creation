#include "main_creature.h"

void main_creature::load_main_menu()
{
	if(!P.D.load_menu("MAIN_MENU","menusaves"))
	{
		doloop = false;
	}
}

void main_creature::handle_all_events()
{
	while(SDL_PollEvent(&event)) //this loop handle all possible events!
	{
		if(event.type == SDL_QUIT)
		{
			doloop = false; //will stop main loop
		}
		
		if(event.type == SDL_WINDOWEVENT_RESIZED)
		{
			SDL_GetWindowSize(P.base_window,&P.SCREEN_WIDTH,&P.SCREEN_HEIGHT);
		}
		
		if(event.type == SDL_WINDOWEVENT_RESIZED)
		{
			SDL_GetWindowSize(P.base_window,&P.SCREEN_WIDTH,&P.SCREEN_HEIGHT);
		}
		
		if(P.D.edit_mode)
		{
			if(P.D.edited_text)
			{
				if(event.type == SDL_TEXTINPUT)
				{
					P.D.edited_text->text += event.text.text;
					std::cout << " > " << P.D.edited_text->text << std::endl;
				}
				
				if(event.type == SDL_KEYDOWN)
				{
					if(event.key.keysym.sym == SDLK_BACKSPACE && P.D.edited_text->text.size()) 
					{
						P.D.edited_text->text.pop_back();
						std::cout << " > " << P.D.edited_text->text << std::endl;
					}
					
					if(event.key.keysym.sym == SDLK_RETURN) 
					{
						P.D.edited_text->text += '\n';
						std::cout << " > " << P.D.edited_text->text << std::endl;
					}
				}
			}
		}
	}
}

int main_creature::main_loop()
{
	while(doloop)
	{
		handle_all_events();
		
		P.clean();
		
        P.menustack_draw();
        
        P.update();
        		
		SDL_Delay(15);
    }
    exit();
	return 0;
}

void main_creature::exit()
{
	P.D.save_menu("MAIN_MENU","menusaves");
    
    P.quit();
}