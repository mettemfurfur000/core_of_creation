#include "creator.h"

creator::creator()
{
	chunk_size_a = 16;
	world_size_a = 16;
	
	World.world_name = "test_world";
	
	blocklib.size_x = 1;
	blocklib.size_y = 1;
	blocklib.size_z = 32;
}

void creator::dev_blocks_gen()
{
	for(int i=0;i<16;i++)
	{
		Librarian.block_create(blocklib.blocks[0][0][i],i+1,rand()%1000,0,"void");
	}
}

void creator::dev_chunk_gen(chunk &dest)
{
	for(int i=0;i<dest.size_x;i++)
	{
		for(int j=0;j<dest.size_y;j++)
		{
			for(int l=0;l<dest.size_z;l++)
			{
				dest.blocks[i][j][l] = blocklib.blocks[0][0][rand()%16];
			}
		}
	}
}