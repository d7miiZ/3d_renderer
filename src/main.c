#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <display.h>
#include <vector.h>

#define CUBE_DIMENSION 9 * 9 * 9

vector3_t camera = { .x = 0, .y = 0, .z = -5};
vector3_t cube_rotation = { .x = 0, .y = 0, .z = 0};

vector3_t cube[CUBE_DIMENSION];
vector2_t projected_cube[CUBE_DIMENSION];

bool is_running = false;

bool setup(void) {
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
    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (size_t i = 0; i < CUBE_DIMENSION; i++) {
        vector3_t cube_point = cube[i];
        vector3_t transformed_cube_point = vec3_rotate_x(cube_point, cube_rotation.x);
        transformed_cube_point = vec3_rotate_y(transformed_cube_point, cube_rotation.y);
        transformed_cube_point = vec3_rotate_z(transformed_cube_point, cube_rotation.z);
        transformed_cube_point.z -= camera.z;
        projected_cube[i] = perspective_projection(transformed_cube_point, 640);
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