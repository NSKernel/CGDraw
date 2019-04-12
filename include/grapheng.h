/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    grapheng.h
    Core graph engine
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <libbmp.h>

#ifndef GRAPHENG_H
#define GRAPHENG_H

typedef struct color_set_t {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} color_set;

#define SAVE_OK     0x0
#define SAVE_ERROR  0x1

char canvas_valid;
bmp_img current_canvas;
color_set current_color;

void init_canvas(int width, int height);
void free_canvas();
void draw_pixel(int R, int G, int B, int x, int y);
int save_canvas(const char *path);

void superdraw_line_dda(float x1, float y1, float x2, float y2);
void superdraw_line_bresenham(float x1, float y1, float x2, float y2);
void superdraw_ellipse(float x, float y, float rx, float ry);
void superdraw_curve_bezier(int n, float *xarray, float *yarray);
void superdraw_curve_b_spline(int n, float *xarray, float *yarray);
void superdraw_ellipse_rotate(float x, float y, float rx, float ry, float r);

static inline float _rotate_point_x(float x, float y, float xr, float yr, float r) 
{
    return (xr + (x - xr) * cos(M_PI * 2 * r / 360.0) - (y - yr) * sin(M_PI * 2 *  r / 360.0));
};

static inline float _rotate_point_y(float x, float y, float xr, float yr, float r) 
{
    return (yr + (x - xr) * sin(M_PI * 2 * r / 360.0) + (y - yr) * cos(M_PI * 2 * r / 360.0));
};

#endif