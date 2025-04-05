#include <stdlib.h>

#include <display.h>
#include <mesh.h>
#include <darray.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

uint32_t *color_buffer = NULL;

uint32_t window_width = 800;
uint32_t window_height = 600;

bool init_window(void) {
    int count;
    const SDL_DisplayMode *display_mode;
    
    if (!SDL_Init(SDL_INIT_ALL)) {
        fprintf(stderr, "Error in init SDL %s\n", SDL_GetError());
        return false;
    }

    SDL_DisplayID *displays = SDL_GetDisplays(&count);
    if (!displays || count <= 0) {
        printf("No displays found! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    display_mode = SDL_GetCurrentDisplayMode(displays[0]);
    if (!display_mode){
        fprintf(stderr, "Error in creating display_mode %s\n", SDL_GetError());
        return false;
    }
    window_width = display_mode->w;
    window_height = display_mode->h;

    window = SDL_CreateWindow(NULL, window_width, window_height, SDL_WINDOW_BORDERLESS);
    if (!window){
        fprintf(stderr, "Error in creating window %s\n", SDL_GetError());
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer){
        fprintf(stderr, "Error in creating renderer %s\n", SDL_GetError());
        return false;
    }
     SDL_SetWindowFullscreen(window, true);

    color_buffer = calloc(window_width * window_height, sizeof(uint32_t));
    if (!color_buffer) {
        fprintf(stderr, "Error in creating color buffer memory\n");
        return false;
    }
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    if (!texture) {
        fprintf(stderr, "Error in creating texture %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x > window_width || y > window_height) {
        return;
    }
    
    color_buffer[(window_width * y) + x] = color;
}

// DDA algo
void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {
    int32_t delta_x = (x1 - x0);
    int32_t delta_y = (y1 - y0);

    int32_t side_len = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_len;
    float y_inc = delta_y / (float)side_len;

    float cur_x = x0;
    float cur_y = y0;

    for (size_t i = 0; i < side_len; i++) {
        draw_pixel(SDL_round(cur_x), SDL_round(cur_y), color);
        cur_x += x_inc;
        cur_y += y_inc;
    }
}

void draw_grid(uint32_t width) {
    for (size_t y = 0; y < window_height; y++) {
        for (size_t x = 0; x < window_width; x++) {
            if (x % width == 0 || y % width == 0) {
                draw_pixel(x, y, 0xFF808080);
            }else{
                draw_pixel(x, y, 0xFF000000);
            }
        }
    }
}

void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    size_t height_start = y;
    size_t height_end = height_start + height;
    size_t width_start = x;
    size_t width_end = width_start + width;
    for (size_t y = height_start; y < height_end; y++) {
        for (size_t x = width_start; x < width_end; x++) {
            draw_pixel(x, y, color);
        }
    }
}

void draw_triangle(vector2_t vec1, vector2_t vec2, vector2_t vec3, uint32_t color) {
    draw_line(vec1.x, vec1.y, vec2.x, vec2.y, color);
    draw_line(vec2.x, vec2.y, vec3.x, vec3.y, color);
    draw_line(vec1.x, vec1.y, vec3.x, vec3.y, color);
}

void render_color_buffer(void) {
    SDL_UpdateTexture(texture, NULL, color_buffer, sizeof(uint32_t) * window_width);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
    for (size_t i = 0; i < (window_width * window_height); i++) {
        color_buffer[i] = color;
    }
}

void cleanup(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
}