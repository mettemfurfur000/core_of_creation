#include "event_handler.h"

void pop_back_utf8(std::string& utf8)
{
    if(utf8.empty())
        return;

    auto cp = utf8.data() + utf8.size();
    while(--cp >= utf8.data() && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {}
    if(cp >= utf8.data())
        utf8.resize(cp - utf8.data());
}

bool check_edit_mode_file()
{
	FILE* f = fopen("edit_mode.txt","rb");

	if(!f) return false;

	char buf[256];

	fgets(buf,256,f);

	fclose(f);

	return !strcmp("91082359823159825698723539875623895235",buf);
}

void event_handler::lua_init()
{
	static bool error = false;
	if(error) return;
	luabridge::LuaRef f = R.W.LW.getGlobal("init");
	
	try{
		f(&R.W);
	}
	
	catch(luabridge::LuaException e)
	{
		printf("init error, what(): %s\n",e.what());
		error = true;
	}
}

void event_handler::lua_tick()
{
	static bool error = false;
	if(error) return;
	luabridge::LuaRef f = R.W.LW.getGlobal("tick");
	
	try{
		f(&R.W);
	}
	
	catch(luabridge::LuaException e)
	{
		printf("tick error, what(): %s\n",e.what());
		error = true;
	}
}

event_handler::~event_handler()
{
	if(!this->edit_mode) return;
	int size = R.W.menus.size();
	for(int i=0; i<size; i++)
	{
		saveMenu(&R.W.menus[i],"menusaves",R.W.menus[i].name);
	}
}

void event_handler::catch_box()
{
	int tsize;
	int bsize;

	SDL_Point mouse;
	mouse.x = this->mouse_motion.x;
	mouse.y = this->mouse_motion.y;
	//clean
	if(edit_text_source) edit_text_source->selected = false;
	edit_pos = NULL;
	edit_text_source = NULL;
	edit_box_source = NULL;
	edit_image_source = NULL;
	
	if(SDL_IsTextInputActive) SDL_StopTextInput();
	
	tsize = this->R.W.menus.size();
	for(int i=tsize-1; i>=0; i--)
	{
		bsize = R.W.menus[i].buttons.size();
		for(int j=bsize-1; j>=0; j--)
		{
			if(SDL_PointInRect(&mouse,&R.W.menus[i].buttons[j].text_part.text_box.pos.real_rect))
			{
				this->edit_box_source = &R.W.menus[i].buttons[j].text_part.text_box;
				this->edit_pos = &edit_box_source->pos;

				if(R.W.menus[i].buttons[j].text_part.editable == false || !edit_mode) return;

				if(this->mouse_click.button == SDL_BUTTON_LEFT)
				{
					edit_text_source = &R.W.menus[i].buttons[j].text_part;
					
					edit_text_source->selected = true;

					SDL_StartTextInput();
				}

				return;
			}
		}
		bsize = R.W.menus[i].texts.size();
		for(int j=bsize-1; j>=0; j--)
		{
			if(SDL_PointInRect(&mouse,&R.W.menus[i].texts[j].text_box.pos.real_rect))
			{
				this->edit_box_source = &R.W.menus[i].texts[j].text_box;
				this->edit_pos = &edit_box_source->pos;

				if(R.W.menus[i].texts[j].editable == false || !edit_mode) return;

				if(this->mouse_click.button == SDL_BUTTON_LEFT)
				{
					edit_text_source = &R.W.menus[i].texts[j];
					
					edit_text_source->selected = true;

					SDL_StartTextInput();
				}

				return;
			}
		}
		bsize = R.W.menus[i].images.size();
		for(int j=bsize-1; j>=0; j--)
		{
			if(SDL_PointInRect(&mouse,&R.W.menus[i].images[j].pos.real_rect))
			{
				this->edit_image_source = &R.W.menus[i].images[j];
				this->edit_pos = &R.W.menus[i].images[j].pos;

				return;
			}
		}
		
		if(SDL_PointInRect(&mouse,&R.W.menus[i].menu_box.pos.real_rect) && R.W.menus[i].movable)
		{
			this->edit_box_source = &R.W.menus[i].menu_box;
			this->edit_pos = &edit_box_source->pos;
			
			return;
		}
	}
}

void event_handler::update_ui()
{
	static SDL_Point mouse_pos;
	static SDL_Point delta;

	static int tsize;
	static int bsize;
	//
	mouse_pos.x = mouse_motion.x;
	mouse_pos.y = mouse_motion.y;

	tsize = R.W.menus.size();
	for(int i=0; i<tsize; i++)
	{
		bsize = R.W.menus[i].buttons.size();
		for(int j=0; j<bsize; j++)
		{
			update_button(mouse_pos,&R.W.menus[i].buttons[j]);
		}
	}

	if(!edit_mode) return;

	delta.x = mouse_motion.x - last_motion.x;
	delta.y = mouse_motion.y - last_motion.y;
	
	if(mouse_click.button == SDL_BUTTON_LEFT) move_box_edit_mode(mouse_pos,delta);
	if(mouse_click.button == SDL_BUTTON_RIGHT) resize_box_edit_mode(mouse_pos,delta);

	last_motion = mouse_motion;
}

void event_handler::update_button(SDL_Point last_mouse_pos, button* b)
{
	if(b->locked) return;

	if(!SDL_PointInRect(&last_mouse_pos,&b->text_part.text_box.pos.real_rect))
	{
		b->focused = false;
		return;
	}
	b->focused = true;
	//for single click registrating, not the button holding
	if(!mouse_pressed)
	{
		b->pressed = false;
		return;
	}

	b->click = !b->pressed;

	b->pressed = true;

	if(!b->click)
	{
		return;
	}
	//
	int t = b->scriptname.size();

	if(t < 5) return;

	std::string sub = b->scriptname.substr(0,t-4);
	std::string funcname = sub + "_bclk";

	luabridge::LuaRef f = R.W.LW.getGlobal(funcname.c_str());
	
	try{
		*b = f(last_mouse_pos,b,&R.W);
	}
	
	catch(luabridge::LuaException e)
	{
		printf("button script error! what(): %s\n",e.what());
	}
}

void event_handler::move_box_edit_mode(SDL_Point mouse_pos, SDL_Point delta)
{
	if(!edit_pos) return;

	if(delta.x == 0 && delta.y == 0) return; //nothing to update

	if(edit_pos->fixed_pos)
	{
		edit_pos->shape.x += delta.x;
		edit_pos->shape.y += delta.y;
	}
	else
	{
		if(edit_pos->delta_mode)
		{
			edit_pos->d_x += delta.x;
			edit_pos->d_y += delta.y;
		}
		else
		{
			edit_pos->rel_perc_w += (float)delta.x / (edit_pos->relative_rect.w);
			edit_pos->rel_perc_h += (float)delta.y / (edit_pos->relative_rect.h);
		}
	}
}

void event_handler::resize_box_edit_mode(SDL_Point mouse_pos, SDL_Point delta)
{
	if(!edit_pos) return;

	if(delta.x == 0 && delta.y == 0) return; //nothing to update

	if(edit_pos->fixed_size)
	{
		edit_pos->shape.w += delta.x;
		edit_pos->shape.h += delta.y;
	}
	else
	{
		if(edit_pos->delta_size)
		{
			edit_pos->d_w += delta.x;
			edit_pos->d_h += delta.y;
		}
		else
		{
			edit_pos->rel_size_perc_w += (float)delta.x / (edit_pos->relative_rect.w);
			edit_pos->rel_size_perc_h += (float)delta.y / (edit_pos->relative_rect.h);
		}
	}
	edit_pos->updated = true;
}

void event_handler::handle_events()
{
	const Uint8 *state;
	while(SDL_PollEvent(&event)) //this loop handle all possible events!
	{
		switch(event.type)
		{
			case SDL_QUIT:
				doloop = false; //will stop main loop
				break;

			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						SDL_GetWindowSize(R.base_window,&R.W.windowrect.w,&R.W.windowrect.h);
						break;
				}
				break;

			case SDL_MOUSEMOTION:
				this->mouse_motion = event.motion;
				break;

			case SDL_MOUSEBUTTONDOWN:
				this->mouse_click = event.button;
				this->mouse_pressed = true;
				catch_box();
				break;

			case SDL_MOUSEBUTTONUP:
				this->mouse_click = event.button;
				this->mouse_pressed = false;
				edit_pos = 0;
				break;

			case SDL_KEYDOWN:
				this->key = event.key;
				edit_mode_for_elements();
				break;

			case SDL_KEYUP:
				this->key = event.key;
				break;

			case SDL_TEXTINPUT:
				if(!edit_text_source) break;

				edit_text_source->text += event.text.text;

				edit_text_source->updated = true;
				break;
		}
	}
}

