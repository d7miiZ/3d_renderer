#ifndef MATRIX_H
#define MATRIX_H

#include <vector.h>

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_translation(float tx, float ty, float tz);

vector4_t mat4_multiply_vector(mat4_t m, vector4_t v);

#endif