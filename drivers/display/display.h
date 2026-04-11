#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    void (*init)(void);
    void (*set_window)(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void (*write_pixel_data)(uint8_t *data, uint32_t length);
} display_t;

#endif // DISPLAY_H