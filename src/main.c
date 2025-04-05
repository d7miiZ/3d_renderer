#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <display.h>
#include <vector.h>
#include <mesh.h>
#include <triangle.h>

uint64_t prev_frame_time = 0;

vector3_t camera = { .x = 0, .y = 0, .z = -5};
vector3_t cube_rotation = { .x = 0, .y = 0, .z = 0};

triangle_t triangles[MESH_FACES];

bool is_running = false;

bool setup(void) {
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
    int64_t time_to_wait = FRAME_TIME - (SDL_GetTicks() - prev_frame_time);

    if (time_to_wait > 0) {
        SDL_Delay(time_to_wait);
    }
    
    prev_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (size_t i = 0; i < MESH_FACES; i++) {
        vector3_t vertices[3];
        triangle_t projected_triangle;
        face_t face = mesh_faces[i];

        vertices[0] = mesh_vertices[--face.a];
        vertices[1] = mesh_vertices[--face.b];
        vertices[2] = mesh_vertices[--face.c];

        for (size_t j = 0; j < 3; j++) {
            vector3_t transformed_vertex = vertices[j];
            vector2_t projected_vertex;

            transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);
            transformed_vertex.z -= camera.z;

            projected_vertex = perspective_projection(transformed_vertex, 640);

            projected_triangle.vertices[j] = projected_vertex;
        }

        triangles[i] = projected_triangle;
    }
}

void render(void) {
    draw_grid(20);

    for (size_t i = 0; i < MESH_FACES; i++) {
        triangle_t triangle = triangles[i];
        for (size_t j = 0; j < 3; j++) {
            draw_rect(triangle.vertices[j].x, triangle.vertices[j].y, 4, 4, 0xFFFFFF00);
        }
        draw_triangle(triangle.vertices[0], triangle.vertices[1], triangle.vertices[2], 0xFFFF00FF);
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