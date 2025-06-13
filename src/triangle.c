#include "vector.h"
#include <triangle.h>
#include <display.h>

void _vec2_swap(vector2_t *a, vector2_t*b) {
    vector2_t temp = *a;
    *a = *b;
    *b = temp;
}

void _fill_flat_bottom_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, color_t color) {
    float inv_slope1 =  (float) (vec1.x - vec0.x) / (vec1.y - vec0.y), inv_slope2 = (float) (vec2.x - vec0.x) / (vec2.y - vec0.y);
    float x_start = vec0.x, x_end = vec0.x;

    for (int y = vec0.y; y <= vec2.y; y++) {
        draw_line(x_start, y, x_end, y, color);
        x_start += inv_slope1;
        x_end += inv_slope2;
    }
}

void _fill_flat_top_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, color_t color) {
    float inv_slope1 =  (float) (vec2.x - vec0.x) / (vec2.y - vec0.y), inv_slope2 = (float) (vec2.x - vec1.x) / (vec2.y - vec1.y);
    float x_start = vec2.x, x_end = vec2.x;

    for (int y = vec2.y; y >= vec1.y; y--) {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inv_slope1;
        x_end -= inv_slope2;
    }
}

// Fill triangle using flat bottom + flat top method
void draw_filled_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, color_t color) {
    vector2_t m;
    if (vec0.y > vec1.y) {
        _vec2_swap(&vec0, &vec1);
    }
    if (vec1.y > vec2.y) {
        _vec2_swap(&vec1, &vec2);
    }
    if (vec0.y > vec1.y) {
        _vec2_swap(&vec0, &vec1);
    }

    if (vec1.y == vec2.y) {
        _fill_flat_bottom_triangle(vec0, vec1, vec2, color);
    } else if (vec0.y == vec1.y) {
        _fill_flat_top_triangle(vec1, vec1, vec2, color);
    } else {
        m.y = vec1.y;
        m.x = ((float) ((vec2.x - vec0.x) * (vec1.y - vec0.y)) / (float) (vec2.y - vec0.y)) + vec0.x; // Using triangle similarity to get m.x value
    
        _fill_flat_bottom_triangle(vec0, vec1, m, color);
        _fill_flat_top_triangle(vec1, m, vec2, color);
    }
}