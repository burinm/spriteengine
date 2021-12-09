#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_keycode.h"

#define RESOULTION_X    (320)
#define RESOULTION_Y    (240)
#define SCALE_WINDOW    (2)
#define SCREEN_X        (RESOULTION_Y * SCALE_WINDOW)
#define SCREEN_Y        (RESOULTION_Y * SCALE_WINDOW)
#define SURFACE_DEPTH   (4)

static volatile int running = 1;
int main() {

SDL_Init(SDL_INIT_VIDEO);

SDL_Window *w = SDL_CreateWindow(
                "RGB 320x240 Qqvga",
                0, 0,
                SCREEN_X,
                SCREEN_Y,
                0);

if (w ==NULL) {
    printf("Unablt to create window!\n");
    return -1;
}

SDL_Surface *srgb = SDL_CreateRGBSurface(
                0,
                SCREEN_X, 
                SCREEN_Y,
                SURFACE_DEPTH,
                0,0,0,0);
                
SDL_Event e;
while(running) {
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_KEYDOWN:
                printf("key:%d\n", e.key.keysym.sym);
                switch(e.key.keysym.sym) {
                    case SDLK_q:
                        running = 0;
                        printf("stopping...\n");
                }
                break;

            default:
                //printf("Unhandled event %d\n", e.type);
                break;
        }


    }
    



}


SDL_FreeSurface(srgb);
SDL_DestroyWindow(w);
SDL_Quit();








}
