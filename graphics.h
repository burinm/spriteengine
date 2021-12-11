#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "SDL2/SDL.h"

void g_clear_screen(SDL_Surface *);
void g_clear_render(SDL_Renderer *);


int plot_pixel(uint8_t **p, int color);


#endif
