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
	
	bool edit_mode = false;
	
	SDL_Point drag_point;
	
	SDL_MouseMotionEvent mouse_motion;
    SDL_MouseButtonEvent mouse_click;
    SDL_MouseWheelEvent mouse_wheel;
    SDL_KeyboardEvent key;
    
    void update_ui_state();
    
    void update_button(button* b);
    
    void move_evement(box* b);
public:
	~main_looper();
	
	void init();
	
	void handle_events();
	
	void loop();
};