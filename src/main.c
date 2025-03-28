#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#define SDL_INIT_ALL SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS | SDL_INIT_SENSOR | SDL_INIT_CAMERA
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

bool is_running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

uint32_t *color_buffer = NULL;

bool init_window(void) {
    if (!SDL_Init(SDL_INIT_ALL)) {
        fprintf(stderr, "Error in init SDL %s\n", SDL_GetError());
        return false;
    }
    
    window = SDL_CreateWindow(NULL, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
    if (!window){
        fprintf(stderr, "Error in creating window %s\n", SDL_GetError());
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer){
        fprintf(stderr, "Error in creating renderer %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool setup(void) {
    color_buffer = calloc(WINDOW_WIDTH * WINDOW_HEIGHT, sizeof(uint32_t));
    if (!color_buffer) {
        fprintf(stderr, "Error in creating color buffer memory\n");
        return false;
    }
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!texture) {
        fprintf(stderr, "Error in creating texture %s\n", SDL_GetError());
        return false;
    }
    
    return true;
}

void process_input(void) {
    SDL_Event event;

    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_EVENT_QUIT:
        is_running = false;
        break;
    case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_ESCAPE) {
            is_running = false;
        }
        break;
    default:
        break;
    }
}

void update(void) {

}

void clear_color_buffer(uint32_t color){
    for (size_t i = 0; i < WINDOW_HEIGHT; i++) {
        for (size_t j = 0; j < WINDOW_WIDTH; j++) {
            color_buffer[(i * WINDOW_WIDTH) + j] = color;
        }
    }
}

void render_color_buffer(void){
    SDL_UpdateTexture(texture, NULL, color_buffer, sizeof(uint32_t) * WINDOW_WIDTH);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);

    render_color_buffer();
    clear_color_buffer(0xFFFFFF00);

    SDL_RenderPresent(renderer);
}

void cleanup(void) {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char const *argv[]) {
    is_running = init_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    cleanup();
    return 0;
}