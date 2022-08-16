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
	
	menustack[j]->name = new char[strlen(name)];
	strcpy(menustack[j]->name,name);
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
	
	menustack[j]->namesize = strlen(name);
	menustack[j]->name = new char[menustack[j]->namesize];
	strcpy(menustack[j]->name,name);
	
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
	for(int i=0;i<menustack[j]->texts_size;i++)
	{
		if(menustack[j]->texts[i]->string) delete[] menustack[j]->texts[i]->string;
		delete menustack[j]->texts[i];
	}
	for(int i=0;i<menustack[j]->buttons_size;i++)
	{
		if(menustack[j]->buttons[i]->string) delete[] menustack[j]->buttons[i]->string;
		delete menustack[j]->buttons[i];
	}
	delete menustack[j];
	return 0;
}

menu * designer::get_menu(char * name)
{
	int j=0;
	
	while(menustack[j]->name&&j<menustack_size)
	{
		if(!strcmp(name,menustack[j]->name))
			break;
		j++;
	}
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
    
    FILE * f = fopen(path,"wb");
    if(f)
    {
    	object_save(f,&Menu);
    	object_save(f,Menu.name,strlen(Menu.name)+1);
    	
    	object_save(f,Menu.buttons);
    	for(int i=0;i<Menu.buttons_size;i++)
    	{
    		object_save(f,Menu.buttons[i]);
    		if(Menu.buttons[i]->string)
			{
				Menu.buttons[i]->strsize = strlen(Menu.buttons[i]->string)+1;
				object_save(f,Menu.buttons[i]->string,Menu.buttons[i]->strsize);
			}else{
				object_save(f,Menu.buttons[i]->string,1);
			}
		}
		object_save(f,Menu.texts);
		for(int i=0;i<Menu.texts_size;i++)
    	{
    		object_save(f,Menu.texts[i]);
    		if(Menu.texts[i])
    		{
    			if(Menu.texts[i]->string)
				{
    				Menu.texts[i]->strsize = strlen(Menu.texts[i]->string)+1;
    				object_save(f,Menu.texts[i]->string,Menu.texts[i]->strsize);
				}else{
					object_save(f,Menu.texts[i]->string,1);
				}
				//!! convert pointer on font into font_name string !!!
				char *tmp = this->normal_fonts.GetByPointer(Menu.texts[i]->font);
				
				Menu.texts[i]->fontnamesize = strlen(tmp);
				
    			object_save(f,tmp);
    			//!! !!
			}
		}
    	fclose(f);
	}
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
    
    FILE * f = fopen(path,"rb");
    if(f)
    {
    	object_load(f,Menu);
    	
    	object_load(f,Menu->name,Menu->namesize+1,true);
    	
    	object_load(f,Menu->buttons,Menu->buttons_size,true);
    	for(int i=0;i<Menu->buttons_size;i++)
    	{
    		object_load(f,Menu->buttons[i],1,true);
    		object_load(f,Menu->buttons[i]->string,Menu->buttons[i]->strsize+1,true);
		}
		
		object_load(f,Menu->texts,Menu->texts_size,true);
		for(int i=0;i<Menu->texts_size;i++)
    	{
    		object_load(f,Menu->texts[i],1,true);
    		object_load(f,Menu->texts[i]->string,Menu->texts[i]->strsize+1,true);
    		
    		//!! convert font_name string into pointer on font(if loaded) !!!
    		
    		object_load(f,tmp,Menu->texts[i]->fontnamesize+1,true);
    		
    		Menu->texts[i]->font = this->normal_fonts.GetByName(tmp);
    		//!! !!
    		delete[] tmp;
		}
    	fclose(f);
	}
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
		Menu->texts[i]->string = _text;
		Menu->texts[i]->strsize = strlen(_text);
	}
	else
		Menu->texts[i]->string = "hello, dev~";
	
	if(font)
		Menu->texts[i]->font = this->normal_fonts.GetByName(font);
	else
		Menu->texts[i]->font = this->normal_fonts.GetByName("default.ttf");
}