#include "ui_struct.h"
#include "renderer.h"

class main_looper
{
private:
	SDL_Event event;
	window W;
	renderer R;
	
	bool doloop = true;

	bool mouse_pressed = false;
	
	bool edit_mode = true;
	
	SDL_Point drag_point;
	
	SDL_MouseMotionEvent mouse_motion;
	SDL_MouseMotionEvent last_motion;
	
    SDL_MouseButtonEvent mouse_click;
    SDL_MouseWheelEvent mouse_wheel;
    SDL_KeyboardEvent key;
    
    void update_button(SDL_Point last_mouse_pos,button* b);
    
    void edit_box(SDL_Point mouse_pos, SDL_Point last_mouse_pos,box* b);
public:
	~main_looper();
	
	void init();
	
	void update_ui();
	
	void handle_events();
	
	void loop();
};