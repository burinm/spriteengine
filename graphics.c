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

#if 0
void screen_render_from_matrix(uint8_t * t) {
    uint8_t *p = (volatile uint8_t *)SRCEEN_RAM_mem;
    for (i=0; i<TOTAL_TEXTURE_BUFFER; i++) {

    }
}
#endif


int _screen_get_char_pos(int screencode) {
    return (screencode * 8);
}

int plot_pixel(uint8_t **p, int color) {
    assert(color < PALLET_ROM_SZ);

    uint8_t r,g,b;
    r = (PALLET_ROM_mem[color] & 0x00ff0000) >> 16;
    g = (PALLET_ROM_mem[color] & 0x0000ff00) >> 8; 
    b = (PALLET_ROM_mem[color] & 0x000000ff);

    *(*p)++ = r;
    *(*p)++ = g;
    *(*p)++ = b;
    *(*p)++ = 0; //alpha?

return 4;
}
