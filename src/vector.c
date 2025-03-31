#include <vector.h>
#include <display.h>

vector2_t orthographic_projection(vector3_t vec3, uint32_t fov_factor) {
    vector2_t vec2;
    if (vec3.z == 0) {
        vec2.x = window_height / 2;
        vec2.y = window_width / 2;
    } else {
        vec2.x = (fov_factor * vec3.x) + (window_height / 2);
        vec2.y = (fov_factor * vec3.y) + (window_width / 2);
    }
    return vec2;
}

vector2_t perspective_projection(vector3_t vec3, uint32_t fov_factor) {
    vector2_t vec2;
    if (vec3.z == 0) {
        vec2.x = window_height / 2;
        vec2.y = window_width / 2;
    } else {
        vec2.x = ((fov_factor * vec3.x) / vec3.z) + (window_height / 2);
        vec2.y = ((fov_factor * vec3.y) / vec3.z) + (window_width / 2);
    }
    return vec2;
}