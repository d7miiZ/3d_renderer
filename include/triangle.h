
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

typedef struct {
    uint32_t a;
    uint32_t b;
    uint32_t c;
} face_t;

typedef struct {
    vector2_t vertices[3];
} triangle_t;

#endif