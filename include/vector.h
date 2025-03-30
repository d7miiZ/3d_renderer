
#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vector2_t;

typedef struct {
    float x;
    float y;
    float z;
} vector3_t;

vector2_t simple_project(vector3_t vec3);

#endif