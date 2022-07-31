#include "designer.h"

/*
	bool locked;
	bool focused;
	bool pressed;
*/

void button::update()
{
	int x,y;
	SDL_PumpEvents();
	Uint32 state = SDL_GetMouseState(&x,&y);
	
	if(x >= pos_x && x <= pos_x + size_x)
	{
		if(y >= pos_y && y <= pos_y + size_y)
		{
			focused = true;
			if( !locked && ((state & SDL_BUTTON_LMASK) != 0))
			{
				pressed = true;
				return;
			}
			pressed = false;
		}
	}
	focused = false;
}

void button::setPos(int &x,int &y)
{
	pos_x = x;
	pos_y = y;
}

void button::setSize(int &x,int &y)
{
	size_x = x;
	size_y = y;
}

void button::lock()
{
	locked = true;
}

void button::unlock()
{
	locked = false;
}

void designer::init(int start_size)
{
	menustack_size = start_size;
	menustack = new menu * [menustack_size];
	for(int i=0;i<menustack_size;i++)
	{
		menustack[i] = NULL;
	}
}

void designer::resize(int new_size)
{
	menu **tempstack = new menu *[menustack_size];
	for(int i=0;i<menustack_size;i++)
	{
		tempstack[i] = menustack[i];
	}
	delete[] menustack;
	
	menustack_size = new_size;
	
	menustack = new menu *[menustack_size];
	for(int i=0;i<menustack_size;i++)
	{
		menustack[i] = tempstack[i];
	}
}

designer::designer()
{
	init(4);
}

void designer::create_menu(char * name,SDL_Rect shape,SDL_Color border_color,SDL_Color menu_color,int border_thickness)
{
	int j = 0;
	while(menustack[j] != NULL)
	{
		if(j>=menustack_size)
		{
			resize(menustack_size+4);
		}
		j++;
	}
	
	menustack[j] = new menu;
	
	menustack[j]->name = name;
	menustack[j]->shape = shape;
	
	menustack[j]->border_color = border_color;
	menustack[j]->menu_color = menu_color;
	
	menustack[j]->border_thickness = border_thickness;
	
}

int designer::delete_menu(char * name)
{
	int j=0;
	while(strcmp(name,menustack[j]->name)!=0)
	{
		if(j>=menustack_size)
		{
			return -1; //oh, where is my menu?
		}
		j++;
	}
	delete menustack[j];
	return 0;
}

menu * designer::get_menu(char * name)
{
	int j=0;
	while(strcmp(name,menustack[j]->name)!=0)
	{
		if(j>=menustack_size)
		{
			return NULL; //oh, where is my menu?
		}
		j++;
	}
	return menustack[j];
}

void designer::menu_data_fprintf(menu *Menu,FILE * f)
{
	fprintf(f,"{\n%s\n",Menu->name);
	
	fprintf(f,"%d\n",Menu->border_thickness);
	
	fprintf(f,"%d\n",Menu->border_color.r);
	fprintf(f,"%d\n",Menu->border_color.g);
	fprintf(f,"%d\n",Menu->border_color.b);
	fprintf(f,"%d\n",Menu->border_color.a);
	
	fprintf(f,"%d\n",Menu->menu_color.r);
	fprintf(f,"%d\n",Menu->menu_color.g);
	fprintf(f,"%d\n",Menu->menu_color.b);
	fprintf(f,"%d\n",Menu->menu_color.a);
	
	fprintf(f,"%d\n",Menu->shape.x);
	fprintf(f,"%d\n",Menu->shape.y);
	fprintf(f,"%d\n",Menu->shape.w);
	fprintf(f,"%d\n",Menu->shape.h);
	
	fprintf(f,"%d\n",Menu->movable);
	fprintf(f,"%s\n",Menu->resizable);
	fprintf(f,"%s\n",Menu->shown);
}

void designer::save_menu(char * name,char * folder)
{
	char * path;
	sprintf(path,"%s\\%s.txt",folder,name);
	FILE * f = fopen(path,"wb");
	if(f!=NULL)
	{
		menu *Menu = get_menu(name);

		menu_data_fprintf(Menu,f);
		
		fclose(f);
	}
	delete [] path;
}