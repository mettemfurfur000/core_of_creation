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

menu * designer::get_menu(std::string name)
{
	int j=0;
	
	while(j<menustack_size&&menustack[j]->name != name) j++; //simple search
	
	if(j>=menustack_size) return NULL; //oh, where is my menu?
	
	return menustack[j]; //it is your menu, i swear!
}

void designer::save_menu(std::string name,std::string folder)
{
	std::string command;
	command += "mkdir ";
	command += folder;
	system(command.c_str());
	
	menu* Menu = get_menu(name);
	std::string path;
	path += folder;
	path += "\\";
	path += name;
	path += ".json";
	
	std::ofstream f(path);
	
	json j;
	
	j["menu"]["name"] = Menu->name;
	
	j["menu"]["shape"]["h"] = Menu->shape.h;
	j["menu"]["shape"]["w"] = Menu->shape.w;
	j["menu"]["shape"]["x"] = Menu->shape.x;
	j["menu"]["shape"]["y"] = Menu->shape.y;
	
	j["menu"]["border_color"]["r"] = Menu->border_color.r;
	j["menu"]["border_color"]["g"] = Menu->border_color.g;
	j["menu"]["border_color"]["b"] = Menu->border_color.b;
	j["menu"]["border_color"]["a"] = Menu->border_color.a;
	
	j["menu"]["menu_color"]["r"] = Menu->menu_color.r;
	j["menu"]["menu_color"]["g"] = Menu->menu_color.g;
	j["menu"]["menu_color"]["b"] = Menu->menu_color.b;
	j["menu"]["menu_color"]["a"] = Menu->menu_color.a;
	
	j["menu"]["border_thickness"] = Menu->border_thickness;
	
	j["menu"]["resizable"] = Menu->resizable;
	j["menu"]["movable"] = Menu->movable;
	j["menu"]["shown"] = Menu->shown;
	
	j["menu"]["buttons_size"] = Menu->buttons_size;
	for(int i=0;i<Menu->buttons_size;i++)
	{
		if(Menu->buttons[i])
		{
			j["menu"]["buttons"][i]["exist"] = true;
			j["menu"]["buttons"][i]["pos_x"] = Menu->buttons[i]->pos_x;
			j["menu"]["buttons"][i]["pos_y"] = Menu->buttons[i]->pos_y;
			j["menu"]["buttons"][i]["size_x"] = Menu->buttons[i]->size_x;
			j["menu"]["buttons"][i]["size_y"] = Menu->buttons[i]->size_y;
			
			j["menu"]["buttons"][i]["locked"] = Menu->buttons[i]->locked;
			j["menu"]["buttons"][i]["focused"] = Menu->buttons[i]->focused;
			j["menu"]["buttons"][i]["pressed"] = Menu->buttons[i]->pressed;
			
			j["menu"]["buttons"][i]["text"] = Menu->buttons[i]->text;
		}else{
			j["menu"]["buttons"][i]["exist"] = false;
		}
	}
	
	j["menu"]["texts_size"] = Menu->texts_size;
	for(int i=0;i<Menu->texts_size;i++)
	{
		if(Menu->texts[i])
		{
			j["menu"]["texts"][i]["exist"] = true;
			
			j["menu"]["texts"][i]["text"] = Menu->texts[i]->text;
			j["menu"]["texts"][i]["font_name"] = Menu->texts[i]->font_name;
			
			j["menu"]["texts"][i]["pos_x"] = Menu->texts[i]->pos_x;
			j["menu"]["texts"][i]["pos_y"] = Menu->texts[i]->pos_y;
		}else{
			j["menu"]["texts"][i]["exist"] = false;
		}
	}
	
	f << std::setw(4) << j << std::endl;
}

