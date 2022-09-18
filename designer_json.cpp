#include "designer.h"

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
	j["menu"]["drag_zone"] = Menu->drag_zone;
	
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

bool designer::load_menu(std::string name,std::string folder) //if success, return true
{
	std::string path;
	path += folder;
	path += "\\";
	path += name;
	path += ".json";
	
	new_menu(name);
	menu* Menu = this->get_menu(name);
	
	std::ifstream f(path);
	
	if(!f.is_open()) return false;
	
	json j = json::parse(f);
	
	Menu->name = j["menu"]["name"].get<std::string>();
	
	Menu->menu_box = j["menu"]["menu_box"].get<box>();
	Menu->drag_zone = j["menu"]["drag_zone"].get<box>();
	
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
	
	return true;
}
