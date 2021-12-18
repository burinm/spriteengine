#include <assert.h>

#include "spriteengine.h"
#include "graphics.h" //Just for assert, TOTAL_TEXTURE_BUFFER
#include "machine.h"

static volatile uint8_t *_video_ctrl = VIDEO_CTRL;

void screen_render_from_matrix(uint8_t *p) {
    //Do this like a raster video system for now

int _dots = 0;
    // "v_blank" at top - but not really, we are writing black pixels
    _dots += v_blank(&p, SCREEN_TILE_Y_OFFSET * TILE_SZ, PALLET_COLOR_NES_BLACK);
    _dots += v_blank(&p, SCREEN_TILE_Y_OFFSET * TILE_SZ, PALLET_COLOR_NES_BLACK);

    for (int j=SCREEN_TILE_Y_OFFSET; j<SCREEN_MATRIX_Y - SCREEN_TILE_Y_OFFSET; j++) { // all rows
        for (int line=0; line<8; line++) { // one row of characters

        //start raster line
            _dots += h_blank(&p, SCREEN_TILE_X_OFFSET * TILE_SZ);
            _dots += h_blank(&p, SCREEN_TILE_X_OFFSET * TILE_SZ);
            _dots += h_blank(&p, SCREEN_TILE_X_OFFSET * TILE_SZ);

            for (int i=SCREEN_TILE_X_OFFSET; i<SCREEN_MATRIX_X - SCREEN_TILE_X_OFFSET; i++) { // visible section
                //x_Scroll(&p, video_ctrl[SE_SCROLL_X]);
                
                _dots += plot_character_line(&p, SRCEEN_RAM_mem[i + j*SCREEN_MATRIX_X], line);
            }

            _dots += h_blank(&p, SCREEN_TILE_X_OFFSET * TILE_SZ);
            _dots += h_blank(&p, SCREEN_TILE_X_OFFSET * TILE_SZ);
            _dots += h_blank(&p, SCREEN_TILE_X_OFFSET * TILE_SZ);
        //end raster line
        }
    }

    // "v_blank" at bottom
    _dots += v_blank(&p, SCREEN_TILE_Y_OFFSET * TILE_SZ, PALLET_COLOR_NES_BLACK);
    _dots += v_blank(&p, SCREEN_TILE_Y_OFFSET * TILE_SZ, PALLET_COLOR_NES_BLACK);

printf("rendered %d dots =? %d\n", _dots, RESOLUTION_X * RESOLUTION_Y);
if (_dots != RESOLUTION_X * RESOLUTION_Y) { printf("!!! scan wrong size !!!\n"); }
}

int v_blank(uint8_t **p, int lines, int color) {
int _dots = 0;

    for (int j=0; j<lines; j++) {
        for (int i=0; i<RESOLUTION_X; i++) { // one raster line
           _dots += plot_pixel(p, color);
        }
    }

return _dots;
}

int h_blank(uint8_t **p, int dots) {
int _dots =0;

    for (int j=0; j<dots; j++) {
        _dots += plot_pixel(p, PALLET_COLOR_NES_BLACK);
    }
return _dots;
}

int _screen_get_screencode_pos(int screencode, uint8_t line) {
assert(line < 8);
assert(screencode < (CHAR_ROM_SZ/8));

    return (screencode * 8 + line);
}

//mov3e to graphics module
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

return 1;
}

int plot_character_line(uint8_t **p, uint8_t screencode, uint8_t line) {

int _dots = 0;

    for (int i=0; i<8; i++) {
        if (CHARACTER_ROM_mem[_screen_get_screencode_pos(screencode, line)] & (1<<(7-i))) {
            _dots += plot_pixel(p, PALLET_COLOR_NES_WHITE);
        } else {
            _dots += plot_pixel(p, PALLET_COLOR_NES_BLACK);
        }
    }

return _dots;
}

void display_character_rom(uint8_t *p) {
//TODO: This isn't raster friendly
    int pos_x = 0;
    int pos_y = 0;
    int rom = 0;
    uint8_t *px = NULL;
    do {
        for (int k=0; k<8; k++) {
            int v = k * RESOLUTION_X * BYTES_PER_PIXEL;
            px = p + v + pos_x + pos_y;
            for (int j=0; j<8; j++) {
                int h = (j * BYTES_PER_PIXEL);

                if ( (v + h + pos_x + pos_y + BYTES_PER_PIXEL) >= TOTAL_TEXTURE_BUFFER) {
                    printf("overrun!\n");
                    goto bail;
                }

                // One bit of character, highest bit first
                if (CHARACTER_ROM_mem[rom] & (1<<(7-j))) {
                    plot_pixel(&px, PALLET_COLOR_NES_WHITE);
                } else {
                    plot_pixel(&px, PALLET_COLOR_NES_BLACK);
                }
            }
            rom++; //Next line of character
            px -= (8 * BYTES_PER_PIXEL);
            px += (RESOLUTION_X * BYTES_PER_PIXEL);
        }
        // Next character
        pos_x += 8 * BYTES_PER_PIXEL;
        if (pos_x > ((RESOLUTION_X * BYTES_PER_PIXEL) - (BYTES_PER_PIXEL * 8))) {
            pos_x = 0;
            pos_y += RESOLUTION_X * BYTES_PER_PIXEL * 8;
        }
        printf("pos_x %d, pos_y %d\n", pos_x, pos_y);
    } while (rom < CHAR_ROM_SZ -1);
    bail:;
}
