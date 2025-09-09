
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

#include <vector.h>
#include <display.h>
#include <texture.h>

#define FACE_NUM_VERTICES 3

typedef struct {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    color_t color;
} face_t;

typedef struct {
    vector2_t vertices[3];
    tex2_t tex_coords[3];
    float avg_depth;
    color_t color;
} triangle_t;

void draw_filled_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, uint32_t color);
void draw_textured_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, tex2_t tex_coord0, tex2_t tex_coord1, tex2_t tex_coord2, const color_t *mesh_texture);

#endif