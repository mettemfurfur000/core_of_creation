#include "painter.h"
#include "creator.h"

class main_creature
{
private:
	SDL_Event event;
	bool doloop = true;
public:
	painter P;
	creator C;
	void handle_all_events();
	int main_loop();
	void load_main_menu();
	void exit();
};