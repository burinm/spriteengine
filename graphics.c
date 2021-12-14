#include <assert.h>

#include "graphics.h"
#include "machine.h"

static SDL_Window *w;
static SDL_Renderer *r;
static SDL_Texture *t;
static uint8_t *pixels;


int g_init() {
    SDL_Init(SDL_INIT_VIDEO);

    if (SDL_CreateWindowAndRenderer(
                    SCREEN_X, SCREEN_Y,
                    SDL_WINDOW_RESIZABLE,
                    &w,
                    &r) != 0) {
        printf("Unable to create window! %s\n", SDL_GetError());
        return -1;
    };

    g_clear_render(r);

return 0;
}


uint8_t * g_texture_init() {
    t = SDL_CreateTexture(
            r,
            SDL_PIXELFORMAT_RGB888,
            //SDL_PIXELFORMAT_RGB332,
            SDL_TEXTUREACCESS_STREAMING,
            RESOLUTION_X, RESOLUTION_Y);

    if(t == NULL) {
        printf("Texture Null: %s\n", SDL_GetError());
        return NULL;
    }

    pixels = malloc(TOTAL_TEXTURE_BUFFER * sizeof(uint8_t));
    if (pixels == NULL) {
        printf("Coudlnt' allocate pixel buffer!\n");
        return NULL;
    }

return pixels;
}

void g_done() {
    if(pixels) { free(pixels); }

    SDL_DestroyTexture(t);
    SDL_DestroyWindow(w);

    SDL_Quit();
}

void g_clear_screen(SDL_Surface *s) {
    if (s) {
        memset(s->pixels, 0, s->h * s->pitch); 
    }
}

void g_clear_render(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderClear(r);
    SDL_RenderPresent(r);
}

void g_update_renderer(uint8_t *pixels) {
    SDL_UpdateTexture(t, NULL, pixels, TEXTURE_PITCH);
    SDL_RenderClear(r);
    SDL_RenderCopy(r, t, NULL, NULL);
    SDL_RenderPresent(r);
}

