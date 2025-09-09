#include <math.h>
#include <stdlib.h>

#include <vector.h>
#include <triangle.h>
#include <display.h>
#include <utils.h>

vector3_t _barycentric_coordinates(vector2_t a, vector2_t b, vector2_t c, vector2_t point) {
    vector2_t ac = vec2_sub(c, a);
    vector2_t ab = vec2_sub(b, a);
    vector2_t pc = vec2_sub(c, point);
    vector2_t pb = vec2_sub(b, point);
    vector2_t ap = vec2_sub(point, a);

    float area_parallelogram_abc = (ac.x * ab.y) - (ac.y * ab.x); // || AC x AB ||
    float area_parallelogram_pbc = (pc.x * pb.y) - (pc.y * pb.x); // || PC x PB ||
    float area_parallelogram_apc = (ac.x * ap.y) - (ac.y * ap.x); // || AC x AP ||

    float alpha = area_parallelogram_pbc / area_parallelogram_abc;
    float beta = area_parallelogram_apc / area_parallelogram_abc;
    float gamma = 1.0f - alpha - beta;

    return (vector3_t) {alpha, beta, gamma};
    
}

void _draw_texel(vector2_t point, vector2_t a, vector2_t b, vector2_t c, tex2_t tex_a, tex2_t tex_b, tex2_t tex_c, const color_t *mesh_texture) {
    vector3_t barycentric_coords = _barycentric_coordinates(a, b, c, point);
    float alpha = barycentric_coords.x;
    float beta = barycentric_coords.y;
    float gamma = barycentric_coords.z;

    float interpolated_u = tex_a.u * alpha + tex_b.u * beta + tex_c.u * gamma;
    float interpolated_v = tex_a.v * alpha + tex_b.v * beta + tex_c.v * gamma;

    int tex_x = abs((int) (interpolated_u * texture_width));
    int tex_y = abs((int) (interpolated_v * texture_height));

    color_t color = mesh_texture[tex_y * texture_width + tex_x];
    draw_pixel(point.x, point.y, color);
}

void _textured_flat_bottom_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, tex2_t tex_coord0, tex2_t tex_coord1, tex2_t tex_coord2, const color_t *mesh_texture) {
    float inv_slope1 =  0, inv_slope2 = 0;
    
    if (vec1.y - vec0.y > 0) {
        inv_slope1 = (float) (vec1.x - vec0.x) / fabsf(vec1.y - vec0.y);
    }
    if (vec2.y - vec0.y > 0) {
        inv_slope2 = (float) (vec2.x - vec0.x) / fabsf(vec2.y - vec0.y);
    }

    for (int y = vec0.y; y <= vec1.y; y++) {
        int x_start = vec1.x + (y - vec1.y) * inv_slope1;
        int x_end = vec0.x + (y - vec0.y) * inv_slope2;

        if (x_start > x_end) {
            swap(&x_start, &x_end, sizeof(int));
        }
        
        for (int x = x_start; x < x_end; x++) {
            _draw_texel((vector2_t) {x, y}, vec0, vec1, vec2, tex_coord0, tex_coord1, tex_coord2, mesh_texture);
        }
    }
}

void _textured_flat_top_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, tex2_t tex_coord0, tex2_t tex_coord1, tex2_t tex_coord2, const color_t *mesh_texture) {
    float inv_slope1 =  0, inv_slope2 = 0;
    
    if (vec2.y - vec1.y > 0) {
        inv_slope1 = (float) (vec2.x - vec1.x) / fabsf(vec2.y - vec1.y);
    }
    if (vec2.y - vec0.y > 0) {
        inv_slope2 = (float) (vec2.x - vec0.x) / fabsf(vec2.y - vec0.y);
    }

    for (int y = vec1.y; y <= vec2.y; y++) {
        int x_start = vec1.x + (y - vec1.y) * inv_slope1;
        int x_end = vec0.x + (y - vec0.y) * inv_slope2;

        if (x_start > x_end) {
            swap(&x_start, &x_end, sizeof(int));
        }
        
        for (int x = x_start; x < x_end; x++) {
            _draw_texel((vector2_t) {x, y}, vec0, vec1, vec2, tex_coord0, tex_coord1, tex_coord2, mesh_texture);
        }
    }
}

