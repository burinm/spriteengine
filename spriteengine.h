#ifndef __SPRINTEENGINE_H__ 
#define __SPRINTEENGINE_H__ 

#include <stdint.h>

void screen_render_from_matrix(uint8_t *p);

void v_blank(uint8_t **p, int lines, int color);
void h_blank(uint8_t **p, int dots);

int plot_pixel(uint8_t **p, int color);
void plot_character_line(uint8_t **p, uint8_t screencode, uint8_t line);

void display_character_rom(uint8_t *p);


#endif
