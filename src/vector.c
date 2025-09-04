#include <math.h>

#include <vector.h>
#include <display.h>

float vec2_len(vector2_t vec2) {
    return sqrt((vec2.x * vec2.x) + (vec2.y * vec2.y));
}

vector2_t vec2_add(vector2_t a, vector2_t b) {
    vector2_t res = {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
    return res;
}

vector2_t vec2_sub(vector2_t a, vector2_t b) {
    vector2_t res = {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
    return res;
}

vector2_t vec2_mul(vector2_t vec2, float scalar) {
    vector2_t res = {
        .x = vec2.x * scalar,
        .y = vec2.y * scalar,
    };
    return res;
}

vector2_t vec2_div(vector2_t vec2, float scalar) {
    vector2_t res = {
        .x = vec2.x / scalar,
        .y = vec2.y / scalar,
    };
    return res;
}

float vec2_dot(vector2_t a, vector2_t b) {
    return (a.x * b.x) + (a.y * b.y);
}

void vec2_normalize(vector2_t *vec2) {
    float vec_len = vec2_len(*vec2);
    
    vec2->x /= vec_len;
    vec2->y /= vec_len;
}

// ===================================================== VEC3 Funcs ====================================

float vec3_len(vector3_t *vec3) {
    return sqrt((vec3->x * vec3->x) + (vec3->y * vec3->y) + (vec3->z * vec3->z));
}

vector3_t vec3_add(vector3_t a, vector3_t b) {
    vector3_t res = {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
    return res;
}

vector3_t vec3_sub(vector3_t a, vector3_t b) {
    vector3_t res = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
    return res;
}

vector3_t vec3_cross(vector3_t a, vector3_t b) {
    vector3_t res = {
        .x = (a.y * b.z) - (a.z * b.y),
        .y = (a.z * b.x) - (a.x * b.z),
        .z = (a.x * b.y) - (a.y * b.x),
    };
    return res;
}

float vec3_dot(vector3_t a, vector3_t b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

vector3_t vec3_mul(vector3_t vec3, float scalar) {
    vector3_t res = {
        .x = vec3.x * scalar,
        .y = vec3.y * scalar,
        .z = vec3.z * scalar,
    };
    return res;
}

vector3_t vec3_div(vector3_t vec3, float scalar) {
    vector3_t res = {
        .x = vec3.x / scalar,
        .y = vec3.y / scalar,
        .z = vec3.z / scalar,
    };
    return res;
}

void vec3_normalize(vector3_t *vec3) {
    float vec_len = vec3_len(vec3);
    
    vec3->x /= vec_len;
    vec3->y /= vec_len;
    vec3->z /= vec_len;
}

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

vector3_t vec3_from_vec4(vector4_t vec4) {
    vector3_t vec3 = {
        .x = vec4.x,
        .y = vec4.y,
        .z = vec4.z,
    };
    return vec3;
}

vector4_t vec4_from_vec3(vector3_t vec3) {
    vector4_t vec4 = {
        .x = vec3.x,
        .y = vec3.y,
        .z = vec3.z,
        .w = 1,
    };
    return vec4;
}