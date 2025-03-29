#include <stdlib.h>

#include <display.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

uint32_t *color_buffer = NULL;

uint32_t window_width = 800;
uint32_t window_height = 600;

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

void draw_rect(uint32_t width, uint32_t height, uint32_t color){
    size_t height_start = window_height / 2;
    size_t height_end = height_start + height;
    size_t width_start = window_width / 2;
    size_t width_end = width_start + width;
    for (size_t i = height_start; i < height_end; i++) {
        for (size_t j = width_start; j < width_end; j++) {
            color_buffer[(i * window_width) + j] = color;
        }
    }
}

void render_color_buffer(void){
    SDL_UpdateTexture(texture, NULL, color_buffer, sizeof(uint32_t) * window_width);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
}

void cleanup(void) {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}