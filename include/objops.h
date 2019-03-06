/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    objops.h
    Object operations
 */

#ifndef OBJOPS_H
#define OBJOPS_H

void obj_translate(int id, float dx, float dy);
void obj_rotate(int id, float x, float y, float r);
void obj_scale(int id, float x, float y, float s);

#endif