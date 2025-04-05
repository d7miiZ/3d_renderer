
#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL3/SDL.h>

#include <vector.h>

#define SDL_INIT_ALL SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS | SDL_INIT_SENSOR | SDL_INIT_CAMERA
#define FPS 30
#define FRAME_TIME (1000 / FPS)

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

extern uint32_t *color_buffer;

extern uint32_t window_width;
extern uint32_t window_height;

bool init_window(void);
void cleanup(void);
void draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);
void draw_grid(uint32_t width);
void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void draw_triangle(vector2_t vec1, vector2_t vec2, vector2_t vec3, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);

#endif