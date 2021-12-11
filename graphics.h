#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define BITS_PER_RED      (8)
#define BITS_PER_GREEN    (8)
#define BITS_PER_BLUE     (8)
#define BITS_PER_ALPHA    (8)
#define BITS_PER_PIXEL  (BITS_PER_RED + BITS_PER_GREEN + BITS_PER_BLUE + BITS_PER_ALPHA)
#define BYTES_PER_PIXEL   (BITS_PER_PIXEL / 8)
#define TEXTURE_PITCH   (RESOLUTION_X * BYTES_PER_PIXEL)
#define TOTAL_TEXTURE_BUFFER (TEXTURE_PITCH * RESOLUTION_Y)


#include "SDL2/SDL.h"

void g_clear_screen(SDL_Surface *);
void g_clear_render(SDL_Renderer *);

void screen_render_from_matrix(uint8_t *p);

int plot_pixel(uint8_t **p, int color);
void plot_character_line(uint8_t **p, uint8_t screencode, uint8_t line);


#endif
