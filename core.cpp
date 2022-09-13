#include "creator.h"
#include "painter.h"

int main(int argc, char* argv[])
{
	painter m;
	creator c;
	SDL_bool loopShouldStop = SDL_FALSE;

	const Uint8 *state;
	SDL_Event event;
	
	SDL_Rect temp;
	
	temp.h = m.SCREEN_HEIGHT/2;
	temp.w = m.SCREEN_WIDTH/2;
	
	temp.x = m.SCREEN_WIDTH/2 - temp.w/2;
	temp.y = m.SCREEN_HEIGHT/2 - temp.h/2;
	
	SDL_Color bcol = {0,255,64,0};
	SDL_Color mcol = {1,64,64,0};
	
	std::string mainmenu = "MAIN_MENU";
	std::string txt = "test_text\namongus";
	std::string dosmini = "DOSMINI.ttf";
	
	SDL_Rect but = {50,50,60,20};
	
	int n;
	std::cin >> n;
	if(n)
	{
		m.Designer.load_menu("MAIN_MENU","menusaves");
	}else{
		m.Designer.create_menu(mainmenu,temp,bcol,mcol,2);
		m.Designer.text_create(mainmenu,10,10,50,100,txt,dosmini);
		m.Designer.button_create(mainmenu,"boop",dosmini,but,bcol);
	}
	
	//

	//m.Designer.delete_menu("MAIN_MENU");
	

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
        
        m.menustack_draw();
        
        m.update();
        SDL_Delay(15);
        
        state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_X]) 
		{
		    printf("[L] - X key pressed, let's exit!.\n");
		    break;
		}
    }
    
	m.Designer.save_menu("MAIN_MENU","menusaves");
    
    m.quit();
    return 0;
}