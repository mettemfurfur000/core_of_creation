#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "templates.hpp"

#pragma once

//┌─────────────────────┐//
//│*   The Librarian   *│//
//│                     │//
//│ *God  of the Known* │//
//└─────────────────────┘//

struct block
{
	short id = 0;
	int durability = 0;
	unsigned char storage_size = 0;
	char * data = NULL;
	struct block * storage = NULL;
};

struct chunk
{
	int size_x = 0;
	int size_y = 0;
	int size_z = 0;
	
	bool allocated = false;
	
	struct block ***blocks;
};

struct world
{
	char * world_name;

	int size_x = 0;
	int size_y = 0;
	int size_z = 0;
	
	bool allocated = false;
	
	struct chunk ***chunks;
};

class librarian
{
	private:
		int sizeofblock;
		void block_save(FILE * f,block source);
		void block_load(FILE * f,block &dest);
	public:
		librarian();
		void block_create(block &dest,int id,int durability,int storage_size,char *data);
		//for single blocks
		void save(char * filename,block source);
		void load(char * filename,block &out);
		void block_delete(block &b);
		void block_clone(block source,block &dest);
		//for 1-2-3d chunks
		void chunk_save(char * filename,chunk &source);
		void chunk_load(char * filename,chunk &dest);
		
		void chunk_allocate(chunk &dest);
		void chunk_free(chunk &source);
		
		//for worlds
		void world_save(world &source);
		void world_load(world &dest);
		
		void info(block source,int level=0);
};