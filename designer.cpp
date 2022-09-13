#include "designer.h"
/*
	bool locked;
	bool focused;
	bool pressed;
*/

void designer::button_update(button &b,SDL_Rect real_pos)
{
	int x,y;
	SDL_PumpEvents();
	Uint32 state = SDL_GetMouseState(&x,&y);
	
	if(x >= real_pos.x && x <= (real_pos.x + real_pos.w))
	{
		if(y >= real_pos.y && y <= (real_pos.y + real_pos.h))
		{
			b.focused = true;
			if( !b.locked && ((state & SDL_BUTTON_LMASK) != 0))
			{
				b.pressed = true;
				return;
			}
			b.pressed = false;
			return;
		}
	}
	b.focused = false;
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

//nlohman!

//SDL_Rect

void to_json(json& j, const SDL_Rect& p) 
{
	j = json
	{
		{"x",p.x},
		{"y",p.y},
		{"h",p.h},
		{"w",p.w}
	};
}

void from_json(const json& j, SDL_Rect& p) 
{
	j.at("x").get_to(p.x);
	j.at("y").get_to(p.y);
	j.at("h").get_to(p.h);
	j.at("w").get_to(p.w);
}

//SDL_Color

void to_json(json& j, const SDL_Color& p) 
{
	j = json
	{
		{"r",p.r},
		{"g",p.g},
		{"b",p.b},
		{"a",p.a}
	};
}

void from_json(const json& j, SDL_Color& p) 
{
	j.at("r").get_to(p.r);
	j.at("g").get_to(p.g);
	j.at("b").get_to(p.b);
	j.at("a").get_to(p.a);
}

//box

void to_json(json& j, const box& p) 
{
	j = json
	{
		{"shape",p.shape},
		{"border_color",p.border_color},
		{"main_color",p.color},
		{"border_th",p.border_th}
	};
}

void from_json(const json& j, box& p) 
{
	j.at("shape").get_to(p.shape);
	j.at("main_color").get_to(p.color);
	j.at("border_color").get_to(p.border_color);
	j.at("border_th").get_to(p.border_th);
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
	
	j["menu"]["menu_box"] = Menu->menu_box;
	
	j["menu"]["resizable"] = Menu->resizable;
	j["menu"]["movable"] = Menu->movable;
	j["menu"]["shown"] = Menu->shown;
	
	j["menu"]["buttons_size"] = Menu->buttons_size;
	for(int i=0;i<Menu->buttons_size;i++)
	{
		if(Menu->buttons[i])
		{
			j["menu"]["buttons"][i]["exist"] = true;
			j["menu"]["buttons"][i]["button_box"] = Menu->buttons[i]->button_box;
			
			j["menu"]["buttons"][i]["locked"] = Menu->buttons[i]->locked;
			j["menu"]["buttons"][i]["focused"] = Menu->buttons[i]->focused;
			j["menu"]["buttons"][i]["pressed"] = Menu->buttons[i]->pressed;
			
			j["menu"]["buttons"][i]["text"] = Menu->buttons[i]->text;
			j["menu"]["buttons"][i]["font_name"] = Menu->buttons[i]->font_name;
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
			
			j["menu"]["texts"][i]["text_box"] = Menu->texts[i]->text_box;
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
	
	Menu->menu_box = j["menu"]["menu_box"].get<box>();
	
	Menu->resizable = j["menu"]["resizable"].get<bool>();
	Menu->movable = j["menu"]["movable"].get<bool>();
	Menu->shown = j["menu"]["shown"].get<bool>();
	
	Menu->buttons_size = j["menu"]["buttons_size"].get<int>();
	Menu->buttons = new button* [Menu->buttons_size];
	
	for(int i=0;i<Menu->buttons_size;i++)
	{
		j["menu"]["buttons"][i]["exist"].get<bool>();
		if(j["menu"]["buttons"][i]["exist"])
		{
			Menu->buttons[i] = new button;
			
			Menu->buttons[i]->button_box = j["menu"]["buttons"][i]["button_box"].get<box>();
			
			Menu->buttons[i]->locked = j["menu"]["buttons"][i]["locked"].get<bool>();
			Menu->buttons[i]->focused = j["menu"]["buttons"][i]["focused"].get<bool>();
			Menu->buttons[i]->pressed = j["menu"]["buttons"][i]["pressed"].get<bool>();
			
			Menu->buttons[i]->text = j["menu"]["buttons"][i]["text"].get<std::string>();
			Menu->buttons[i]->font_name = j["menu"]["buttons"][i]["font_name"].get<std::string>();
			Menu->buttons[i]->font = this->normal_fonts.GetByName(Menu->buttons[i]->font_name);
		}else{
			Menu->buttons[i] = 0;
		}
	}
	
	Menu->texts_size = j["menu"]["texts_size"].get<int>();
	Menu->texts = new text* [Menu->texts_size];
	
	for(int i=0;i<Menu->texts_size;i++)
	{
		j["menu"]["texts"][i]["exist"].get<bool>();
		if(j["menu"]["texts"][i]["exist"])
		{
			Menu->texts[i] = new text;
			Menu->texts[i]->text_box = j["menu"]["texts"][i]["text_box"].get<box>();
			Menu->texts[i]->text = j["menu"]["texts"][i]["text"].get<std::string>();
			Menu->texts[i]->font_name = j["menu"]["texts"][i]["font_name"].get<std::string>();
			//
			Menu->texts[i]->font = this->normal_fonts.GetByName(Menu->texts[i]->font_name);
			//
			
		}else{
			Menu->texts[i] = 0;
		}
	}
	
}

void menu::update()
{
	if(!movable) return;
	
	int x,y;
	SDL_PumpEvents();
	Uint32 state = SDL_GetMouseState(&x,&y);
	
	if(moving)
	{
		menu_box.shape.x += x - mouse_press_pos.x;
		menu_box.shape.y += y - mouse_press_pos.y;
	}else{
		if(menu_box.shape.x < 0) menu_box.shape.x = 0;
		if(menu_box.shape.y < 0) menu_box.shape.y = 0;
	}
	
	if(x >= menu_box.shape.x && x <= (menu_box.shape.x + menu_box.shape.w))
	{
		if(y >= menu_box.shape.y && y <= (menu_box.shape.y + 20))
		{
			if( ((state & SDL_BUTTON_LMASK) != 0))
			{
				moving = true;
				mouse_press_pos.x = x;
				mouse_press_pos.y = y;
				return;
			}
			moving = false;
			return;
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

void designer::text_create(std::string menuname,int x,int y,int h,int w,std::string _text,std::string font)
{
	menu* Menu = this->get_menu(menuname.c_str());
	
	int i=0;
	while(Menu->texts && i < Menu->texts_size) i++; //simple search fot free place
	
	if(i >= Menu->texts_size) Menu->texts_resize(4); //expand if not enough
	
	Menu->texts[i] = new text;
	Menu->texts[i]->text_box.shape.x = x;
	Menu->texts[i]->text_box.shape.y = y;
	Menu->texts[i]->text_box.shape.h = h;
	Menu->texts[i]->text_box.shape.w = w;
	
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

void designer::button_create(std::string menuname,std::string str,std::string font_name,SDL_Rect shape,SDL_Color color)
{
	menu* Menu = get_menu(menuname.c_str());
	
	int i=0;
	while(Menu->buttons && i < Menu->buttons_size) i++; //simple search fot free place
	
	if(i >= Menu->buttons_size) Menu->buttons_resize(4); //expand if not enough
	
	Menu->buttons[i] = new button;
	Menu->buttons[i]->button_box.shape = shape;
	Menu->buttons[i]->button_box.color = color;
	
	if(str.empty())
		Menu->buttons[i]->text = " ";
	else 
		Menu->buttons[i]->text = str;
	
	if(font_name.empty())
	{
		Menu->buttons[i]->font = this->normal_fonts.GetByName("default.ttf");
		Menu->buttons[i]->font_name = "default.ttf";
	}else{
		Menu->buttons[i]->font = this->normal_fonts.GetByName(font_name);
		Menu->buttons[i]->font_name = font_name;
	}
}