#include<stdlib.h>

#include <mesh.h>
#include <darray.h>

mesh_t mesh = {
    .faces = NULL,
    .vertices = NULL,
    .rotations = { 0, 0, 0},
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