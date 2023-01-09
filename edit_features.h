#include "ui_struct.h"

position d_make_position();

SDL_Color d_make_color();

image make_default_image();

text make_default_text();

button make_default_button();

SDL_Color input_color();

void edit_box(box* b);

void edit_text(text* t);

SDL_Rect& operator+=(SDL_Rect &destination, SDL_Rect source);

menu d_make_menu(char* menuname);