bool event_handler::edit_mode_for_elements()
{
	auto state = SDL_GetKeyboardState(NULL);
	
	if(edit_text_source) //manual text editing
	{
		if( state[SDL_SCANCODE_BACKSPACE] ) //erasing text
		{
			if(edit_text_source->text.empty()) return true;
			pop_back_utf8(edit_text_source->text);
			edit_text_source->updated = true;
		}
		
		if( state[SDL_SCANCODE_C] && SDL_GetModState() & KMOD_CTRL ) //ctrl+c
		{
			SDL_SetClipboardText( edit_text_source->text.c_str() );
		}
		
		if( state[SDL_SCANCODE_V] && SDL_GetModState() & KMOD_CTRL ) //ctrl+v
		{
			edit_text_source->text = SDL_GetClipboardText();
			edit_text_source->updated = true;
		}
	}
	
	if(edit_mode)//special edit mode commands
	{
		button b;
		text t;
		image i;
		int input;
		char menuname[256];
		char temp[256];
		if(!(SDL_GetModState() & KMOD_ALT)) return true; //works only with alt button
		
		if( state[SDL_SCANCODE_A] )//adding new element
		{
			printf("name of menu to create new element : ");
			
			scanf("%s",menuname);
			
			menu* m = R.W.w_getMenu(menuname);
			
			printf("1 - button, 2 - text, 3 - image\n");
			
			scanf("%d",&input);
			switch(input)
			{
				case 1:
					b = make_default_button();
					printf("scriptname for button:");
					scanf("%s",temp);
					b.scriptname = temp;
					edit_text(&b.text_part);
					m->buttons.push_back(b);
					break;
				case 2:
					t = make_default_text();
					edit_text(&t);
					m->texts.push_back(t);
					break;
				case 3:
					i = make_default_image();
					
					printf("filename for image:");
					
					scanf("%s",temp);
					
					m->images.push_back(i);
					
					break;
			}
		}
		
		if( state[SDL_SCANCODE_M] )//creating new menu
		{
			printf("creating new menu\nname:");
			scanf("%s",menuname);
			this->R.W.menus.push_back(d_make_menu(menuname));
		}
		
		if( state[SDL_SCANCODE_C] && edit_box_source)//editing colors of box
		{
			SDL_Color c;
			printf("changing colors in box\n");
			printf("1 - main, 2 - border\n");
			scanf("%d",&input);
			switch(input)
			{
				case 1:
					printf("main color:\n");
					scanf("%x %x %x %x", &c.r,&c.g,&c.b,&c.a);
					this->edit_box_source->color = c;
					break;
				case 2:
					printf("border color:\n");
					scanf("%x %x %x %x", &c.r,&c.g,&c.b,&c.a);
					this->edit_box_source->border_color = c;
					break;
			}
		}
	}
	return false;
}

void event_handler::init()
{
	edit_mode = check_edit_mode_file();
	register_things(this->R.W.LW.L);
	menu main_menu;
	loadMenu(&R.W,&main_menu,"menusaves","START_MENU");
	R.W.menus.push_back(main_menu);
	R.W.LW.loadfile_aschunk("init.lua");
	R.W.LW.loadfile_aschunk("tick.lua");
	
	lua_init();
}

void event_handler::loop()
{
	while(doloop)
	{
		handle_events();

		update_ui();
		
		lua_tick();

		R.clear();

		R.render(&R.W);

		R.screen_update();

		SDL_Delay(16);
	}
}