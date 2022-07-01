#include "librarian.h"

class creator
{
private:
	int chunk_size_a; //a = any direction, x y and z
	int world_size_a;
	
	struct chunk blocklib;
	
	struct world World;
public:
	librarian Librarian;
	creator();
	
	void dev_blocks_gen();
	
	void dev_chunk_gen(chunk &dest);
};