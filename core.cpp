#include "event_handler.h"

int main(int argc, char* argv[])
{
	lua_test();
	
	std::cout << "im alive!\n" << std::endl;
	
	main_looper M;
	
	M.init();
	
	M.loop();

    return 0;
}