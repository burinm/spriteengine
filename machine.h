#ifndef __MACHINE_H__
#define __MACHINE_H__

#include "spriteengine.h" // backwards, spriteengine defines video ctlr register size
#include <stdint.h>

//Resolution of machine output - fixed (qvga)
#define RESOLUTION_X    (320)
#define RESOLUTION_Y    (240)

//Character ROM (pattern table)
#define CHAR_ROM_SZ     (4096)
const volatile uint8_t* CHARACTER_ROM_mem;

//Some pallet colors for debug, not planned part of the machine - yet
#define PALLET_COLOR_NES_BLACK  (0x0)
#define PALLET_COLOR_NES_WHITE  (0x1)
#define PALLET_ROM_SZ     (64)
const volatile uint32_t PALLET_ROM_mem[PALLET_ROM_SZ];

//SpriteEngine mapped memory (control registers)
volatile uint8_t VIDEO_CTRL[SE_REG_SZ];



//Screen RAM memory (nametable), called matix in this implementation
#define TILE_SZ (8)
/* This could be just the resolution split up by 8x8 tiles..

    #define SCREEN_MATRIX_X (size_t)(RESOLUTION_X / TILE_SZ)
    #define SCREEN_MATRIX_Y (size_t)(RESOLUTION_Y / TILE_SZ)
    #define SCREEN_MEM_SZ   (SCREEN_MATRIX_X * SCREEN_MATRIX_Y)
*/

/* However, in order to support smooth scrolling, there will be a border
    of tiles around the viewing area that can be filled when they
    are not being displayed.

    The folowing is chosen to fit in a 1K block
    (the rational being, that it will make a hardware implementation easier)

                 36 tiles
    BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB  36x28 = 0x3f0
    B                  .               B
    B......34 tiles....................B  aspect ratio 34:26
    B                  .               B      17:13 ~1.3
    B                  .               B
    B                  .               B 28 tiles
    B                  .26 tiles       B
    B                  .               B
    B                  .               B
    B                  .               B
    BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

    B - border
    . - max area that will be displayed
*/
#define SCREEN_MATRIX_X (36)
#define SCREEN_MATRIX_Y (28)
#define SCREEN_MEM_SZ   (SCREEN_MATRIX_X * SCREEN_MATRIX_Y)
volatile uint8_t* SRCEEN_RAM_mem;

//Where to draw the screen from the upper-left
//TODO: make this generic so screen_render_from_matrix can be changed...
#define SCREEN_TILE_X_OFFSET   (1)
#define SCREEN_TILE_Y_OFFSET   (1)

typedef struct {
    uint8_t *pixels;
} vsync_params_t;

int machine_init_mem(void);

void machine_clear_screen_matrix(void);
void machine_pattern_screen_matrix(void);

#endif

