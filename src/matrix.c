#include <math.h>

#include <matrix.h>

mat4_t mat4_identity() {
    mat4_t m = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};

    return m;
}

mat4_t mat4_scale(float sx, float sy, float sz) {
    mat4_t m = mat4_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;
    return m;
}

mat4_t mat4_translation(float tx, float ty, float tz) {
    mat4_t m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    return m;
}

mat4_t mat4_rotation_x(float angle) {
    mat4_t m = mat4_identity();
    m.m[1][1] = cos(angle);
    m.m[1][2] = -sin(angle);
    m.m[2][1] = sin(angle);
    m.m[2][2] = cos(angle);
    return m;
}

mat4_t mat4_rotation_y(float angle) {
    mat4_t m = mat4_identity();
    m.m[0][0] = cos(angle);
    m.m[0][2] = sin(angle);
    m.m[2][0] = -sin(angle);
    m.m[2][2] = cos(angle);
    return m;
}

mat4_t mat4_rotation_z(float angle) {
    mat4_t m = mat4_identity();
    m.m[0][0] = cos(angle);
    m.m[0][1] = -sin(angle);
    m.m[1][0] = sin(angle);
    m.m[1][1] = cos(angle);
    return m;
}

mat4_t mat4_perspective(float fov, float aspect_ratio, float znear, float zfar) {
    mat4_t m = {{{0}}};
    m.m[0][0] = aspect_ratio * ( 1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-zfar * znear) / (zfar - znear);
    m.m[3][2] = 1.0;
    return m;
}

vector4_t mat4_multiply_vector(mat4_t m, vector4_t v) {
    vector4_t result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}

vector4_t mat4_multiply_projection(mat4_t projection_matrix, vector4_t v) {
    vector4_t result = mat4_multiply_vector(projection_matrix, v);

    // Perspective division, divide original z-value which is stored in w
    if (v.w != 0) {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}

mat4_t mat4_multiply_matrix(mat4_t a, mat4_t b) {
    mat4_t result = {{{0}}};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    
    return result;
}