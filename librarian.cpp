#include "librarian.h"

librarian::librarian()
{
	sizeofblock = sizeof(struct block);
}

void librarian::block_create(struct block &dest,int id,int durability,int storage_size,std::string data)
{
	dest.id = id;
	dest.durability = durability;
	dest.storage_size = storage_size;
	
	dest.data = data;
	
	//storage init and voiding
	if(storage_size>0)
	{
		dest.storage = new block[storage_size];
		for(int i=0;i<storage_size;i++)
		{
			block_create(dest.storage[i],0,0,0,0);
		}
	}
}

void librarian::block_delete(block &b)
{
	if(b.storage_size>0)
	{
		for(int i=0;i<b.storage_size;i++)
		{
			block_delete(b.storage[i]);
		}
		delete[] b.storage;
	}
}

void librarian::block_save(FILE * f,struct block source)
{
	struct block *block_pointer = &source;
	char *byte_pointer = (char *)block_pointer;
	//basicly print block data, byte to byte
	for(int i=0;i<sizeofblock;i++)
	{
		putc(*byte_pointer,f);
		byte_pointer++;
	}
	//and print data string
	int data_size = source.data.length()+1;
	char * cstr = new char [data_size];
	std::strcpy (cstr, source.data.c_str());
	
	for(int i=0;i<data_size;i++)
	{
		putc(cstr[i],f);
	}
	
	delete[] cstr;
	//and check storage
	if(source.storage_size>0)
	{
		for(int i=0;i<source.storage_size;i++)
		{
			block_save(f,source.storage[i]);
		}
	}
}

void librarian::block_load(FILE * f,struct block &dest)
{
	struct block *block_pointer = &dest;
	char *byte_pointer = (char *)block_pointer;
	char c;
	
	int i = 0;
	for(int h=0;h<sizeofblock;h++)
	{
		c = getc(f);
		byte_pointer[i] = c;
		i++;
	}
	
	i = 0;
	c = getc(f);
	while(c!='\0')
	{
		dest.data[i] = c;
		i++;
		c = getc(f);
	}

	if(dest.storage_size>0)
	{
		for(int i=0;i<dest.storage_size;i++)
		{
			block_load(f,dest.storage[i]);
		}
	}
}

void librarian::block_clone(block source,block &dest)
{
	dest.id = source.id;
	dest.durability = source.durability;
	dest.storage_size = source.storage_size;
	
	dest.data = source.data;
	
	dest.storage = source.storage;
}
//chunks
void librarian::chunk_save(char * filename,chunk &source)
{
	FILE * f;
	f = fopen(filename,"wb");
	if(f==NULL)
	{
		printf("[LIBRARIAN][E] - Error occured while open(write) file\n");
		return;
	}
	
	for(int i=0;i<source.size_x;i++)
	{
		for(int j=0;j<source.size_y;j++)
		{
			for(int l=0;l<source.size_z;l++)
			{
				block_save(f,source.blocks[i][j][l]);
			}
		}
	}

	fclose(f);
}

void librarian::chunk_load(char * filename,chunk &dest)
{
	FILE * f;
	f = fopen(filename,"r");
	if(f==NULL)
	{
		printf("[LIBRARIAN][E] - Error occured while open(read) file\n");
		return;
	}
	
	for(int i=0;i<dest.size_x;i++)
	{
		for(int j=0;j<dest.size_y;j++)
		{
			for(int l=0;l<dest.size_z;l++)
			{
				block_load(f,dest.blocks[i][j][l]);
			}
		}
	}

	fclose(f);
}

void librarian::chunk_allocate(chunk &dest)
{
	if(dest.allocated==0)
	{
		dest.blocks = new block **[dest.size_x];
		for(int i=0;i<dest.size_x;i++)
		{
			dest.blocks[i] = new block *[dest.size_y];
			for(int j=0;j<dest.size_y;j++)
			{
				dest.blocks[i][j] = new block [dest.size_z];
			}
		}
	}
}

void librarian::chunk_free(chunk &source)
{
	if(source.allocated==1)
	{
		for(int i=0;i<source.size_x;i++)
		{
			for(int j=0;j<source.size_y;j++)
			{
				for(int l=0;l<source.size_z;l++)
				{
					block_delete(source.blocks[i][j][l]);
				}
				delete[] source.blocks[i][j];
			}
			delete[] source.blocks[i];
		}
		delete[] source.blocks;
	}
}

// blocks
void librarian::save(char * filename,block source)
{
	FILE * f;
	
	f = fopen(filename,"wb");
	if(f==NULL)
	{
		printf("[LIBRARIAN][E] - Error occured while open(write) file\n");
		return;
	}
	//
	block_save(f,source);
	//
	fclose(f);
}
		
void librarian::load(char * filename,block &out)
{
	FILE * f;
	
	f = fopen(filename,"r");
	if(f==NULL)
	{
		printf("[LIBRARIAN][E] - Error occured while open(read) file\n");
		return;
	}
	//
	block_load(f,out);
	//
	fclose(f);
}

//world
void librarian::world_save(world &source)
{
	if(source.world_name.length()==0)
	{
		printf("[LIBRARIAN][E] - Failed world save! world_name is null!\n");
		return;
	}
	char * filename;
	for(int i=0;i<source.size_x;i++)
	{
		for(int j=0;j<source.size_y;j++)
		{
			for(int l=0;l<source.size_z;l++)
			{
				if(source.chunks[i][j][l].allocated!=0)
				{
					sprintf(filename,"%s\\%d_%d_%d.chunk",source.world_name,i,j,l);
					chunk_save(filename,source.chunks[i][j][l]);
				}
			}
		}
	}
}

void librarian::world_load(world &dest)
{
	if(dest.world_name.length()==0)
	{
		printf("[LIBRARIAN][E] - Failed world load! world_name is null!\n");
		return;
	}
	char command[256];
	
	int x,y,z;
	
	sprintf(command,"dir /b %s > tmp.txt",dest.world_name);
	system( command );
	
	FILE * list = fopen("tmp.txt","r");
	char * filename;

	while(fscanf(list,"%d_%d_%d.chunk",&x,&y,&z) > 0)
	{
		sprintf(filename,"%d_%d_%d.chunk",x,y,z);
		chunk_load(filename,dest.chunks[x][y][z]);
		memset (filename,0,300);
	}
	
	printf("[LIBRARIAN][L] - Sucessfull loaded world %s\n",dest.world_name);
	fclose(list);
}

//utils
void putspaces(int count)
{
	for(int i=0;i<count;i++)
		printf("    ");
}

void librarian::info(struct block source,int level)
{
	putspaces(level);
	printf("id = %d\n",source.id);
	putspaces(level);
	printf("durability = %d\n",source.durability);
	putspaces(level);
	printf("storage size = %d\n",source.storage_size);
	putspaces(level);
	printf("data = %s\n",source.data.c_str());
	if(source.storage_size>0)
	{
		putspaces(level);
		printf("storage:[\n");
		for(int i=0;i<source.storage_size;i++)
		{
			this->info(source.storage[i],level+1);
		}
		putspaces(level);
		printf("]\n");
	}
}
