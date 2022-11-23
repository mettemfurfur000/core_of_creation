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
	
	SDL_MouseMotionEvent mouse_motion;
	SDL_MouseMotionEvent last_motion;
	
    SDL_MouseButtonEvent mouse_click;
    SDL_MouseWheelEvent mouse_wheel;
    SDL_KeyboardEvent key;
    
    box* edit_box = 0;
    
    void catch_box_to_edit();
    
    void update_button(SDL_Point last_mouse_pos, button* b);
    
    void move_box_edit_mode(SDL_Point mouse_pos, SDL_Point delta);
public:
	~main_looper();
	
	void init();
	
	void update_ui();
	
	void handle_events();
	
	void loop();
};