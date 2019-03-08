/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    exec.h
    Inline helpers for execution engine
 */

#include <stdio.h>
#include <stdlib.h>

#include <cgdraw.h>
#include <grapheng.h>
#include <objmgr.h>

#ifndef EXEC_H
#define EXEC_H

inline int _exec_reset_canvas(int width, int height) 
{
    if (canvas_valid) {
        free_canvas();
    }
    init_canvas(width, height);
    canvas_valid = 1;
    return EXEC_OK;
};

inline int _exec_save_canvas(const char *path) 
{
    if (canvas_valid) {
        objmgr_full_render();
        if (save_canvas(path) != SAVE_OK) {
            printf("cgdraw: \033[0;31merror\033[0m: failed to save bmp to %s\n", path);
            return EXEC_ERROR;
        }
        printf("cgdraw: \033[0;32minfo\033[0m: successfully saved canvas to %s\n", path);
        return EXEC_OK;
    }
    printf("cgdraw: \033[0;31merror\033[0m: no canvas to be saved\n");
    return EXEC_ERROR;
};

inline int _exec_set_color(int R, int G, int B) 
{
    if (R > 255 || R < 0 ||
        G > 255 || G < 0 ||
        B > 255 || B < 0 ) {
        printf("cgdraw: \033[0;31merror\033[0m: R G B value should be in a range from 0 to 255\n");
        return EXEC_ERROR;
    }
    current_color.R = (uint8_t)R;
    current_color.G = (uint8_t)G;
    current_color.B = (uint8_t)B;
    return EXEC_OK;
};

inline int _exec_draw_line(uint32_t id, float x1, float y1, float x2, float y2, uint8_t algorithm) 
{
    cgdraw_object *object = NULL;
    if (canvas_valid) {
        if (algorithm != A_DDA && algorithm != A_BRESENHAM) {
            // should not be here
            printf("cgdraw: \033[0;31minternel error\033[0m: unexpected algorithm in draw line. THIS SHOULD NOT HAPPEN\n");
            return EXEC_ERROR;
        }
        object = malloc(sizeof(cgdraw_object));
        object->id = id;
        object->type = T_LINE;
        object->x1 = x1;
        object->y1 = y1;
        object->x2 = x2;
        object->y2 = y2;
        object->algorithm = algorithm;
        object->color.R = current_color.R;
        object->color.G = current_color.G;
        object->color.B = current_color.B;
        objmgr_commit_object(object);
        return EXEC_OK;
    }
    printf("cgdraw: \033[0;31merror\033[0m: no canvas to draw\n");
    return EXEC_ERROR;
};

int _exec_draw_polygon(uint32_t id, int n, uint8_t algorithm, float *xarray, float *yarray);

inline int _exec_draw_ellipse(uint32_t id, float x, float y, float rx, float ry) 
{
    cgdraw_object *object = NULL;
    if (canvas_valid) {
        object = malloc(sizeof(cgdraw_object));
        object->id = id;
        object->type = T_ELLIPSE;
        object->x = x;
        object->y = y;
        object->rx = rx;
        object->ry = ry;
        object->color.R = current_color.R;
        object->color.G = current_color.G;
        object->color.B = current_color.B;
        objmgr_commit_object(object);
        return EXEC_OK;
    }
    printf("cgdraw: \033[0;31merror\033[0m: no canvas to draw\n");
    return EXEC_ERROR;
};

inline int _exec_draw_curve(uint32_t id, int n, uint8_t algorithm, float *xarray, float *yarray) 
{
    cgdraw_object *object = NULL;
    if (canvas_valid) {
        if (algorithm != A_BEZIER && algorithm != A_B_SPLINE) {
            // should not be here
            printf("cgdraw: \033[0;31minternel error\033[0m: unexpected algorithm in draw curve. THIS SHOULD NOT HAPPEN\n");
            return EXEC_ERROR;
        }
        object = malloc(sizeof(cgdraw_object));
        object->type = T_CURVE;
        object->id = id;
        object->n = n;
        object->algorithm = algorithm;
        // xarray and yarray are not freed by free_isntr
        // thus we can use them here
        object->xarray = xarray;
        object->yarray = yarray;
        object->color.R = current_color.R;
        object->color.G = current_color.G;
        object->color.B = current_color.B;
        objmgr_commit_object(object);
        return EXEC_OK;
    }
    printf("cgdraw: \033[0;31merror\033[0m: no canvas to draw\n");
    return EXEC_ERROR;
};

#endif