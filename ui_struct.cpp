#include "ui_struct.h"

SDL_Rect& operator+=(SDL_Rect &destination, SDL_Rect source)
{
	destination.x += source.x;
	destination.y += source.y;
	return destination;
}

using json = nlohmann::json;

void to_json(json& j, const SDL_Point& p) 
{
	j = json
	{
		{"x",p.x},
		{"y",p.y}
	};
}

void from_json(const json& j, SDL_Point& p) 
{
	j.at("x").get_to(p.x);
	j.at("y").get_to(p.y);
}

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

void to_json(json& j, const position& p) 
{
	j = json
	{
		{"updated",p.updated},
		
		{"shape",p.shape},
		{"fixed_pos",p.fixed_pos},
		{"delta_mode",p.delta_mode},
		{"auto_center",p.auto_center},
		{"center",p.center},
		{"rel_perc_h",p.rel_perc_h},
		{"rel_perc_w",p.rel_perc_w},
		{"d_x",p.d_x},
		{"d_y",p.d_y}
	};
}

void from_json(const json& j, position& p) 
{
	j.at("updated").get_to(p.updated);
	
	j.at("shape").get_to(p.shape);
	j.at("fixed_pos").get_to(p.fixed_pos);
	j.at("delta_mode").get_to(p.delta_mode);
	j.at("auto_center").get_to(p.auto_center);
	j.at("center").get_to(p.center);
	j.at("rel_perc_h").get_to(p.rel_perc_h);
	j.at("rel_perc_w").get_to(p.rel_perc_w);
	j.at("d_x").get_to(p.d_x);
	j.at("d_y").get_to(p.d_y);
}

void to_json(json& j, const box& p) 
{
	j = json
	{
		{"pos",p.pos},
		
		{"shown",p.shown},
		{"main_color",p.color},
		{"border_color",p.border_color},
		{"border_th",p.border_th},
		
		{"moving",p.moving}
	};
}

void from_json(const json& j, box& p) 
{
	j.at("pos").get_to(p.pos);
	
	j.at("shown").get_to(p.shown);
	j.at("main_color").get_to(p.color);
	j.at("border_color").get_to(p.border_color);
	j.at("border_th").get_to(p.border_th);
	
	j.at("moving").get_to(p.moving);
}

void to_json(json& j, const text& p) 
{
	j = json
	{
		{"text_box",p.text_box},
		{"text",p.text},
		{"font_name",p.font_name},
		{"font_size",p.font_size},
		{"centered",p.centered}
	};
}

void from_json(const json& j, text& p) 
{
	j.at("text_box").get_to(p.text_box);
	j.at("text").get_to(p.text);
	j.at("font_name").get_to(p.font_name);
	j.at("font_size").get_to(p.font_size);
	j.at("centered").get_to(p.centered);
}

void menu::save(std::string folder,std::string filename)
{
	std::string command;
	command += "mkdir ";
	command += folder;
	system(command.c_str());
	
	std::string path;
	path += folder;
	path += "\\";
	path += filename;
	path += ".json";
	
	std::ofstream f;
	f.open(path);
	
	json j;
	
	j["menu"]["name"] = name;
	
	j["menu"]["menu_box"] = menu_box;
	j["menu"]["drag_zone"] = drag_zone;
	
	j["menu"]["resizable"] = resizable;
	j["menu"]["movable"] = movable;
	j["menu"]["shown"] = shown;
	j["menu"]["copy_window_rect"] = copy_window_rect;
	
	j["menu"]["buttons_size"] = buttons.size();
	
	for(int i=0;i<buttons.size();i++)
	{
		j["menu"]["buttons"][i]["text_part"] = buttons[i].text_part;
		
		j["menu"]["buttons"][i]["locked"] = buttons[i].locked;
		j["menu"]["buttons"][i]["focused"] = buttons[i].focused;
		j["menu"]["buttons"][i]["pressed"] = buttons[i].pressed;
	}
	
	j["menu"]["texts_size"] = texts.size();
	
	for(int i=0;i<texts.size();i++)
	{
		j["menu"]["texts"][i] = texts[i];
	}
	
	f << std::setw(4) << j << std::endl;
	
	f.close();
}

bool menu::load(std::string folder,std::string filename) //if success, return true
{
	std::string path;
	path += folder;
	path += "\\";
	path += filename;
	path += ".json";
	
	std::ifstream f(path);
	
	if(!f.is_open()) return false;
	
	json j = json::parse(f);
	
	name = j["menu"]["name"].get<std::string>();
	
	menu_box = j["menu"]["menu_box"].get<box>();
	drag_zone = j["menu"]["drag_zone"].get<box>();
	
	resizable = j["menu"]["resizable"].get<bool>();
	movable = j["menu"]["movable"].get<bool>();
	shown = j["menu"]["shown"].get<bool>();
	copy_window_rect = j["menu"]["copy_window_rect"].get<bool>();
	
	int buttons_size = j["menu"]["buttons_size"].get<int>();
	
	buttons.resize(buttons_size);
	for(int i=0;i<buttons_size;i++)
	{
		buttons[i].locked = j["menu"]["buttons"][i]["locked"].get<bool>();
		buttons[i].focused = j["menu"]["buttons"][i]["focused"].get<bool>();
		buttons[i].pressed = j["menu"]["buttons"][i]["pressed"].get<bool>();
		
		buttons[i].text_part = j["menu"]["buttons"][i]["text_part"].get<text>();
	}
	
	int texts_size = j["menu"]["texts_size"].get<int>();
	
	texts.resize(texts_size);
	
	for(int i=0;i<texts_size;i++)
	{
		texts[i] = j["menu"]["texts"][i].get<text>();
	}
	
	return true;
}

void window::loadMainMenu()
{
	menu main_menu;
	main_menu.load("menusaves","MAIN_MENU");
	this->menus.push_back(main_menu);
}

menu* window::getMenuByName(std::string _name)
{
	int size = this->menus.size();
	for(int i = 0;i<size;i++)
	{
		if(menus[i].name == _name) return &menus[i];
	}
	return 0;
}

