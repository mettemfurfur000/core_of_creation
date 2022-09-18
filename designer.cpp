#include "designer.h"

/*
	bool locked;
	bool focused;
	bool pressed;
*/

inline bool SDL_PointInRectRel(const SDL_Point *p,SDL_Rect r,const SDL_Rect *relative)
{
	r.x += relative->x;
	r.y += relative->y;
	return SDL_PointInRect(p,&r);
}

void designer::button_update(button &b,SDL_Rect &real_pos)
{
	SDL_Point cursor;
	
	SDL_PumpEvents();
	
	Uint32 state = SDL_GetMouseState(&cursor.x,&cursor.y);
	
	b.focused = SDL_PointInRect(&cursor,&real_pos);
	if( b.focused && !b.locked && ((state & SDL_BUTTON_LMASK) != 0))
	{
		b.pressed = true;
			return;
	}
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

void designer::edit_move_object_in_menu(box menubox,box &objbox)
{
	SDL_Point cursor;
	
	SDL_PumpEvents();
	Uint32 state = SDL_GetMouseState(&cursor.x,&cursor.y);
	
	if(objbox.ed_pos.moving)
	{
		objbox.shape.x += cursor.x - objbox.ed_pos.old_cursor.x;
		objbox.shape.y += cursor.y - objbox.ed_pos.old_cursor.y;
	}
	
	if(SDL_PointInRectRel(&cursor,objbox.shape,&menubox.shape))
	{
		if(state & SDL_BUTTON_LMASK)
		{
			objbox.ed_pos.moving = true;
			objbox.ed_pos.old_cursor = cursor;
			return;
		}
		objbox.ed_pos.moving = false;
		return;
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

void designer::new_menu(std::string name)
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

void designer::create_menu(std::string name,SDL_Rect shape,SDL_Color border_color,SDL_Color menu_color,int border_thickness)
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
	
	menustack[j]->menu_box.shape = shape;
	
	menustack[j]->menu_box.border_color = border_color;
	menustack[j]->menu_box.color = menu_color;
	
	menustack[j]->menu_box.border_th = border_thickness;
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

menu * designer::get_menu(std::string name)
{
	int j=0;
	
	while(j<menustack_size&&menustack[j]->name != name) j++; //simple search
	
	if(j>=menustack_size) return NULL; //oh, where is my menu?
	
	return menustack[j]; //it is your menu, i swear!
}

void menu::update()
{
	if(!movable) return;
	
	static SDL_Point cursor;
	static SDL_Point old_cursor;
	
	SDL_PumpEvents();
	Uint32 state = SDL_GetMouseState(&cursor.x,&cursor.y);
	
	if(moving)
	{
		menu_box.shape.x += cursor.x - old_cursor.x;
		menu_box.shape.y += cursor.y - old_cursor.y;
	}
	
	if(SDL_PointInRectRel(&cursor,drag_zone.shape,&menu_box.shape))
	{
		if(state & SDL_BUTTON_LMASK)
		{
			moving = true;
			old_cursor = cursor;
			return;
		}
		moving = false;
		return;
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