void designer::load_menu(std::string name,std::string folder)
{
	std::string path;
	path += folder;
	path += "\\";
	path += name;
	path += ".json";
	
	new_menu(name);
	menu* Menu = this->get_menu(name);
	
	std::ifstream f(path);
	json j = json::parse(f);
	
	Menu->name = j["menu"]["name"].get<std::string>();
	
	Menu->shape.h = j["menu"]["shape"]["h"].get<int>();
	Menu->shape.w = j["menu"]["shape"]["w"].get<int>();
	Menu->shape.x = j["menu"]["shape"]["x"].get<int>();
	Menu->shape.y = j["menu"]["shape"]["y"].get<int>();
	
	Menu->border_color.r = j["menu"]["border_color"]["r"].get<int>();
	Menu->border_color.g = j["menu"]["border_color"]["g"].get<int>();
	Menu->border_color.b = j["menu"]["border_color"]["b"].get<int>();
	Menu->border_color.a = j["menu"]["border_color"]["a"].get<int>();
	
	Menu->menu_color.r = j["menu"]["menu_color"]["r"].get<int>();
	Menu->menu_color.g = j["menu"]["menu_color"]["g"].get<int>();
	Menu->menu_color.b = j["menu"]["menu_color"]["b"].get<int>();
	Menu->menu_color.a = j["menu"]["menu_color"]["a"].get<int>();
	
	Menu->border_thickness = j["menu"]["border_thickness"].get<int>();
	
	Menu->resizable = j["menu"]["resizable"].get<bool>();
	Menu->movable = j["menu"]["movable"].get<bool>();
	Menu->shown = j["menu"]["shown"].get<bool>();
	
	Menu->buttons_size = j["menu"]["buttons_size"].get<int>();
	Menu->buttons = new button* [Menu->buttons_size];
	
	for(int i=0;i<Menu->buttons_size;i++)
	{
		if(j["menu"]["buttons"][i]["exist"].get<bool>())
		{
			Menu->buttons[i] = new button;
			Menu->buttons[i]->pos_x = j["menu"]["buttons"][i]["pos_x"].get<int>();
			Menu->buttons[i]->pos_y = j["menu"]["buttons"][i]["pos_y"].get<int>();
			Menu->buttons[i]->size_x = j["menu"]["buttons"][i]["size_x"].get<int>();
			Menu->buttons[i]->size_y = j["menu"]["buttons"][i]["size_y"].get<int>();
			
			Menu->buttons[i]->locked = j["menu"]["buttons"][i]["locked"].get<bool>();
			Menu->buttons[i]->focused = j["menu"]["buttons"][i]["focused"].get<bool>();
			Menu->buttons[i]->pressed = j["menu"]["buttons"][i]["pressed"].get<bool>();
			
			Menu->buttons[i]->text = j["menu"]["buttons"][i]["text"].get<std::string>();
		}else{
			Menu->buttons[i] = 0;
		}
	}
	
	Menu->texts_size = j["menu"]["texts_size"].get<int>();
	Menu->texts = new text* [Menu->texts_size];
	
	for(int i=0;i<Menu->texts_size;i++)
	{
		if(j["menu"]["texts"][i]["exist"].get<bool>())
		{
			Menu->texts[i] = new text;
			Menu->texts[i]->text = j["menu"]["texts"][i]["text"].get<std::string>();
			Menu->texts[i]->font_name = j["menu"]["texts"][i]["font_name"].get<std::string>();
			//
			Menu->texts[i]->font = this->normal_fonts.GetByName(Menu->texts[i]->font_name);
			//
			
			Menu->texts[i]->pos_x = j["menu"]["texts"][i]["pos_x"].get<int>();
			Menu->texts[i]->pos_y = j["menu"]["texts"][i]["pos_y"].get<int>();
		}else{
			Menu->texts[i] = 0;
		}
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

void designer::text_create(std::string menuname,int x,int y,std::string _text,std::string font)
{
	menu * Menu = this->get_menu(menuname.c_str());
	
	int i=0;
	while(Menu->texts && i < Menu->texts_size) i++; //simple search fot free place
	
	if(i >= Menu->texts_size) Menu->texts_resize(4); //expand if not enough
	
	Menu->texts[i] = new text;
	Menu->texts[i]->pos_x = x;
	Menu->texts[i]->pos_y = y;
	
	if(_text.empty()) 
		Menu->texts[i]->text = "hello, dev~";
	else 
		Menu->texts[i]->text = _text;
	
	if(font.empty())
	{
		Menu->texts[i]->font = this->normal_fonts.GetByName("default.ttf");
		Menu->texts[i]->font_name = "default.ttf";
	}else{
		Menu->texts[i]->font = this->normal_fonts.GetByName(font);
		Menu->texts[i]->font_name = font;
	}
}