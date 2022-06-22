#include "librarian.h"

librarian::librarian()
{
	sizeofblock = sizeof(struct block);
}

void librarian::block_create(struct block &dest,int id,int durability,int storage_size,char *data)
{
	dest.id = id;
	dest.durability = durability;
	dest.storage_size = storage_size;
	
	int string_size;
	
	//data init
	if(data!=0)
	{
		string_size = strlen(data);
		dest.data = new char[string_size];
		strcpy(dest.data,data);
	}else{
		char temp[128] = "void";
		string_size = strlen(temp);
		dest.data = new char[string_size];
		strcpy(dest.data,temp);
	}
	
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
	int i=0;
	int data_size = strlen(source.data);
	while(source.data[i-1]!='\0')
	{
		putc(source.data[i],f);
		i++;
	}
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
//chunks
void librarian::chunk_save(char * filename,chunk &source)
{
	FILE * f;
	f = fopen(filename,"wb");
	if(f==NULL)
	{
		printf("[E] - Error occured while open(write) file\n");
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
		printf("[E] - Error occured while open(read) file\n");
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
// blocks
void librarian::save(char * filename,block source)
{
	FILE * f;
	
	f = fopen(filename,"wb");
	if(f==NULL)
	{
		printf("[E] - Error occured while open(write) file\n");
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
		printf("[E] - Error occured while open(read) file\n");
		return;
	}
	//
	block_load(f,out);
	//
	fclose(f);
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
	printf("data = %s\n",source.data);
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