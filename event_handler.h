#include "renderer.h"

class event_handler
{
private:
	SDL_Event event;
	renderer R;
	
	bool doloop = true;

	bool mouse_pressed = false;
	
	bool edit_mode = true;
	
	SDL_MouseMotionEvent mouse_motion;
	SDL_MouseMotionEvent last_motion;
	
    SDL_MouseButtonEvent mouse_click;
    SDL_MouseWheelEvent mouse_wheel;
    SDL_KeyboardEvent key;
    
    position* edit_pos = NULL;
    
    text* edit_text_source;
    
    void catch_box_to_edit();
    
    void update_button(SDL_Point last_mouse_pos, button* b);
    
    void move_box_edit_mode(SDL_Point mouse_pos, SDL_Point delta);
public:
	~event_handler();
	
	void init();
	
	void update_ui();
	
	void handle_events();
	
	void loop();
};