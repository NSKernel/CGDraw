/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    objmgr.c
    Manages objects
 */

#include <stdio.h>
#include <stdlib.h>

#include <grapheng.h>
#include <instr.h>
#include <objmgr.h>

uint32_t obj_count = 0;
cgdraw_object **objects;

void objmgr_close() {
    while (obj_count--) {
        if (objects[obj_count]->type == T_POLYGON || objects[obj_count]->type == T_CURVE) {
            free(objects[obj_count]->xarray);
            free(objects[obj_count]->yarray);
        }
        free(objects[obj_count]);
    }
}

int objmgr_init() {
    objmgr_close();
    obj_count = 0;
    objects = malloc(sizeof(cgdraw_object*) * MAX_OBJS);
    if (objects != NULL)
        return OBJMGR_OK;
    return OBJMGR_ERROR;
}

int objmgr_commit_object(cgdraw_object *object) {
    int i = 0;
    if (obj_count < MAX_OBJS) {
        for (i = 0; i < obj_count; i++) {
            if (objects[i]->id == object->id) {
                printf("cgdraw: \033[0;31merror\033[0m: you are commiting an object that has the same id as what you have already commited. aborted.\n");
                return OBJMGR_ERROR;
            }
        }
        objects[obj_count++] = object;
        return OBJMGR_OK;
    }
    else {
        printf("cgdraw: \033[0;31merror\033[0m: you are commiting more objects than object manager can handle. aborted.\n");
        return OBJMGR_ERROR;
    }
}

int objmgr_full_redraw() {
    int i = 0;
    for (i = 0; i < obj_count; i++) {
        switch (objects[i]->type) {
            case T_LINE:
              if (objects[i]->algorithm == A_DDA)
                  superdraw_line_dda(objects[i]->x1, objects[i]->y1, objects[i]->x2, objects[i]->y2);
              else if (objects[i]->algorithm == A_BRESENHAM)
                  superdraw_line_bresenham(objects[i]->x1, objects[i]->y1, objects[i]->x2, objects[i]->y2);
              else
                  printf("cgdraw: \033[0;31minternel error\033[0m: unexpected algorithm in full redraw\n");
                  return OBJMGR_ERROR;
              break;
            default:
              printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in full redraw\n");
              return OBJMGR_ERROR;
        }
    }
    return OBJMGR_OK;
}