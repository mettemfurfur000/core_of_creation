#include "edit_features.h"

position d_make_position()
{
	position p;
	p.auto_center = true;
	p.delta_mode = false;
	p.delta_size = false;
	p.fixed_pos = true;
	p.fixed_size = true;
	
	p.d_h = 0;
	p.d_w = 0;
	p.d_x = 0;
	p.d_y = 0;
	
	p.rel_perc_h = 0.0;
	p.rel_perc_w = 0.0;
	
	p.rel_size_perc_h = 0.0;
	p.rel_size_perc_w = 0.0;
	
	p.shape.h = 20 + rand()%50;
	p.shape.w = 20 + rand()%50;
	p.shape.x = 10 + rand()%80;
	p.shape.y = 10 + rand()%80;
	p.updated = false;
	return p;
}

SDL_Color d_make_color()
{
	SDL_Color c;
	c.r = 55 + rand()%200;
	c.g = 55 + rand()%200;
	c.b = 55 + rand()%200;
	c.a = 55 + rand()%200;
	return c;
}

image make_default_image()
{
	image i;
	i.pos = d_make_position();
	i.shown = true;
	i.filename = "default.png";
	return i;
}

SDL_Rect& operator+=(SDL_Rect &destination, SDL_Rect source)
{
	destination.x += source.x;
	destination.y += source.y;
	return destination;
}

menu d_make_menu(char* menuname)
{
	menu m;
	m.copy_window_rect = false;
	m.menu_box.pos = d_make_position();
	m.menu_box.color = d_make_color();
	m.menu_box.border_color = d_make_color();
	m.menu_box.shown = true;
	m.menu_box.border_th = 2;
	m.movable = true;
	m.resizable = true;
	m.shown = true;
	m.name = menuname;
	return m;
}
