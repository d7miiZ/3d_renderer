#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <display.h>
#include <vector.h>

#define CUBE_DIMENSION 9 * 9 * 9

vector3_t cube[CUBE_DIMENSION];
vector2_t projected_cube[CUBE_DIMENSION];

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
    
    uint16_t count = 0;
    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                vector3_t vec = { .x = x, .y = y, .z = z};
                cube[count++] = vec;
            }
        }
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
    for (size_t i = 0; i < CUBE_DIMENSION; i++) {
        projected_cube[i] = orthographic_projection(cube[i]);
    }
}

void render(void) {
    draw_grid(20);

    for (size_t i = 0; i < CUBE_DIMENSION; i++) {
        vector2_t projection = projected_cube[i];
        draw_rect(projection.x, projection.y, 4, 4, 0xFFFFFF00);
    }
    
    render_color_buffer();

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