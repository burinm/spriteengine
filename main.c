#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_keycode.h"

#include "graphics.h"
#include "blitz_n_chips.h"

#include "machine.h"
#define SCALE_WINDOW    (2)
#define SCREEN_X        (RESOLUTION_X * SCALE_WINDOW)
#define SCREEN_Y        (RESOLUTION_Y * SCALE_WINDOW)
#define SURFACE_DEPTH   (4)

static volatile int running = 1;
int main() {

SDL_Init(SDL_INIT_VIDEO);

/*
SDL_Window *w = SDL_CreateWindow(
                "RGB 320x240 Qqvga",
                0, 0,
                SCREEN_X,
                SCREEN_Y,
                0);
*/

SDL_Window *w;
SDL_Renderer *r;
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

/*
SDL_Surface *srgb = SDL_CreateRGBSurface(
                0,
                SCREEN_X, 
                SCREEN_Y,
                SURFACE_DEPTH,
                0,0,0,0);

g_clear_screen(srgb);
*/

SDL_Texture *t = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGB888,
        SDL_TEXTUREACCESS_STREAMING,
        //SCREEN_X, SCREEN_Y);
        RESOLUTION_X / 4, RESOLUTION_Y / 4);

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
#define TEXTURE_PITCH   (SCREEN_X * BITS_PER_PIXEL)
#define TOTAL_SCREEN_BUFFER (TEXTURE_PITCH * SCREEN_Y)
uint8_t *pixels = malloc(TOTAL_SCREEN_BUFFER * sizeof(uint8_t));
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
                        for (int j=0; j<RESOLUTION_Y; j++) {
                            for (int k=0; k<SCALE_WINDOW; k++) {
                                for (int i=0; i<RESOLUTION_X; i++) { //
                                    for (int z=0; z<SCALE_WINDOW; z++) {
                                        int r_pixel_color = rand() %255; 
                                        int g_pixel_color = rand() %255; 
                                        int b_pixel_color = rand() %255; 
                                        *_p++ = r_pixel_color;
                                        *_p++ = g_pixel_color;
                                        *_p++ = b_pixel_color;
                                        *_p++ = 0; //alpha?
                                    }
                                }
                            } 
                        }
                        SDL_UpdateTexture(t, NULL, pixels, TEXTURE_PITCH);

                        SDL_RenderClear(r);
                        SDL_RenderCopy(r, t, NULL, NULL);
                        SDL_RenderPresent(r);
                    }

                        break;
                    case SDLK_c: //clear
                        g_clear_render(r);
                        break;
                    default:
                        printf("key:%d\n", e.key.keysym.sym);
                }
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
