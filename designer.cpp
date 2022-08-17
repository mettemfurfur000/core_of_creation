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

void designer::init(int start_size)
{
	menustack_size = start_size;
	menustack = new menu * [menustack_size];
	for(int i=0;i<menustack_size;i++)
	{
		menustack[i] = NULL;
	}
}

void designer::resize(int increment)
{
	t_resize(menustack,menustack_size,increment);
}

designer::designer()
{
	init(4);
}

void designer::new_menu(char * name)
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
	
	memset(menustack[j],0,sizeof(menu));
	
	menustack[j]->name = name;
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
	while(menustack[j]->name != name && j < menustack_size)
	{
		if(j>=menustack_size)
		{
			return -1; //oh, where is my menu?
		}
		j++;
	}
	for(int i=0;i<menustack[j]->texts_size;i++)
	{
		delete menustack[j]->texts[i];
	}
	for(int i=0;i<menustack[j]->buttons_size;i++)
	{
		delete menustack[j]->buttons[i];
	}
	delete menustack[j];
	return 0;
}

menu * designer::get_menu(char * name)
{
	int j=0;
	
	while(j<menustack_size&&menustack[j]->name != name) j++; //simple search
	
	if(j>=menustack_size) return NULL; //oh, where is my menu?
	
	return menustack[j]; //it is your menu, i swear!
}

void designer::save_menu(char * name,char * folder)
{
	char path[256];
	char cmd[256];
    sprintf(cmd,"mkdir %s",folder);
	system(cmd);
    sprintf(path,"%s\\%s.txt",folder,name);
    
    menu Menu = *get_menu(name);
}

void designer::load_menu(char * name,char * folder)
{
	char path[256];
	char cmd[256];
	char* tmp;
	char v[256];
	
	new_menu(name);
	
	menu* Menu = this->get_menu(name);
	
	sprintf(path,"%s\\%s.txt",folder,name);
}

void menu::buttons_resize(int increment)
{
	t_resize(this->buttons,this->buttons_size,increment);
}

void menu::texts_resize(int increment)
{
	t_resize(this->texts,this->texts_size,increment);
}

void menu::texts_init(int size)
{
	t_init(this->texts,size);
	this->texts_size = size;
}
void menu::buttons_init(int size)
{
	t_init(this->buttons,size);
	this->buttons_size = size;
}

void designer::text_create(char* menuname,int x,int y,char *_text,char *font)
{
	menu * Menu = this->get_menu(menuname);
	int i=0;
	while(Menu->texts!=NULL&&i<Menu->texts_size) i++;
	
	if(!(i<Menu->texts_size)) Menu->texts_resize(4);
	
	Menu->texts[i] = new text;
	Menu->texts[i]->pos_x = x;
	Menu->texts[i]->pos_y = y;
	
	if(_text)
	{
		Menu->texts[i]->text = _text;
	}
	else
		Menu->texts[i]->text = "hello, dev~";
	
	if(font)
	{
		Menu->texts[i]->font = this->normal_fonts.GetByName(font);
		Menu->texts[i]->font_name = font;
	}
	else
	{
		Menu->texts[i]->font = this->normal_fonts.GetByName("default.ttf");
		Menu->texts[i]->font_name = "default.ttf";
	}
}