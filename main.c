#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

void draw(SDL_Surface *surface, SDL_Window *win) {

    SDL_Surface *img = IMG_Load("cat.jpg");
    SDL_Rect imgRect = {0, 0, img->w, img->h};
    SDL_Rect desRect = {10, 10, img->w, img->h};
    SDL_UpperBlit(img, &imgRect, surface, &desRect);
    SDL_UpdateWindowSurface(win);

    SDL_FreeSurface(img);
}

void event_loop(SDL_Surface *surface, SDL_Window *win) {
    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e) == 1) {
            if (e.type == SDL_QUIT) {
                return;
            }
        }

        draw(surface, win);
    }
}

int main(int argc, char *args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return 2;
    }

    SDL_Window *win = SDL_CreateWindow("TestSample", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        SDL_Log("create window error: %s\n", SDL_GetError());
        return 3;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(win);

    Uint32 color = SDL_MapRGBA(surface->format, 0xff, 0xff, 0xff, 0x00);
    SDL_FillRect(surface, NULL, color);

    SDL_Rect rect = {0, 0, 100, 100};
    SDL_FillRect(surface, &rect, 0x00ff0000);

    ((Uint32 *)surface->pixels)[10] = 0xff000000;

    SDL_UpdateWindowSurface(win);

    event_loop(surface, win);

    SDL_DestroyWindow(win);

    SDL_Quit();
    return 0;
}
