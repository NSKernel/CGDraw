/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    exec.c
    Executor
 */

#include <stdio.h>
#include <stdlib.h>

#include <cgdraw.h>
#include <exec.h>
#include <compiler.h>
#include <grapheng.h>
#include <objmgr.h>

char *algorithm_friendly_name[7] = {"Naive", "DDA", "Bresenham", "Bezier", "B-spline", "Cohen-Sutherland", "Liang-Barsky"};

#ifdef DEBUG

void _print_instruction(struct instruction *instr) {
    int i;
    switch (instr->opcode) {
        case I_RESET_CANVAS:
          printf("resetCanvas width=%d height=%d\n", instr->width, instr->height);
          break;
        case I_SAVE_CANVAS:
          printf("saveCanvas name=%s\n", instr->name);
          break;
        case I_SET_COLOR:
          printf("setColor R=%d G=%d B=%d\n", instr->R, instr->G, instr->B);
          break;
        case I_DRAW_LINE:
          printf("drawLine id=%d x1=%lf y1=%lf x2=%lf y2=%lf algorithm=%s\n", instr->id, instr->x1, instr->y1, instr->x2, instr->y2, algorithm_friendly_name[instr->algorithm]);
          break;
        case I_DRAW_POLYGON:
          printf("drawPolygon id=%d n=%d algorithm=%s ", instr->id, instr->n, algorithm_friendly_name[instr->algorithm]);
          for (i = 0; i < instr->n; i++) {
              printf("x%d=%lf y%d=%lf ", i, instr->xarray[i], i, instr->yarray[i]);
          }
          printf("\n");
          break;
        case I_DRAW_ELLIPSE:
          printf("drawEllipse id=%d x=%lf y=%lf rx=%lf ry=%lf\n", instr->id, instr->x, instr->y, instr->rx, instr->ry);
          break;
        case I_DRAW_CURVE:
          printf("drawCurve id=%d n=%d algorithm=%s ", instr->id, instr->n, algorithm_friendly_name[instr->algorithm]);
          for (i = 0; i < instr->n; i++) {
              printf("x%d=%lf y%d=%lf ", i, instr->xarray[i], i, instr->yarray[i]);
          }
          printf("\n");
          break;
        case I_TRANSLATE:
          printf("translate id=%d dx=%lf dy=%lf\n", instr->id, instr->dx, instr->dy);
          break;
        case I_ROTATE:
          printf("rotate id=%d x=%lf y=%lf r=%lf\n", instr->id, instr->x, instr->y, instr->r);
          break;
        case I_SCALE:
          printf("scale id=%d x=%lf y=%lf s=%lf\n", instr->id, instr->x, instr->y, instr->s);
          break;
        case I_CLIP:
          printf("clip id=%d x1=%lf y1=%lf x2=%lf y2=%lf algorithm=%s\n", instr->id, instr->x1, instr->y1, instr->x2, instr->y2, algorithm_friendly_name[instr->algorithm]);
          break;
        default:
          break;
    }
}

#endif

int _exec_draw_polygon(uint32_t id, int n, uint8_t algorithm, float *xarray, float *yarray) {
    cgdraw_object *object = NULL;
    int i;
    if (canvas_valid) {
        object = malloc(sizeof(cgdraw_object));
        object->type = T_POLYGON;
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
        if (algorithm == A_DDA) {
            for (i = 0; i < n - 1; i++) {
                superdraw_line_dda(xarray[i], yarray[i], xarray[i + 1], yarray[i + 1]);
            }
            superdraw_line_dda(xarray[n - 1], yarray[n - 1], xarray[0], yarray[0]);
        }
        else if (algorithm == A_BRESENHAM) {
            for (i = 0; i < n - 1; i++) {
                superdraw_line_bresenham(xarray[i], yarray[i], xarray[i + 1], yarray[i + 1]);
            }
            superdraw_line_bresenham(xarray[n - 1], yarray[n - 1], xarray[0], yarray[0]);
        }
        else {
            // should not be here
            printf("cgdraw: \033[0;31minternel error\033[0m: unexpected algorithm in draw line. THIS SHOULD NOT HAPPEN\n");
            return EXEC_ERROR;
        }
        return EXEC_OK;
    }
    printf("cgdraw: \033[0;31merror\033[0m: no canvas to draw\n");
    return EXEC_ERROR;
}

int exec_next_instr() {
    struct instruction *instr = get_next_instruction();
    int return_code = EXEC_ERROR;

    if (instr == (void *)C_ERROR) {
        return EXEC_ERROR;
    }
    if (instr == (void *)C_DONE) {
        return EXEC_DONE;
    }

#ifdef DEBUG
    _print_instruction(instr);
#endif

    switch (instr->opcode) {
        case I_RESET_CANVAS:
          // reinit object manager
          objmgr_init();
          return_code = _exec_reset_canvas(instr->width, instr->height);
          break;
        case I_SAVE_CANVAS:
          return_code = _exec_save_canvas(instr->name);
          break;
        case I_SET_COLOR:
          return_code = _exec_set_color(instr->R, instr->G, instr->B);
          break;
        case I_DRAW_LINE:
          return_code = _exec_draw_line(instr->id, instr->x1, instr->y1, instr->x2, instr->y2, instr->algorithm);
          break;
        case I_DRAW_POLYGON:
          return_code = _exec_draw_polygon(instr->id, instr->n, instr->algorithm, instr->xarray, instr->yarray);
          break;
        case I_DRAW_ELLIPSE:
          return_code = _exec_draw_ellipse(instr->id, instr->x, instr->y, instr->rx, instr->ry);
          break;
        case I_DRAW_CURVE:
          printf("cgdraw: unimplemented\n");
          break;
        case I_TRANSLATE:
          printf("cgdraw: unimplemented\n");
          break;
        case I_ROTATE:
          printf("cgdraw: unimplemented\n");
          break;
        case I_SCALE:
          printf("cgdraw: unimplemented\n");

          break;
        case I_CLIP:
          printf("cgdraw: unimplemented\n");
          break;
        default:
          // you should not be here
          printf("cgdraw: \033[0;31minternel error\033[0m: unexpected opcode. THIS SHOULD NOT HAPPEN.\n");
          return_code = EXEC_ERROR;
          break;
    }

    free_instr(instr);
    return return_code;
}