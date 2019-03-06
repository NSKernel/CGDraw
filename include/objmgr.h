/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    objmgr.h
    Object manager structures
 */

#include <stdlib.h>

#include <grapheng.h>

#ifndef OBJMGR_H
#define OBJMGR_H

#define MAX_OBJS     1000

#define OBJMGR_OK    0x1
#define OBJMGR_ERROR 0x2

#define T_LINE       0x1
#define T_POLYGON    0x2
#define T_ELLIPSE    0x3
#define T_CURVE      0x4
#define T_ELLIPSE_R  0x5

typedef struct cgdraw_object_t {
    uint32_t id;
    uint8_t type;
    int n;
    union {
        float x;
        float x1;
    };
    union {
        float y;
        float y1;
    };
    union {
        float rx;
        float x2;
    };
    union {
        float ry;
        float y2;
    };
    float r;
    float *xarray;
    float *yarray;
    uint8_t algorithm;
    color_set color;
} cgdraw_object;

void objmgr_close();
int objmgr_init();
cgdraw_object *objmgr_get_object(int id);
int objmgr_commit_object(cgdraw_object *object);
int objmgr_full_render();

#endif