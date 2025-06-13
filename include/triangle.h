
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

#include <vector.h>

#define FACE_NUM_VERTICES 3

typedef struct {
    uint32_t a;
    uint32_t b;
    uint32_t c;
} face_t;

typedef struct {
    vector2_t vertices[3];
    float avg_depth;
} triangle_t;

void draw_filled_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, uint32_t color);

#endif