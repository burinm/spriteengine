#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_keycode.h"

#include "graphics.h"
#include "blitz_n_chips.h"
#include "machine.h"
#include "screen_matrix.h"

#include "machine.h"
#include "spriteengine.h"

static volatile int running = 1;
int main() {



if (machine_init_mem() != 0) {
    printf("Failed to init machine.\n");
    return -1;
}

if (g_init() == -1) {
    printf("Could not initialize SDL2 and open window\n");
    return -1;
}

uint8_t *pixels = g_texture_init();
if (pixels == NULL) {
    return -1;
}

//Go!

//start vsync timer
#if 1
#define VSYNC_RATE_MS   (17)
//#define VSYNC_RATE_MS   (1000)
vsync_params_t vp_t = { .pixels = pixels };
SDL_TimerID timer_vsync;
if ( (timer_vsync = SDL_AddTimer(VSYNC_RATE_MS, do_vsync, &vp_t)) == 0) {
    printf("Couldn't start v_sync timer\n");
    return -1;
}
#endif
                
SDL_Event e;
while(running) {
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_q:
                        running = 0;
                        printf("stopping...\n");
                        break;
                    case SDLK_l: //load screen matrix
                        if (load_screen_matrix("sample_matrix.bg") != 0) {
                            printf("couldn't load: sample_matrix.bg\n");
                        }
                        break;
                    case SDLK_d: //dump screen matrix
                        for (int j=0; j<SCREEN_MATRIX_Y; j++) {
                            for (uint i=0; i<SCREEN_MATRIX_X; i++) {
                                printf("%.2x", SRCEEN_RAM_mem[i + j * SCREEN_MATRIX_X]);
                            }
                            printf("\n");
                        }
                        printf("\n");
                        break;
                    case SDLK_r: //Render screen from matrix 
                        screen_render_from_matrix(pixels);
                        break;
                    case SDLK_e: //Render screen from matrix 
                        machine_clear_screen_matrix();
                        break;
                    case SDLK_b:{  //blit
                        /*
                        for (int i=0; i<TOTAL_SCREEN_BUFFER; i++) {
                            pixels[i] = rand() %255;
                        }
                        */
                        uint8_t* _p = pixels;
                        int counter = 0;
                        for (int j=0; j<RESOLUTION_Y; j++) {
                            for (int i=0; i<RESOLUTION_X; i++) { //
                                int pixel_color = rand() %PALLET_ROM_SZ;
                                counter += plot_pixel(&_p, pixel_color);
                            }
                        }
                        printf("counter = %d is good %d (%d)\n", counter, counter == TOTAL_TEXTURE_BUFFER, TOTAL_TEXTURE_BUFFER);
                    }

                        break;
                    case SDLK_t:
                        for (int i=0; i<TOTAL_TEXTURE_BUFFER; i++) {
                            pixels[i] = 0xee;
                        }
                        break;
                    case SDLK_u: {
                        int pos_x = 0;
                        int pos_y = 0;
                        int rom = 0;
                        do {
                            for (int k=0; k<8; k++) {
                                int v = k * RESOLUTION_X * BYTES_PER_PIXEL;
                                for (int j=0; j<8; j++) { 
                                    int h = (j * BYTES_PER_PIXEL);

                                    if ( (v + h + pos_x + pos_y + BYTES_PER_PIXEL) >= TOTAL_TEXTURE_BUFFER) {
                                        printf("overrun!\n");
                                        goto bail;
                                    }

                                    // One bit of character, highest bit first
                                    if (CHARACTER_ROM_mem[rom] & (1<<(7-j))) {
                                        pixels[v+h+0 + pos_x + pos_y] = 0xff; 
                                        pixels[v+h+1 + pos_x + pos_y] = 0xff;
                                        pixels[v+h+2 + pos_x + pos_y] = 0xff;
                                        pixels[v+h+3 + pos_x + pos_y] = 0;
                                    } else {
                                        pixels[v+h+0 + pos_x + pos_y] = 0;
                                        pixels[v+h+1 + pos_x + pos_y] = 0;
                                        pixels[v+h+2 + pos_x + pos_y] = 0;
                                        pixels[v+h+3 + pos_x + pos_y] = 0;
                                    }
                                }
                                rom++; //Next line of character
                            }
                            // Next character
                            pos_x += 8 * BYTES_PER_PIXEL;
                            if (pos_x > ((RESOLUTION_X * BYTES_PER_PIXEL) - (BYTES_PER_PIXEL * 8))) {
                                pos_x = 0;
                                pos_y += RESOLUTION_X * BYTES_PER_PIXEL * 8;
                            }
                            printf("pos_x %d, pos_y %d\n", pos_x, pos_y);
                        } while (rom < CHAR_ROM_SZ -1);
                    }
                        bail:
                        break;
                    case SDLK_c: //clear
                        for (int i=0; i<TOTAL_TEXTURE_BUFFER; i++) {
                            pixels[i] = 0;
                        }
                        break;
                    default:
                        printf("key:%d\n", e.key.keysym.sym);
                }

                //Always update renderer
                    g_update_renderer(pixels);
                break;
            case SDL_QUIT:
                running = 0;
                break;

            default:
                //printf("Unhandled event %d\n", e.type);
                break;
        }
    }
}

g_done();
}
