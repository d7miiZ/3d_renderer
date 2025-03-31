#include <math.h>

#include <vector.h>
#include <display.h>

vector2_t orthographic_projection(vector3_t vec3, uint32_t fov_factor) {
    vector2_t vec2;
    if (vec3.z == 0) {
        vec2.x = window_width / 2;
        vec2.y = window_height / 2;
    } else {
        vec2.x = (fov_factor * vec3.x) + (window_width / 2);
        vec2.y = (fov_factor * vec3.y) + (window_height / 2);
    }
    return vec2;
}

vector2_t perspective_projection(vector3_t vec3, uint32_t fov_factor) {
    vector2_t vec2;
    if (vec3.z == 0) {
        vec2.x = window_width / 2;
        vec2.y = window_height / 2;
    } else {
        vec2.x = ((fov_factor * vec3.x) / vec3.z) + (window_width / 2);
        vec2.y = ((fov_factor * vec3.y) / vec3.z) + (window_height / 2);
    }
    return vec2;
}
vector3_t vec3_rotate_x(vector3_t vec3, float angle) {
    vector3_t rotated_vec;
    rotated_vec.x = vec3.x;
    rotated_vec.y = vec3.y * cos(angle) - vec3.z * sin(angle);
    rotated_vec.z = vec3.y * sin(angle) + vec3.z * cos(angle);
    return rotated_vec;
}

vector3_t vec3_rotate_y(vector3_t vec3, float angle) {
    vector3_t rotated_vec;
    rotated_vec.x = vec3.x * cos(angle) - vec3.z * sin(angle);
    rotated_vec.y = vec3.y;
    rotated_vec.z = vec3.x * sin(angle) + vec3.z * cos(angle);
    return rotated_vec;
}

vector3_t vec3_rotate_z(vector3_t vec3, float angle) {
    vector3_t rotated_vec;
    rotated_vec.x = vec3.x * cos(angle) - vec3.y * sin(angle);
    rotated_vec.y = vec3.x * sin(angle) + vec3.y * cos(angle);
    rotated_vec.z = vec3.z;
    return rotated_vec;

}