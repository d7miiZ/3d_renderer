#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <display.h>

bool is_running = false;

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

void update(void) { }

void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);

    render_color_buffer();
    draw_grid(30);
    draw_rect(300, 200, 0xFF0000FF);

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

    cleanup();
    return 0;
}