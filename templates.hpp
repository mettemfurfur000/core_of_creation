#include <cstdio>

#pragma once

template<typename T>
void object_save(FILE * f,T* object,int size=1)
{
	char *byte_pointer = (char *)object;
	size *= sizeof(T); 
	if(object)
	{
		for(int i=0;i<size;i++)
		{
			putc(byte_pointer[i],f);
		}
		return;
	}
	for(int i=0;i<size;i++)
	{
		putc(0,f);
	}
}

template<typename T>
void object_load(FILE * f,T* &object,int size=1,bool dynamic=false)
{
	if(dynamic) object = new T [size];
	
	char *byte_pointer = (char *)object;
	size *= sizeof(T);
	for(int i=0;i<size;i++)
	{
		byte_pointer[i] = getc(f);
	}
}

template<typename T>
void t_resize(T** &arr,int &size,int increment)
{
	int new_size = size + increment;
	T** res = new T* [new_size];
	
	for(int i=0;i<new_size;i++)
	{
		res[i] = 0;
	}
	
	for(int i=0;i<size;i++)
	{
		res[i] = arr[i];
	}
	
	size += increment;
	delete[] arr;
	arr = res;
}

template<typename T>
void t_init(T** &arr,int size)
{
	arr = new T *[size];
	
	for(int i=0;i<size;i++)
	{
		arr[i] = 0;
	}
}
