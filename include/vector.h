
#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef struct {
    float x;
    float y;
} vector2_t;

typedef struct {
    float x;
    float y;
    float z;
} vector3_t;

vector2_t orthographic_projection(vector3_t vec3, uint32_t fov_factor);
vector2_t perspective_projection(vector3_t vec3, uint32_t fov_factor);

#endif