#include <assert.h>

#include "graphics.h"
#include "machine.h"

void screen_render_from_matrix(uint8_t *p) {
//assert(TOTAL_TEXTURE_BUFFER == SCREEN_MEM_SZ * (8 * 8) * BYTES_PER_PIXEL);

    //Do this like a raster video system for now
    for (int j=0; j<SCREEN_MATRIX_Y; j++) { // all rows
        for (int line=0; line<8; line++) { // one row of characters
            for (uint i=0; i<SCREEN_MATRIX_X; i++) { // one raster line
                plot_character_line(&p, SRCEEN_RAM_mem[i + j*SCREEN_MATRIX_X], line);
            }
        }


    }
}

int _screen_get_screencode_pos(int screencode, uint8_t line) {
assert(line < 8);
assert(screencode < (CHAR_ROM_SZ/8));

    return (screencode * 8 + line);
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

void plot_character_line(uint8_t **p, uint8_t screencode, uint8_t line) {

    for (int i=0; i<8; i++) {
        if (CHARACTER_ROM_mem[_screen_get_screencode_pos(screencode, line)] & (1<<(7-i))) {
            plot_pixel(p, PALLET_COLOR_NES_WHITE);
        } else {
            plot_pixel(p, PALLET_COLOR_NES_BLACK);
        }
    }

}
