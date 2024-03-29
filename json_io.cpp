#include "json_io.h"

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
		
		{"fixed_pos",p.fixed_pos},
		{"delta_mode",p.delta_mode},
		{"auto_center",p.auto_center},
		
		{"rel_perc_h",p.rel_perc_h},
		{"rel_perc_w",p.rel_perc_w},
		{"d_x",p.d_x},
		{"d_y",p.d_y},
		
		{"fixed_size",p.fixed_size},
		{"delta_size",p.delta_size},
		{"rel_size_perc_h",p.rel_size_perc_h},
		{"rel_size_perc_w",p.rel_size_perc_w},
		{"d_h",p.d_h},
		{"d_w",p.d_w},
		
		{"shape",p.shape},
		{"center",p.center}
	};
}

void from_json(const json& j, position& p) 
{
	j.at("updated").get_to(p.updated);
		
	j.at("fixed_pos").get_to(p.fixed_pos);
	j.at("delta_mode").get_to(p.delta_mode);
	j.at("auto_center").get_to(p.auto_center);
		
	j.at("rel_perc_h").get_to(p.rel_perc_h);
	j.at("rel_perc_w").get_to(p.rel_perc_w);
	j.at("d_x").get_to(p.d_x);
	j.at("d_y").get_to(p.d_y);
		
	j.at("fixed_size").get_to(p.fixed_size);
	j.at("delta_size").get_to(p.delta_size);
	j.at("rel_size_perc_h").get_to(p.rel_size_perc_h);
	j.at("rel_size_perc_w").get_to(p.rel_size_perc_w);
	j.at("d_h").get_to(p.d_h);
	j.at("d_w").get_to(p.d_w);
		
	j.at("shape").get_to(p.shape);
	j.at("center").get_to(p.center);
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
		{"background_text",p.background_text},
		{"font_name",p.font_name},
		{"font_size",p.font_size},
		{"centered",p.centered},
		{"editable",p.editable},
		{"use_background_text",p.use_background_text}
	};
}

void from_json(const json& j, text& p) 
{
	j.at("text_box").get_to(p.text_box);
	j.at("text").get_to(p.text);
	j.at("background_text").get_to(p.background_text);
	j.at("font_name").get_to(p.font_name);
	j.at("font_size").get_to(p.font_size);
	j.at("centered").get_to(p.centered);
	j.at("editable").get_to(p.editable);
	j.at("use_background_text").get_to(p.use_background_text);
}

void to_json(json& j, const image& p) 
{
	j = json
	{
		{"shown",p.shown},
		{"pos",p.pos},
		{"filename",p.filename}
	};
}

void from_json(const json& j, image& p) 
{
	j.at("shown").get_to(p.shown);
	j.at("pos").get_to(p.pos);
	j.at("filename").get_to(p.filename);
}

void saveMenu(menu* t,std::string folder,std::string filename)
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
	
	j["menu"]["name"] = t->name;
	
	j["menu"]["menu_box"] = t->menu_box;
	
	j["menu"]["resizable"] = t->resizable;
	j["menu"]["movable"] = t->movable;
	j["menu"]["shown"] = t->shown;
	j["menu"]["copy_window_rect"] = t->copy_window_rect;
////////////////////////////////////////////////////////////////////////////////////////////////
	int bsize = t->buttons.size();
	j["menu"]["buttons_size"] = bsize;
	for(int i=0;i<bsize;i++)
	{
		j["menu"]["buttons"][i]["text_part"] = t->buttons[i].text_part;
		
		j["menu"]["buttons"][i]["locked"] = t->buttons[i].locked;
		j["menu"]["buttons"][i]["focused"] = t->buttons[i].focused;
		j["menu"]["buttons"][i]["pressed"] = t->buttons[i].pressed;
		
		j["menu"]["buttons"][i]["scriptname"] = t->buttons[i].scriptname;
	}
////////////////////////////////////////////////////////////////////////////////////////////////
	int tsize = t->texts.size();
	j["menu"]["texts_size"] = tsize;
	for(int i=0;i<tsize;i++)
	{
		j["menu"]["texts"][i] = t->texts[i];
	}
////////////////////////////////////////////////////////////////////////////////////////////////
	int isize = t->images.size();
	j["menu"]["images_size"] = isize;
	for(int i=0;i<isize;i++)
	{
		j["menu"]["images"][i] = t->images[i];
	}
	
	f << std::setw(4) << j << std::endl;
	
	f.close();
}

void loadMenu(window* w,menu* t,std::string folder,std::string filename)
{
	std::string path;
	path += folder;
	path += "\\";
	path += filename;
	path += ".json";
	
	std::ifstream f(path);
	
	if(!f.is_open()) return;
	
	json j = json::parse(f);
	
	t->name = j["menu"]["name"].get<std::string>();
	
	t->menu_box = j["menu"]["menu_box"].get<box>();
	
	t->resizable = j["menu"]["resizable"].get<bool>();
	t->movable = j["menu"]["movable"].get<bool>();
	t->shown = j["menu"]["shown"].get<bool>();
	t->copy_window_rect = j["menu"]["copy_window_rect"].get<bool>();
////////////////////////////////////////////////
	int buttons_size = j["menu"]["buttons_size"].get<int>();
	t->buttons.resize(buttons_size);
	for(int i=0;i<buttons_size;i++)
	{
		t->buttons[i].locked = j["menu"]["buttons"][i]["locked"].get<bool>();
		t->buttons[i].focused = j["menu"]["buttons"][i]["focused"].get<bool>();
		t->buttons[i].pressed = j["menu"]["buttons"][i]["pressed"].get<bool>();
		
		t->buttons[i].text_part = j["menu"]["buttons"][i]["text_part"].get<text>();
		
		t->buttons[i].scriptname = j["menu"]["buttons"][i]["scriptname"].get<std::string>();
		
		w->LW.loadfile_aschunk(t->buttons[i].scriptname);
	}
////////////////////////////////////////////////
	int texts_size = j["menu"]["texts_size"].get<int>();
	t->texts.resize(texts_size);
	for(int i=0;i<texts_size;i++)
	{
		t->texts[i] = j["menu"]["texts"][i].get<text>();
	}
////////////////////////////////////////////////
	int images_size = j["menu"]["images_size"].get<int>();
	t->images.resize(images_size);
	for(int i=0;i<images_size;i++)
	{
		t->images[i] = j["menu"]["images"][i].get<image>();
	}
	
	return;
}

menu* window::w_getMenu(std::string menu_name)
{
	if(menu_name.empty())
	{
		return NULL;
	}
	
	int size = menus.size();
	
	for(int i=0; i<size; i++)
	{
		if(menus[i].name == menu_name)
		{
			return &menus[i]; 
		}
	}
	
	return NULL;
}

bool window::w_loadMenu(std::string name)
{
	if(name.empty()) return false;
	if(w_getMenu(name)) return false;
	menu temp;
	loadMenu(this,&temp,"menusaves",name);
	this->menus.push_back(temp);
	return true;
}