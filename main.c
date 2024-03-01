#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[]) {
  SDL_Window *window = NULL;
  SDL_Surface *screenSurface = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error hehhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("SDL sample", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("could not create window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  screenSurface = SDL_GetWindowSurface(window);

  SDL_Rect rect = {0, 0, 100, 100};

  SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

  SDL_UpdateWindowSurface(window);

  SDL_bool quit = SDL_FALSE;

  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      SDL_Log("PollEvent");
      if (e.type == SDL_QUIT) {
        quit = SDL_TRUE;
      } else if (e.type == SDL_KEYDOWN) {
        SDL_Log("KeyDown");
      }
    }

    SDL_Delay(16);
  }

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
