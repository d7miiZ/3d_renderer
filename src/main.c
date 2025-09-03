#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <display.h>
#include <vector.h>
#include <mesh.h>
#include <triangle.h>
#include <darray.h>
#include <matrix.h>

uint64_t prev_frame_time = 0;

union {
    struct {
        uint8_t wireframe: 1;
        uint8_t dots: 1;
        uint8_t fill: 1;
        uint8_t backface_culling: 1;
        uint8_t resrev1: 1;
        uint8_t resrev2: 1;
        uint8_t resrev3: 1;
        uint8_t resrev4: 1;
    } options;
    uint8_t flags;
} render_options = { .options = {0, 0, 1, 1, 0, 0, 0, 0} };

vector3_t camera = { .x = 0, .y = 0, .z = 0};

triangle_t *triangles = NULL;

bool is_running = false;

int _depth_sort(const void *_a, const void *_b) {
    triangle_t const *a = _a, *b = _b;

    return b->avg_depth - a->avg_depth;
}

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
        if (event.key.key == SDLK_1) {
            render_options.options.dots = 1;
            render_options.options.wireframe = 1;
            render_options.options.fill = 0;
        }
        if (event.key.key == SDLK_2) {
            render_options.options.dots = 0;
            render_options.options.wireframe = 1;
            render_options.options.fill = 0;
        }
        if (event.key.key == SDLK_3) {
            render_options.options.dots = 0;
            render_options.options.wireframe = 0;
            render_options.options.fill = 1;
        }
        if (event.key.key == SDLK_4) {
            render_options.options.dots = 0;
            render_options.options.wireframe = 1;
            render_options.options.fill = 1;
        }
        if (event.key.key == SDLK_C) {
            render_options.options.backface_culling = 1;
        }
        if (event.key.key == SDLK_D) {
            render_options.options.backface_culling = 0;
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
    float sum_z_components = 0;
    
    prev_frame_time = SDL_GetTicks();

    triangles = NULL;

    mesh.rotations.x += 0.01;
    mesh.rotations.y += 0.01;
    mesh.rotations.z += 0.01;

    mesh.scale.x += 0.002;
    mesh.scale.y += 0.002;

    mesh.translation.x += 0.01;
    mesh.translation.z = 5.0;

    mat4_t scale_matrix = mat4_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translation_matrix = mat4_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t rotation_x_matrix = mat4_rotation_x(mesh.rotations.x);
    mat4_t rotation_y_matrix = mat4_rotation_y(mesh.rotations.y);
    mat4_t rotation_z_matrix = mat4_rotation_z(mesh.rotations.z);

    for (size_t i = 0; i < array_length(mesh.faces); i++) {
        vector3_t vertices[FACE_NUM_VERTICES];
        vector4_t transformed_vertices[FACE_NUM_VERTICES];
        triangle_t projected_triangle;
        face_t face = mesh.faces[i];

        vertices[0] = mesh.vertices[face.a - 1];
        vertices[1] = mesh.vertices[face.b - 1];
        vertices[2] = mesh.vertices[face.c - 1];

        for (size_t j = 0; j < FACE_NUM_VERTICES; j++) {
            vector4_t transformed_vertex = vec4_from_vec3(vertices[j]);

            // Transformations order is important
            transformed_vertex = mat4_multiply_vector(scale_matrix, transformed_vertex);
            
            transformed_vertex = mat4_multiply_vector(rotation_x_matrix, transformed_vertex);
            transformed_vertex = mat4_multiply_vector(rotation_y_matrix, transformed_vertex);
            transformed_vertex = mat4_multiply_vector(rotation_z_matrix, transformed_vertex);

            transformed_vertex = mat4_multiply_vector(translation_matrix, transformed_vertex);
            
            transformed_vertices[j] = transformed_vertex;
        }

        if (render_options.options.backface_culling) {
            vector3_t vec_a = vec3_from_vec4(transformed_vertices[0]);
            vector3_t vec_b = vec3_from_vec4(transformed_vertices[1]);
            vector3_t vec_c = vec3_from_vec4(transformed_vertices[2]);

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
        }
    
        for (size_t j = 0; j < FACE_NUM_VERTICES; j++) {
            projected_triangle.vertices[j] = perspective_projection(vec3_from_vec4(transformed_vertices[j]), 640);;
            sum_z_components += transformed_vertices[j].z;
        }
        projected_triangle.avg_depth = sum_z_components / FACE_NUM_VERTICES;

        array_push(triangles, projected_triangle);
    }

    // Using painters algo to sort faces
    // TODO: Use z-buffer
    qsort(triangles, array_length(triangles), sizeof(triangle_t), _depth_sort);
}

void render(void) {
    draw_grid(20);

    for (size_t i = 0; i < array_length(triangles); i++) {
        triangle_t triangle = triangles[i];
        if (render_options.options.dots) {
            for (size_t j = 0; j < FACE_NUM_VERTICES; j++) {
                draw_rect(triangle.vertices[j].x, triangle.vertices[j].y, 4, 4, 0xFFFFFF00);
            }
        }
        if (render_options.options.wireframe) {
            draw_triangle(triangle.vertices[0], triangle.vertices[1], triangle.vertices[2], 0xFFFFFFFF);
        }
        if (render_options.options.fill) {
            draw_filled_triangle(triangle.vertices[0], triangle.vertices[1], triangle.vertices[2], 0xFFFF00FF);
        }
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