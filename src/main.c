#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <display.h>
#include <vector.h>
#include <mesh.h>
#include <triangle.h>
#include <darray.h>

uint64_t prev_frame_time = 0;

vector3_t camera = { .x = 0, .y = 0, .z = 0};

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
        vector3_t transformed_vertices[3];
        triangle_t projected_triangle;
        face_t face = mesh.faces[i];

        vertices[0] = mesh.vertices[face.a - 1];
        vertices[1] = mesh.vertices[face.b - 1];
        vertices[2] = mesh.vertices[face.c - 1];

        for (size_t j = 0; j < 3; j++) {
            vector3_t transformed_vertex = vertices[j];

            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotations.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotations.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotations.z);
            transformed_vertex.z += 5;

            transformed_vertices[j] = transformed_vertex;
        }

        vector3_t vec_a = transformed_vertices[0];
        vector3_t vec_b = transformed_vertices[1];
        vector3_t vec_c = transformed_vertices[2];

        vector3_t vec_ab = vec3_sub(vec_b, vec_a);
        vector3_t vec_ac = vec3_sub(vec_c, vec_a);
        vec3_normalize(&vec_ab);
        vec3_normalize(&vec_ac);

        vector3_t vec_normal = vec3_cross(vec_ab, vec_ac);
        vec3_normalize(&vec_normal);

        vector3_t camera_ray = vec3_sub(camera, vec_a);

        float alignment_val = vec3_dot(vec_normal, camera_ray);

        // Backface culling
        if (alignment_val < 0) {
            continue;
        }

        for (size_t j = 0; j < 3; j++) {
            projected_triangle.vertices[j] = perspective_projection(transformed_vertices[j], 640);;
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