/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    objops.c
    Object operations
 */

#include <objmgr.h>

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
          for (i = 0; i < obj->n; i++) {
              obj->xarray[i] += dx;
              obj->yarray[i] += dy;
          }
          break;
        case T_ELLIPSE:
          obj->x += dx;
          obj->y += dy;
          break;
        case T_CURVE:
          for (i = 0; i < obj->n; i++) {
              obj->xarray[i] += dx;
              obj->yarray[i] += dy;
          }
          break;
        default:
          printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in object translate\n");
          break;
    }
    objmgr_full_redraw();
}