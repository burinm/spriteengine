#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_keycode.h"

#include "graphics.h"
#include "machine.h"
#include "screen_matrix.h"

#include "machine.h"
#include "spriteengine.h"

static volatile int running = 1;
static volatile int vsync_running = 0;

// scrolling demo
static volatile int level_loc=0;
static volatile int level_fine_control=0;

#define AUTO_SYNC

#ifdef AUTO_SYNC
static uint32_t do_vsync(uint32_t interval, void* param);
#endif

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
#ifdef AUTO_SYNC
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
                    case SDLK_k:
                        VIDEO_CTRL[SE_SCROLL_X] = ++VIDEO_CTRL[SE_SCROLL_X] & 0x7;
                        printf("[SE_SCROLL_X+] = %u\n",VIDEO_CTRL[SE_SCROLL_X]);
                        break;
                    case SDLK_j:
                        VIDEO_CTRL[SE_SCROLL_X] = --VIDEO_CTRL[SE_SCROLL_X] & 0x7;
                        printf("[SE_SCROLL_X-] = %u\n",VIDEO_CTRL[SE_SCROLL_X]);
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
                    case SDLK_e: //Clear matrix
                        machine_clear_screen_matrix();
                        break;
                    case SDLK_p: //pattern to matrix
                        machine_pattern_screen_matrix();
                        break;
                    case SDLK_b:{  //blit
                        uint8_t* _p = pixels;
                        int counter = 0;
                        for (int j=0; j<RESOLUTION_Y; j++) {
                            for (int i=0; i<RESOLUTION_X; i++) { //
                                int pixel_color = rand() %PALLET_ROM_SZ;
                                counter += plot_pixel(&_p, pixel_color) * BYTES_PER_PIXEL;
                            }
                        }
                        printf("counter = %d is good %d (%d)\n", counter, counter == TOTAL_TEXTURE_BUFFER, TOTAL_TEXTURE_BUFFER);
                    }

                        break;
                    case SDLK_t: //paint screen grey
                        for (int i=0; i<TOTAL_TEXTURE_BUFFER; i++) {
                            pixels[i] = 0xee;
                        }
                        break;
                    case SDLK_u: //Display character rom on screen
                        display_character_rom(pixels);
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

#ifdef AUTO_SYNC
SDL_RemoveTimer(timer_vsync);
while(vsync_running); //make sure last timer has stopped
#endif

g_done();
}

#ifdef AUTO_SYNC
static uint32_t do_vsync(uint32_t interval, void* param) {
    vsync_running = 1;
    vsync_params_t *v_param = (vsync_params_t*)param;

    //scrolling demo
    level_fine_control++;
    if (level_fine_control> 7) {
        level_fine_control = 0;
        level_loc++;
        if (level_loc > 100) {
            level_loc = 0;
        }
        matrix_scroll_left();
        load_screen_matrix_on_right_from_memory(level_loc);
    }
    VIDEO_CTRL[SE_SCROLL_X] = level_fine_control;


    //msync(SRCEEN_RAM_mem, SCREEN_MEM_SZ, MS_SYNC);
    screen_render_from_matrix(v_param->pixels);

    g_update_renderer(v_param->pixels);

//printf("tick\n");
    vsync_running = 0;
return (interval);
}
#endif
