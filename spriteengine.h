#ifndef __SPRINTEENGINE_H__ 
#define __SPRINTEENGINE_H__ 

#include <stdint.h>

//Sprite Engine Registers
#define SE_REG_SZ               (512)

//Register relative address
#define SE_BACKGROUND_COLOR     (0x0) 
#define SE_SCROLL_X             (0x1) 
#define SE_SCROLL_Y             (0x1) 

void screen_render_from_matrix(uint8_t *p);

int v_blank(uint8_t **p, int lines, int color);
int h_blank(uint8_t **p, int dots);

int plot_pixel(uint8_t **p, int color);
int plot_raster_line(uint8_t **p, uint16_t line);

void display_character_rom(uint8_t *p);


#endif
