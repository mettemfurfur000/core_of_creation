#include "ui_struct.h"
#include "renderer.h"

class main_looper
{
private:
	SDL_Event event;
	bool doloop = true;
	window W;
	renderer R;
	SDL_Point mouse_pos;
public:
	~main_looper();
	
	void init();
	
	void handle_events();
	
	void loop();
};