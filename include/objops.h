/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    objops.h
    Object operations
 */

#ifndef OBJOPS_H
#define OBJOPS_H

#define OBJOPS_OK    0x0
#define OBJOPS_ERROR 0x1

int obj_translate(int id, float dx, float dy);
int obj_rotate(int id, float x, float y, float r);
int obj_scale(int id, float x, float y, float s);
int obj_clip_cs(int id, float x1, float y1, float x2, float y2);
int obj_clip_lb(int id, float x1, float y1, float x2, float y2);

#endif