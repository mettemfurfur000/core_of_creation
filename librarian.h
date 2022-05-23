#include <cstdio>
#include <cstdlib>
#include <cstring>

#pragma once

//┌─────────────────────┐//
//│*   The Librarian   *│//
//│                     │//
//│ *God  of the Known* │//
//└─────────────────────┘//

struct block
{
	short id;
	int durability;
	unsigned char storage_size;
	char *data;
	struct block *storage;
};

struct chunk
{
	int size_x;
	int size_y;
	int size_z;
	
	bool allocated = false;
	
	struct block ***blocks;
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
		//for 1-2-3d chunks
		void chunk_save(char * filename,chunk &source);
		void chunk_load(char * filename,chunk &dest);
		
		void info(block source,int level=0);
};