#ifndef LIGHT_H
#define LIGHT_H

#include <vector.h>
#include <display.h>

typedef struct {
    vector3_t direction;
} light_t;

color_t light_apply_intensity(color_t color, float intensity);

extern light_t light;

#endif