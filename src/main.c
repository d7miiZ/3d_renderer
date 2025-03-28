#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#define SDL_INIT_ALL SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS | SDL_INIT_SENSOR | SDL_INIT_CAMERA

bool is_running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

uint32_t window_width = 800;
uint32_t window_height = 600;
uint32_t *color_buffer = NULL;

bool init_window(void) {
    int count;
    const SDL_DisplayMode *display_mode;
    
    if (!SDL_Init(SDL_INIT_ALL)) {
        fprintf(stderr, "Error in init SDL %s\n", SDL_GetError());
        return false;
    }

    SDL_DisplayID *displays = SDL_GetDisplays(&count);
    if (!displays || count <= 0) {
        printf("No displays found! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    display_mode = SDL_GetCurrentDisplayMode(displays[0]);
    if (!display_mode){
        fprintf(stderr, "Error in creating display_mode %s\n", SDL_GetError());
        return false;
    }
    window_width = display_mode->w;
    window_height = display_mode->h;

    window = SDL_CreateWindow(NULL, window_width, window_height, SDL_WINDOW_BORDERLESS);
    if (!window){
        fprintf(stderr, "Error in creating window %s\n", SDL_GetError());
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer){
        fprintf(stderr, "Error in creating renderer %s\n", SDL_GetError());
        return false;
    }
     SDL_SetWindowFullscreen(window, true);
    return true;
}

bool setup(void) {
    color_buffer = calloc(window_width * window_height, sizeof(uint32_t));
    if (!color_buffer) {
        fprintf(stderr, "Error in creating color buffer memory\n");
        return false;
    }
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
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

void draw_grid(uint32_t width){
    int pixel;
    for (size_t i = 0; i < window_height; i++) {
        for (size_t j = 0; j < window_width; j++) {
            pixel = (i * window_width) + j;
            if (i % width == 0 || j % width == 0) {
                color_buffer[pixel] = 0xFF000000;
            }else{
                color_buffer[pixel] = 0xFFFFFFFF;
            }
        }
    }
}

void render_color_buffer(void){
    SDL_UpdateTexture(texture, NULL, color_buffer, sizeof(uint32_t) * window_width);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);

    render_color_buffer();
    draw_grid(30);

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