
#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

#define VEC3_NUM_COMPONENTS 3
#define VEC2_NUM_COMPONENTS 2

typedef struct {
    float x;
    float y;
} vector2_t;

typedef struct {
    float x;
    float y;
    float z;
} vector3_t;

float vec2_len(vector2_t vec2);
vector2_t vec2_add(vector2_t a, vector2_t b);
vector2_t vec2_sub(vector2_t a, vector2_t b);
vector2_t vec2_mul(vector2_t vec2, float scalar);
vector2_t vec2_div(vector2_t vec2, float scalar);
float vec2_dot(vector2_t a, vector2_t b);
void vec2_normalize(vector2_t *vec2);

// ===================================================== VEC3 Funcs ====================================

vector2_t orthographic_projection(vector3_t vec3, uint32_t fov_factor);
vector2_t perspective_projection(vector3_t vec3, uint32_t fov_factor);

vector3_t vec3_rotate_x(vector3_t vec3, float angle);
vector3_t vec3_rotate_y(vector3_t vec3, float angle);
vector3_t vec3_rotate_z(vector3_t vec3, float angle);

float vec3_len(vector3_t vec3);
vector3_t vec3_add(vector3_t a, vector3_t b);
vector3_t vec3_sub(vector3_t a, vector3_t b);
vector3_t vec3_mul(vector3_t vec3, float scalar);
vector3_t vec3_div(vector3_t vec3, float scalar);
vector3_t vec3_cross(vector3_t a, vector3_t b);
float vec3_dot(vector3_t a, vector3_t b);
void vec3_normalize(vector3_t *vec3);

#endif