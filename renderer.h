#include "libraries.h"
#include "json_io.h"

class renderer
{
private:
	bool sdl_init();
	void move_box_relative_to_other_box(box *b,SDL_Rect rel);
public:
	window W;
	texture_lib T;
	font_lib F;
	
	SDL_Window * base_window = NULL;
	SDL_Renderer * base_renderer = NULL;
	
	renderer();
	~renderer();
	
	void render(window* w);
	void render(menu* m);
	void render(box* b);
	void render(box* b, SDL_Rect rel_rect);
	void render(box* b, int color_shift, SDL_Rect rel_rect);
	void render(text* t, bool do_render_box, SDL_Rect rel_rect);
	void render(button* b, SDL_Rect rel_rect);
	
	void clear();
	
	void screen_update();
};
