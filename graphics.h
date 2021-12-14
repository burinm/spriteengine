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

#define WINDOW_SCALE    (2)
//#define SURFACE_DEPTH   (4)

#define SCREEN_X        (RESOLUTION_X * WINDOW_SCALE)
#define SCREEN_Y        (RESOLUTION_Y * WINDOW_SCALE)



#include "SDL2/SDL.h"

void g_clear_screen(SDL_Surface *);
void g_clear_render(SDL_Renderer *);

void g_update_renderer(uint8_t *pixels);

int g_init();
uint8_t * g_texture_init();

void g_done();


#endif
