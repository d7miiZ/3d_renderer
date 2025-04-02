
#ifndef MESH_H
#define MESH_H

#include <vector.h>
#include <triangle.h>

#define MESH_VERTICES 8
#define MESH_FACES 6 * 2 // 6 cube faces, 2 triangle per face

extern vector3_t mesh_vertices[MESH_VERTICES];
extern face_t mesh_faces[MESH_FACES];

#endif