#ifndef __SCREEN_MATRIX_H__
#define __SCREEN_MATRIX_H__

#include <stdint.h>

int load_screen_matrix(const char*);
void screen_render_from_matrix(uint8_t *);
 
void matrix_scroll_left();
void load_screen_matrix_on_right_from_memory(size_t offset);

#endif
