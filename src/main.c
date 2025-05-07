#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 550;

enum dir { LEFT = 1, UP, RIGHT, DOWN };

typedef struct App {
    SDL_Window *win;
    SDL_Renderer *render;
    TTF_Font *font;
    int score;
    struct Snake *snake;
    SDL_Rect *playground;
    SDL_Point *egg;
} App;

typedef struct Body {
    int x, y;
    struct Body *next;
    struct Body *prev;
} Body;

typedef struct Snake {
    struct Body *head;
    struct Body *tail;
    int length;
    int dir;
} Snake;

int randNum(int max) {
    int min = 0;
    return min + rand() % (max - min + 1);
}

void drawBackground(App *app) {
    SDL_SetRenderDrawColor(app->render, 0, 0, 0, 255);
    SDL_RenderClear(app->render);

    SDL_SetRenderDrawColor(app->render, 255, 255, 255, 255);
    SDL_Rect rect = {app->playground->x, app->playground->y, app->playground->w, app->playground->h};
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
    SDL_SetRenderDrawColor(app->render, 0, 255, 0, 255);

    Body *current = app->snake->head;

    while (current != NULL) {
        if (current == app->snake->head) {
            SDL_SetRenderDrawColor(app->render, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(app->render, 0, 255, 0, 255);
        }
        SDL_Rect rect = {app->playground->x + current->x, app->playground->y + current->y, 10, 10};
        SDL_RenderFillRect(app->render, &rect);
        SDL_SetRenderDrawColor(app->render, 255, 255, 255, 255);
        SDL_RenderDrawRect(app->render, &rect);
        current = current->next;
    }
}

void drawEgg(App *app) {
    SDL_SetRenderDrawColor(app->render, 255, 255, 0, 255);
    SDL_Rect rect = {app->egg->x * 10 + app->playground->x, app->egg->y * 10 + app->playground->y, 10, 10};
    SDL_RenderFillRect(app->render, &rect);
}

void draw(App *app) {
    drawBackground(app);

    drawScore(app);

    drawEgg(app);

    drawSnake(app);
}

void updateSnake(Snake *snake) {
    Body *current = snake->tail;

    while (current != NULL && current->prev != NULL) {
        current->x = current->prev->x;
        current->y = current->prev->y;
        current = current->prev;
    }

    switch (snake->dir) {
    case LEFT:
        snake->head->x = snake->head->x - 10;
        break;
    case RIGHT:
        snake->head->x = snake->head->x + 10;
        break;
    case UP:
        snake->head->y = snake->head->y - 10;
        break;
    case DOWN:
        snake->head->y = snake->head->y + 10;
        break;
    }
}

int hitTest(App *app) {
    Body *head = app->snake->head;
    Body *tail = app->snake->tail;
    SDL_Rect *playground = app->playground;

    if (head->x + playground->x < playground->x) {
        return 1;
    } else if (head->x + 10 + playground->x > playground->x + playground->w) {
        return 1;
    } else if (head->y + playground->y < playground->y) {
        return 1;
    } else if (head->y + 10 + playground->y > playground->y + playground->h) {
        return 1;
    }

    SDL_Rect egg = {app->egg->x * 10 + app->playground->x, app->egg->y * 10 + app->playground->y, 10, 10};
    SDL_Rect headRect = {head->x + playground->x, head->y + playground->y, 10, 10};
    if (SDL_HasIntersection(&headRect, &egg)) {
        app->egg->x = randNum(45);
        app->egg->y = randNum(45);

        Body *body = malloc(sizeof(Body));
        body->x = tail->x;
        body->y = tail->y;
        tail->next = body;
        body->prev = tail;
        app->snake->tail = body;
        app->snake->length += 1;

        app->score = app->score + 10;
    }

    return 0;
}

void event_loop(App *app) {
    int snakeFrame = 0;

    while (1) {
        SDL_Event e;
        while (SDL_PollEvent(&e) == 1) {
            switch (e.type) {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                if (e.key.keysym.scancode == 20) {
                    return;
                }
                if (e.key.keysym.scancode == 79) {
                    if (app->snake->dir == UP || app->snake->dir == DOWN || app->snake->dir == RIGHT) {
                        app->snake->dir = RIGHT;
                        updateSnake(app->snake);
                    }
                }
                if (e.key.keysym.scancode == 80) {
                    if (app->snake->dir == UP || app->snake->dir == DOWN || app->snake->dir == LEFT) {
                        app->snake->dir = LEFT;
                        updateSnake(app->snake);
                    }
                }
                if (e.key.keysym.scancode == 82) {
                    if (app->snake->dir == LEFT || app->snake->dir == RIGHT || app->snake->dir == UP) {
                        app->snake->dir = UP;
                        updateSnake(app->snake);
                    }
                }
                if (e.key.keysym.scancode == 81) {
                    if (app->snake->dir == LEFT || app->snake->dir == RIGHT || app->snake->dir == DOWN) {
                        app->snake->dir = DOWN;
                        updateSnake(app->snake);
                    }
                }
                break;
            }
        }

        if (snakeFrame == 1000) {
            updateSnake(app->snake);
            snakeFrame = 0;
        }
        snakeFrame++;

        if (hitTest(app) != 0) {
            return;
        }

        draw(app);

        SDL_RenderPresent(app->render);
    }
}

void initSnake(Snake *snake) {
    SDL_Log("InitSnake");
    snake->length = 100;
    snake->dir = LEFT;

    Body *last = NULL;
    for (int i = 0; i < snake->length; i++) {
        struct Body *body = malloc(sizeof(Body));
        if (i == 0) {
            snake->head = body;
            body->x = 230;
            body->y = 230;
        }
        if (i == snake->length - 1) {
            snake->tail = body;
        }

        if (last != NULL) {
            body->prev = last;
            last->next = body;
            body->x = last->x + 10;
            body->y = last->y;
        }

        SDL_Log("x:%d y:%d", body->x, body->y);

        last = body;
    }
}

void destroySnake(Snake *snake) {
    Body *current = snake->head;
    while (current != NULL) {
        Body *temp = current->next;
        free(current);
        current = temp;
    }
}

int initApp(App *app) {
    SDL_Log("initApp");
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

    TTF_Font *font = TTF_OpenFont("../CaskaydiaCoveNerdFont-Regular.ttf", 32);
    if (font == NULL) {
        SDL_Log("create render error: %s\n", TTF_GetError());
        return 3;
    }

    Snake *snake = malloc(sizeof(Snake));
    initSnake(snake);

    SDL_Rect *playground = malloc(sizeof(SDL_Rect));
    playground->x = 20;
    playground->y = 70;
    playground->w = 460;
    playground->h = 460;

    SDL_Point *egg = malloc(sizeof(SDL_Point));
    egg->x = randNum(45);
    egg->y = randNum(45);

    app->win = win;
    app->render = render;
    app->font = font;
    app->score = 0;
    app->snake = snake;
    app->playground = playground;
    app->egg = egg;

    return 0;
}

void destroyApp(App *app) {
    destroySnake(app->snake);

    TTF_CloseFont(app->font);

    free(app->egg);
    free(app->playground);
    free(app->snake);
    free(app);
}

int main(int argc, char *args[]) {
    srand(time(NULL));
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
