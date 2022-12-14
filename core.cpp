#include "event_handler.h"

/*
			core
			│
			event_handler
			│
			renderer
		┌───┴───────┐
		libraries	json_io
					│
					ui_struct
					│
					lua_worker
*/

int main(int argc, char* argv[])
{
	std::cout << "im alive!\n" << std::endl;
	
	event_handler H;
	
	H.init();
	
	H.loop();

    return 0;
}