void _fill_flat_bottom_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, color_t color) {
    float inv_slope1 =  0, inv_slope2 = 0;
    float x_start = vec0.x, x_end = vec0.x;

    if (vec1.y - vec0.y > 0) {
        inv_slope1 = (float) (vec1.x - vec0.x) / fabsf(vec1.y - vec0.y);
    }
    if (vec2.y - vec0.y > 0) {
        inv_slope2 = (float) (vec2.x - vec0.x) / fabsf(vec2.y - vec0.y);
    }

    for (int y = vec0.y; y <= vec2.y; y++) {
        draw_line(x_start, y, x_end, y, color);
        x_start += inv_slope1;
        x_end += inv_slope2;
    }
}

void _fill_flat_top_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, color_t color) {
    float inv_slope1 =  0, inv_slope2 = 0;
    float x_start = vec2.x, x_end = vec2.x;

    if (vec2.y - vec0.y > 0) {
        inv_slope1 =(float) (vec2.x - vec0.x) / fabsf(vec2.y - vec0.y);
    }
    if (vec2.y - vec1.y > 0) {
        inv_slope2 = (float) (vec2.x - vec1.x) / fabsf(vec2.y - vec1.y);
    }

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
        swap(&vec0, &vec1, sizeof(vector2_t));
    }
    if (vec1.y > vec2.y) {
        swap(&vec1, &vec2, sizeof(vector2_t));
    }
    if (vec0.y > vec1.y) {
        swap(&vec0, &vec1, sizeof(vector2_t));
    }

    if (vec1.y == vec2.y) {
        _fill_flat_bottom_triangle(vec0, vec1, vec2, color);
    } else if (vec0.y == vec1.y) {
        _fill_flat_top_triangle(vec0, vec1, vec2, color);
    } else {
        m.y = vec1.y;
        m.x = ((float) ((vec2.x - vec0.x) * (vec1.y - vec0.y)) / (float) (vec2.y - vec0.y)) + vec0.x; // Using triangle similarity to get m.x value
    
        _fill_flat_bottom_triangle(vec0, vec1, m, color);
        _fill_flat_top_triangle(vec1, m, vec2, color);
    }
}

void draw_textured_triangle(vector2_t vec0, vector2_t vec1, vector2_t vec2, tex2_t tex_coord0, tex2_t tex_coord1, tex2_t tex_coord2, const color_t *mesh_texture) {
    vector2_t m;
    if (vec0.y > vec1.y) {
        swap(&vec0, &vec1, sizeof(vector2_t));
        swap(&tex_coord0, &tex_coord1, sizeof(tex2_t));
    }
    if (vec1.y > vec2.y) {
        swap(&vec1, &vec2, sizeof(vector2_t));
        swap(&tex_coord1, &tex_coord2, sizeof(tex2_t));
    }
    if (vec0.y > vec1.y) {
        swap(&vec0, &vec1, sizeof(vector2_t));
        swap(&tex_coord0, &tex_coord1, sizeof(tex2_t));
    }

    if (vec1.y == vec2.y) {
        _textured_flat_bottom_triangle(vec0, vec1, vec2, tex_coord0, tex_coord1, tex_coord2, mesh_texture);
    } else if (vec0.y == vec1.y) {
        _textured_flat_top_triangle(vec0, vec1, vec2, tex_coord0, tex_coord1, tex_coord2, mesh_texture);
    } else {
        m.y = vec1.y;
        m.x = ((float) ((vec2.x - vec0.x) * (vec1.y - vec0.y)) / (float) (vec2.y - vec0.y)) + vec0.x; // Using triangle similarity to get m.x value
    
        _textured_flat_bottom_triangle(vec0, vec1, m, tex_coord0, tex_coord1, tex_coord2, mesh_texture);
        _textured_flat_top_triangle(vec1, m, vec2, tex_coord1, tex_coord2, tex_coord2, mesh_texture);
    }
}