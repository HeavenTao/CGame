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
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 550;

typedef struct App {
    SDL_Window *win;
    SDL_Renderer *render;
    TTF_Font *font;
    int score;
    struct Snake *snake;
} App;

typedef struct Body {
    int x, y, w, h;
    struct Body *next;
    struct Body *prev;
} Body;

typedef struct Snake {
    struct Body *head;
    struct Body *tail;
    int length;
} Snake;

void drawBackground(App *app) {
    SDL_SetRenderDrawColor(app->render, 0, 0, 0, 255);
    SDL_RenderClear(app->render);

    SDL_SetRenderDrawColor(app->render, 255, 255, 255, 255);
    SDL_Rect rect = {25, 75, 450, 450};
    SDL_RenderDrawRect(app->render, &rect);
}

void drawScore(App *app) {
    char text[100];
    sprintf(text, "score:%d", app->score);

    SDL_Color color = {255, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderUTF8_Blended(app->font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app->render, surface);

    SDL_Rect rect = {0, 0, surface->w, surface->h};
    SDL_RenderCopy(app->render, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawSnake(App *app) {
    SDL_SetRenderDrawColor(app->render, 255, 0, 0, 0);
    SDL_Rect rect = {0, 0, 100, 100};
}

void draw(App *app) {
    drawBackground(app);

    drawScore(app);

    /*drawSnake(app);*/
}

int update(App *app) {
    SDL_Event e;
    while (SDL_PollEvent(&e) == 1) {
        switch (e.type) {
        case SDL_QUIT:
            return 0;
        case SDL_KEYDOWN:
            if (e.key.keysym.scancode == 20) {
                return 0;
            }
            if (e.key.keysym.scancode == 79) {
            }
            if (e.key.keysym.scancode == 80) {
            }
            SDL_Log("%d", e.key.keysym.scancode);
            break;
        }
    }

    return 1;
}

void event_loop(App *app) {
    while (1) {
        int code = update(app);

        if (code == 0) {
            return;
        }

        draw(app);

        SDL_UpdateWindowSurface(app->win);

        SDL_RenderPresent(app->render);
    }
}

void initSnake(Snake *snake) {
    snake->length = 3;

    for (int i = 0; i < snake->length; i++) {
        struct Body *body = malloc(sizeof(Body));

        if (i == 0) {
            snake->head = body;
        }
        if (i == 2) {
            snake->tail = body;
        }
    }
}

void destroySnake(Snake *snake) {}

int initApp(App *app) {
    SDL_Window *win = SDL_CreateWindow("TestSample", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (win == NULL) {
        SDL_Log("create window error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (render == NULL) {
        SDL_Log("create render error: %s\n", SDL_GetError());
        return 2;
    }

    SDL_Rect *rect = malloc(sizeof(SDL_Rect));
    rect->x = 0;
    rect->y = 0;
    rect->w = 100;
    rect->h = 100;

    TTF_Font *font = TTF_OpenFont("../CaskaydiaCoveNerdFont-Regular.ttf", 32);
    if (font == NULL) {
        SDL_Log("create render error: %s\n", TTF_GetError());
        return 3;
    }

    Snake *snake = malloc(sizeof(Snake));

    initSnake(snake);

    app->win = win;
    app->render = render;
    app->font = font;
    app->score = 0;

    return 0;
}

void destroyApp(App *app) {
    destroySnake(app->snake);

    TTF_CloseFont(app->font);

    free(app);
}

int main(int argc, char *args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return 2;
    }

    if (TTF_Init() < 0) {
        SDL_Log("SDL_TTF Error: %s\n", TTF_GetError());
        return 3;
    }

    App *app = malloc(sizeof(App));

    initApp(app);

    event_loop(app);

    SDL_DestroyWindow(app->win);

    destroyApp(app);

    TTF_Quit();

    SDL_Quit();

    return 0;
}
