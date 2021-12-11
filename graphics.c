#include <assert.h>

#include "graphics.h"
#include "machine.h"

void g_clear_screen(SDL_Surface *s) {
    if (s) {
        memset(s->pixels, 0, s->h * s->pitch); 
    }
}

void g_clear_render(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderClear(r);
    SDL_RenderPresent(r);
    
}

