#include <vector.h>
#include <display.h>

#define FOV_FACTOR 128

vector2_t orthographic_projection(vector3_t vec3) {
    vector2_t vec2 = { .x = (FOV_FACTOR * vec3.x) + (window_width / 2), .y = (FOV_FACTOR * vec3.y) + (window_height / 2)};
    return vec2;
}