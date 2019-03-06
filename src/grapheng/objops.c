/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    objops.c
    Object operations
 */

#include <math.h>

#include <grapheng.h>
#include <objmgr.h>

extern inline float _rotate_point_x(float x, float y, float xr, float yr, float r);
extern inline float _rotate_point_y(float x, float y, float xr, float yr, float r);

void obj_translate(int id, float dx, float dy) {
    int i;
    cgdraw_object *obj = objmgr_get_object(id);
    switch (obj->type) {
        case T_LINE:
          obj->x1 += dx;
          obj->y1 += dy;
          obj->x2 += dx;
          obj->y2 += dy;
          break;
        case T_POLYGON:
        case T_CURVE:
          for (i = 0; i < obj->n; i++) {
              obj->xarray[i] += dx;
              obj->yarray[i] += dy;
          }
          break;
        case T_ELLIPSE:
        case T_ELLIPSE_R:
          obj->x += dx;
          obj->y += dy;
          break;
        default:
          printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in object translate\n");
          break;
    }
}

void obj_rotate(int id, float x, float y, float r) {
    int i;
    float xbackup;
    
    cgdraw_object *obj = objmgr_get_object(id);
    switch (obj->type) {
        case T_LINE:
          xbackup = obj->x1;
          obj->x1 = _rotate_point_x(obj->x1, obj->y1, x, y, r);
          obj->y1 = _rotate_point_y(xbackup, obj->y1, x, y, r);
          xbackup = obj->x2;
          obj->x2 = _rotate_point_x(obj->x2, obj->y2, x, y, r);
          obj->y2 = _rotate_point_y(xbackup, obj->y2, x, y, r);
          break;
        case T_POLYGON:
        case T_CURVE:
          for (i = 0; i < obj->n; i++) {
              xbackup = obj->xarray[i];
              obj->xarray[i] = _rotate_point_x(obj->xarray[i], obj->yarray[i], x, y, r);
              obj->yarray[i] = _rotate_point_y(xbackup, obj->yarray[i], x, y, r);
          }
          break;
        case T_ELLIPSE:
          obj->type = T_ELLIPSE_R;
          xbackup = obj->x;
          obj->x = _rotate_point_x(obj->x, obj->y, x, y, r);
          obj->y = _rotate_point_y(xbackup, obj->y, x, y, r);
          obj->r = r;
          break;
        case T_ELLIPSE_R:
          obj->type = T_ELLIPSE_R;
          xbackup = obj->x;
          obj->x = _rotate_point_x(obj->x, obj->y, x, y, r);
          obj->y = _rotate_point_y(xbackup, obj->y, x, y, r);
          obj->r += r;
          break;
        default:
          printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in rotate\n");
          break;
    }
}

void obj_scale(int id, float x, float y, float s) {
    int i;

    cgdraw_object *obj = objmgr_get_object(id);
    switch (obj->type) {
        case T_LINE:
          obj->x1 = obj->x1 * s + x * (1 - s);
          obj->y1 = obj->y1 * s + y * (1 - s);
          obj->x2 = obj->x2 * s + x * (1 - s);
          obj->y2 = obj->y2 * s + y * (1 - s);
          break;
        case T_POLYGON:
        case T_CURVE:
          for (i = 0; i < obj->n; i++) {
              obj->xarray[i] = obj->xarray[i] * s + x * (1 - s);
              obj->yarray[i] = obj->yarray[i] * s + y * (1 - s);
          }
          break;
        case T_ELLIPSE:
        case T_ELLIPSE_R:
          obj->x = obj->x * s + x * (1 - s);
          obj->y = obj->y * s + y * (1 - s);
          obj->rx *= s;
          obj->ry *= s;
          break;
        default:
          printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in scale\n");
          break;
    }
}