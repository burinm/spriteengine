#ifndef __SPRINTEENGINE_H__ 
#define __SPRINTEENGINE_H__ 

void screen_render_from_matrix(uint8_t *p);

int plot_pixel(uint8_t **p, int color);
void plot_character_line(uint8_t **p, uint8_t screencode, uint8_t line);


#endif
