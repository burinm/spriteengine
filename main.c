#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_keycode.h"

#include "graphics.h"
#include "blitz_n_chips.h"

#include "machine.h"
#define WINDOW_SCALE    (2)
#define SURFACE_DEPTH   (4)

static volatile int running = 1;
int main() {

SDL_Init(SDL_INIT_VIDEO);

SDL_Window *w;
SDL_Renderer *r;

#define SCREEN_X        (RESOLUTION_X * WINDOW_SCALE)
#define SCREEN_Y        (RESOLUTION_Y * WINDOW_SCALE)
if (SDL_CreateWindowAndRenderer(
                SCREEN_X, SCREEN_Y,
                SDL_WINDOW_RESIZABLE,
                &w,
                &r) != 0) {
    printf("Unable to create window! %s\n", SDL_GetError());
};

if (w == NULL) {
    printf("Window Null!\n");
    return -1;
}

g_clear_render(r);

SDL_Texture *t = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGB888,
        //SDL_PIXELFORMAT_RGB332,
        SDL_TEXTUREACCESS_STREAMING,
        RESOLUTION_X, RESOLUTION_Y);

if(t == NULL) {
    printf("Texture Null: %s\n", SDL_GetError());
    return -1;
}

#define BITS_PER_RED      (8)
#define BITS_PER_GREEN    (8)
#define BITS_PER_BLUE     (8)
#define BITS_PER_ALPHA    (8)
#define BITS_PER_PIXEL  (BITS_PER_RED + BITS_PER_GREEN + BITS_PER_BLUE + BITS_PER_ALPHA)
#define BYTES_PER_PIXEL   (BITS_PER_PIXEL / 8)
#define TEXTURE_PITCH   (RESOLUTION_X * BITS_PER_PIXEL)
#define TOTAL_TEXTURE_BUFFER (TEXTURE_PITCH * RESOLUTION_Y)

uint8_t *pixels = malloc(TOTAL_TEXTURE_BUFFER * sizeof(uint8_t));
if (pixels == NULL) {
    printf("Coudlnt' allocate pixel buffer!\n");
    return -1;
}



/*
SDL_Surface *ws = SDL_GetWindowSurface(w);

if(ws == NULL) {
    printf("Window Surface Null: %s\n", SDL_GetError());
    return -1;
}

if (SDL_BlitSurface(srgb,NULL,ws,NULL) != 0) {
    printf("Problem updating window: %s\n", SDL_GetError());
    return -1;
}
*/
                
SDL_Event e;
while(running) {
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_q:
                        running = 0;
                        printf("stopping...\n");
                    case SDLK_b:{  //blit
                        /*
                        for (int i=0; i<TOTAL_SCREEN_BUFFER; i++) {
                            pixels[i] = rand() %255;
                        }
                        */
                        uint8_t* _p = pixels;
                        int counter = 0;
                        for (int j=0; j<RESOLUTION_Y; j++) {
                            for (int k=0; k<WINDOW_SCALE; k++) {
                                for (int i=0; i<RESOLUTION_X; i++) { //
                                    for (int z=0; z<WINDOW_SCALE; z++) {
                                        int r_pixel_color = rand() %255; 
                                        int g_pixel_color = rand() %255; 
                                        int b_pixel_color = rand() %255; 
                                        *_p++ = r_pixel_color;
                                        *_p++ = g_pixel_color;
                                        *_p++ = b_pixel_color;
                                        *_p++ = 0; //alpha?
                                        counter+=4;
                                    }
                                }
                            } 
                        }
                        printf("counter = %d is good %d (%d)\n", counter, counter == TOTAL_TEXTURE_BUFFER, TOTAL_TEXTURE_BUFFER);
                    }

                        break;
                     case SDLK_t:
                        for (int i=0; i<TOTAL_TEXTURE_BUFFER; i++) {
                            pixels[i] = rand() %255;
                        }
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
                    SDL_UpdateTexture(t, NULL, pixels, TEXTURE_PITCH);

                    SDL_RenderClear(r);
                    SDL_RenderCopy(r, t, NULL, NULL);
                    SDL_RenderPresent(r);
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


//SDL_FreeSurface(srgb);
if(pixels) { free(pixels); }
SDL_DestroyTexture(t);

SDL_DestroyWindow(w);
SDL_Quit();








}
