
#ifndef MESH_H
#define MESH_H

#include <vector.h>
#include <triangle.h>

#define CUBE_VERTICES 8
#define CUBE_FACES 6 * 2 // 6 cube faces, 2 triangle per face

extern vector3_t cube_vertices[CUBE_VERTICES];
extern face_t cube_faces[CUBE_FACES];

typedef struct {
    vector3_t rotations;
    vector3_t *vertices;
    face_t    *faces;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh();
void load_obj_file(const char *path);

#endif