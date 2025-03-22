#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#define SDL_INIT_ALL SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS | SDL_INIT_SENSOR | SDL_INIT_CAMERA

bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool init_window(void) {
    if (!SDL_Init(SDL_INIT_ALL)) {
        fprintf(stderr, "Error in init SDL %s\n", SDL_GetError());
        return false;
    }
    
    window = SDL_CreateWindow(NULL, 800, 600, SDL_WINDOW_BORDERLESS);
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

void setup(void) {

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

void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

int main(int argc, char const *argv[]) {
    is_running = init_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    return 0;
}