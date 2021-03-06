/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    objmgr.c
    Manages objects
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <exec.h>
#include <grapheng.h>
#include <instr.h>
#include <objmgr.h>

uint32_t obj_count = 0;
cgdraw_object **objects = NULL;

void objmgr_close() {
    while (obj_count--) {
        if (objects[obj_count]->type == T_POLYGON || objects[obj_count]->type == T_CURVE) {
            free(objects[obj_count]->xarray);
            free(objects[obj_count]->yarray);
        }
        free(objects[obj_count]);
    }
    if (objects != NULL) {
        free(objects);
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

cgdraw_object *objmgr_get_object(int id) {
    int i;
    for (i = 0; i < obj_count; i++) {
        if (objects[i]->id == id)
            return objects[i];
    }
    return NULL;
}

int objmgr_full_render() {
    int i, j;
    int R_backup, G_backup, B_backup;
    int width, height;

    // clear canvas
    width = current_canvas.img_header.biWidth;
    height = current_canvas.img_header.biHeight;
    free_canvas(&current_canvas);
    init_canvas(width, height);
    
    // backup current color
    R_backup = current_color.R;
    G_backup = current_color.G;
    B_backup = current_color.B;

    // redraw all objects
    for (i = 0; i < obj_count; i++) {
        // set color
        current_color.R = objects[i]->color.R;
        current_color.G = objects[i]->color.G;
        current_color.B = objects[i]->color.B;
        switch (objects[i]->type) {
            case T_LINE:
              if (objects[i]->algorithm == A_DDA)
                  superdraw_line_dda(objects[i]->x1, objects[i]->y1, objects[i]->x2, objects[i]->y2);
              else if (objects[i]->algorithm == A_BRESENHAM)
                  superdraw_line_bresenham(objects[i]->x1, objects[i]->y1, objects[i]->x2, objects[i]->y2);
              else {
                  printf("cgdraw: \033[0;31minternel error\033[0m: unexpected algorithm in full redraw. type = LINE, objit = %d, algorithm = %d\n", i, objects[i]->algorithm);
                  return OBJMGR_ERROR;
              }
              break;
            case T_POLYGON:
              if (objects[i]->algorithm == A_DDA) {
                  for (j = 0; j < objects[i]->n - 1; j++) {
                      superdraw_line_dda(objects[i]->xarray[j], objects[i]->yarray[j], objects[i]->xarray[j + 1], objects[i]->yarray[j + 1]);
                  }
                  superdraw_line_dda(objects[i]->xarray[objects[i]->n - 1], objects[i]->yarray[objects[i]->n - 1], objects[i]->xarray[0], objects[i]->yarray[0]);
              }
              else if (objects[i]->algorithm == A_BRESENHAM) {
                  for (j = 0; j < objects[i]->n - 1; j++) {
                    superdraw_line_bresenham(objects[i]->xarray[j], objects[i]->yarray[j], objects[i]->xarray[j + 1], objects[i]->yarray[j + 1]);
                  }
                  superdraw_line_bresenham(objects[i]->xarray[objects[i]->n - 1], objects[i]->yarray[objects[i]->n - 1], objects[i]->xarray[0], objects[i]->yarray[0]);
              }
              else {
                  printf("cgdraw: \033[0;31minternel error\033[0m: unexpected algorithm in full redraw. type = POLYGON, objit = %d, algorithm = %d\n", i, objects[i]->algorithm);
                  return OBJMGR_ERROR;
              }
              break;
            case T_ELLIPSE:
              superdraw_ellipse(objects[i]->x, objects[i]->y, objects[i]->rx, objects[i]->ry);
              break;
            case T_CURVE:
              if (objects[i]->algorithm == A_BEZIER)
                  superdraw_curve_bezier(objects[i]->n, objects[i]->xarray, objects[i]->yarray);
              else if (objects[i]->algorithm == A_B_SPLINE)
                  superdraw_curve_b_spline(objects[i]->n, objects[i]->xarray, objects[i]->yarray);
              else {
                  printf("cgdraw: \033[0;31minternel error\033[0m: unexpected algorithm in full redraw. type = CURVE, objit = %d, algorithm = %d\n", i, objects[i]->algorithm);
                  return OBJMGR_ERROR;
              }
              break;
            case T_ELLIPSE_R:
              superdraw_ellipse_rotate(objects[i]->x, objects[i]->y, objects[i]->rx, objects[i]->ry, objects[i]->r);
              break;
            case T_DIED:
              break;
            default:
              printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in full redraw. objit = %d, objcount = %d\n", i, obj_count);
              return OBJMGR_ERROR;
        }
    }

    // roll back colors
    current_color.R = R_backup;
    current_color.G = G_backup;
    current_color.B = B_backup;
    return OBJMGR_OK;
}

void objmgr_dump_to_csv(FILE *target_file) {
    int i, j;
    float xmin = INT_MAX;
    float xmax = -INT_MAX;
    float ymin = INT_MAX;
    float ymax = -INT_MAX;
    for (i = 0; i < obj_count; i++) {
        switch(objects[i]->type) {
            case T_LINE:
                xmin = (objects[i]->x1 > objects[i]->x2 ? objects[i]->x2 : objects[i]->x1);
                xmax = (objects[i]->x1 > objects[i]->x2 ? objects[i]->x1 : objects[i]->x2);
                ymin = (objects[i]->y1 > objects[i]->y2 ? objects[i]->y2 : objects[i]->y1);
                ymax = (objects[i]->y1 > objects[i]->y2 ? objects[i]->y1 : objects[i]->y2);
                fprintf(target_file, "%d,%d,%f,%f,%f,%f\n", objects[i]->id, objects[i]->type, xmin, xmax, ymin, ymax);
                break;
            case T_CURVE: 
            case T_POLYGON:
                for (j = 0; j < objects[i]->n; j++) {
                    if (objects[i]->xarray[j] > xmax) {
                        xmax = objects[i]->xarray[j];
                    }
                    if (objects[i]->xarray[j] < xmin) {
                        xmin = objects[i]->xarray[j];
                    }
                    if (objects[i]->yarray[j] > ymax) {
                        ymax = objects[i]->yarray[j];
                    }
                    if (objects[i]->yarray[j] < ymin) {
                        ymin = objects[i]->yarray[j];
                    }
                }
                fprintf(target_file, "%d,%d,%f,%f,%f,%f\n", objects[i]->id, objects[i]->type, xmin, xmax, ymin, ymax);
                break;
            case T_ELLIPSE:
            case T_ELLIPSE_R:
                fprintf(target_file, "%d,%d,0,0,0,0\n", objects[i]->id, objects[i]->type);
                break;
        }
    }
}