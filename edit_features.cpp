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

box make_mox()
{
	box b;
	b.pos = d_make_position();
	b.color = d_make_color();
	b.border_color = d_make_color();
	b.shown = true;
	b.border_th = 2;
	return b;
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

text make_default_text()
{
	text t;
	t.centered = false;
	t.editable = false;
	t.font_name = "default.fon";
	t.font_size = 24;
	t.text = "Default Text";
	t.text_box = make_mox();
	return t;
}

button make_default_button()
{
	button b;
	b.text_part = make_default_text();
	b.locked = false;
	b.focused = false;
	b.click = false;
	b.pressed = false;
	b.scriptname = "void";
	return b;
}

SDL_Color input_color()
{
	SDL_Color c;
	printf("RGBA:\n");
	scanf("%d",&c.r);
	scanf("%d",&c.g);
	scanf("%d",&c.b);
	scanf("%d",&c.a);
	return c;
}

void edit_box(box* b)
{
	int choice;
	char temp[256];
	bool do_loop = true;
	printf("what to change?\n 0 - exit\n 1 - color\n 2 - border_color\n 3 - border_th\n");
	while(do_loop)
	{
		scanf("%d",&choice);
		switch(choice)
		{
			case 0:
				do_loop = false;
			break;
			case 1:
				printf("color = ");
				b->color = input_color();
			break;
			case 2:
				printf("centered = ");
				b->border_color = input_color();
			break;
			case 3:
				printf("border_th = ");
				scanf("%d",&b->border_th);
			break;
		}
	}
}

void edit_text(text* t)
{
	int choice;
	char temp[256];
	bool do_loop = true;
	printf("what to change?\n 0 - exit\n 1 - centered\n 2 - editable\n 3 - font_name\n 4 - font_size\n 5 - text\n 6 - text_box\n");
	while(do_loop)
	{
		scanf("%d",&choice);
		switch(choice)
		{
			case 0:
				do_loop = false;
			break;
			case 1:
				printf("centered = ");
				scanf("%d",&t->centered);
			break;
			case 2:
				printf("editable = ");
				scanf("%d",&t->editable);
			break;
			case 3:
				printf("font_name = ");
				scanf("%s",temp);
				t->font_name = temp;
			break;
			case 4:
				printf("font_size = ");
				scanf("%d",&t->font_size);
			break;
			case 5:
				printf("text = ");
				scanf("%s",&temp);
				t->text = temp;
			break;
			case 6:
				printf("text_box = ");
				edit_box(&t->text_box);
			break;
		}
	}
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
