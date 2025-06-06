#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <display.h>
#include <vector.h>
#include <mesh.h>
#include <triangle.h>
#include <darray.h>

uint64_t prev_frame_time = 0;

vector3_t camera = { .x = 0, .y = 0, .z = -5};

triangle_t *triangles = NULL;

bool is_running = false;

bool setup(const char *path) {
    load_obj_file(path);
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

    triangles = NULL;

    mesh.rotations.x += 0.01;
    mesh.rotations.y += 0.01;
    mesh.rotations.z += 0.01;

    for (size_t i = 0; i < array_length(mesh.faces); i++) {
        vector3_t vertices[3];
        triangle_t projected_triangle;
        face_t face = mesh.faces[i];

        vertices[0] = mesh.vertices[--face.a];
        vertices[1] = mesh.vertices[--face.b];
        vertices[2] = mesh.vertices[--face.c];

        for (size_t j = 0; j < 3; j++) {
            vector3_t transformed_vertex = vertices[j];
            vector2_t projected_vertex;

            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotations.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotations.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotations.z);
            transformed_vertex.z -= camera.z;

            projected_vertex = perspective_projection(transformed_vertex, 640);

            projected_triangle.vertices[j] = projected_vertex;
        }

        array_push(triangles, projected_triangle);
    }
}

void render(void) {
    draw_grid(20);

    for (size_t i = 0; i < array_length(triangles); i++) {
        triangle_t triangle = triangles[i];
        for (size_t j = 0; j < 3; j++) {
            draw_rect(triangle.vertices[j].x, triangle.vertices[j].y, 4, 4, 0xFFFFFF00);
        }
        draw_triangle(triangle.vertices[0], triangle.vertices[1], triangle.vertices[2], 0xFFFF00FF);
    }

    array_free(triangles);
    
    render_color_buffer();

    SDL_RenderPresent(renderer);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    const char *filepath = argv[1];
    printf("File path passed: %s\n", filepath);

    is_running = init_window();

    setup(filepath);

    while (is_running) {
        process_input();
        update();
        render();
    }

    cleanup();
    return 0;
}