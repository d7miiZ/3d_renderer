#ifndef MATRIX_H
#define MATRIX_H

#include <vector.h>

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_translation(float tx, float ty, float tz);
mat4_t mat4_rotation_x(float angle);
mat4_t mat4_rotation_y(float angle);
mat4_t mat4_rotation_z(float angle);

vector4_t mat4_multiply_vector(mat4_t m, vector4_t v);
mat4_t mat4_multiply_matrix(mat4_t a, mat4_t b);

#endif