#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <mesh.h>
#include <darray.h>

#define MAX_BUFFER_SIZE 1024

mesh_t mesh = {
    .faces = NULL,
    .vertices = NULL,
    .rotations = { 0, 0, 0},
    .scale = { 1, 1, 1},
    .translation = { 0, 0, 0},
};

vector3_t cube_vertices[CUBE_VERTICES] = { 
    { .x = -1, .y = -1, .z = -1},
    { .x = -1, .y = 1, .z = -1},
    { .x = 1, .y = 1, .z = -1},
    { .x = 1, .y = -1, .z = -1},
    { .x = 1, .y = 1, .z = 1},
    { .x = 1, .y = -1, .z = 1},
    { .x = -1, .y = 1, .z = 1},
    { .x = -1, .y = -1, .z = 1}
};

face_t cube_faces[CUBE_FACES] = {
    { .a = 1, .b = 2, .c = 3},
    { .a = 1, .b = 3, .c = 4},
    { .a = 4, .b = 3, .c = 5},
    { .a = 4, .b = 5, .c = 6},
    { .a = 6, .b = 5, .c = 7},
    { .a = 6, .b = 7, .c = 8},
    { .a = 8, .b = 7, .c = 2},
    { .a = 8, .b = 2, .c = 1},
    { .a = 2, .b = 7, .c = 5},
    { .a = 2, .b = 5, .c = 3},
    { .a = 6, .b = 8, .c = 1},
    { .a = 6, .b = 1, .c = 4},
};

void load_cube_mesh() {
    for (size_t i = 0; i < CUBE_VERTICES; i++) {
        array_push(mesh.vertices, cube_vertices[i]);
    }
    for (size_t i = 0; i < CUBE_FACES; i++) {
        array_push(mesh.faces, cube_faces[i]);
    }
}

// TODO: just use sscanf :/
void load_obj_file(const char *path) {
    if (access(path, F_OK) != 0) {
        fprintf(stderr, "Not enough permission on file %s\n", path);
        return;
    }

    FILE *file = fopen(path, "r");

    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) {
        char *outer_token = NULL;
        char *token = strtok_r(buffer, " ", &outer_token);
        if (token == NULL) {
            continue;
        }

        if (strcmp(token, "v") == 0) {
            vector3_t vec3;
            float points[3] = {0};
            uint8_t n_points = 0;

            while ((token = strtok_r(NULL, " ", &outer_token)) && n_points < 3) {
                points[n_points++] = strtof(token, NULL);
            }

            if (n_points != 3) {
                fprintf(stderr, "Vertices line got less than 3 points\n");
                continue;
            }

            vec3.x = points[0];
            vec3.y = points[1];
            vec3.z = points[2];
            array_push(mesh.vertices, vec3);
        } else if (strcmp(token, "f") == 0) {
            face_t face;
            float index[3] = {0};
            uint8_t n_index = 0;
            char *inner_token = NULL;

            while ((token = strtok_r(NULL, " ", &outer_token)) && n_index < 3) {
                char *face_value = strtok_r(token, "/", &inner_token);
                if (face_value == NULL) {
                    break;
                }

                index[n_index++] = strtoul(face_value, NULL, 10);
            }

            if (n_index != 3) {
                fprintf(stderr, "Face indexes line got less than 3 index\n");
                continue;
            }

            face.a = index[0];
            face.b = index[1];
            face.c = index[2];
            array_push(mesh.faces, face);
        }
    }

    fclose(file);
}