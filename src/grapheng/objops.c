/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    objops.c
    Object operations
 */

#include <math.h>
#include <stdlib.h>

#include <cgdraw.h>
#include <grapheng.h>
#include <objmgr.h>
#include <objops.h>

extern inline float _rotate_point_x(float x, float y, float xr, float yr, float r);
extern inline float _rotate_point_y(float x, float y, float xr, float yr, float r);

int obj_translate(int id, float dx, float dy) {
    int i;
    cgdraw_object *obj = objmgr_get_object(id);

    if (obj == NULL) {
        printf("cgdraw: \033[0;31merror\033[0m: object %d not found\n", id);
        return OBJOPS_ERROR;
    }

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
        case T_DIED:
          printf("cgdraw: \033[0;31merror\033[0m: this object was died due to clipping\n");
          return OBJOPS_ERROR;
        default:
          printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in object translate\n");
          return OBJOPS_ERROR;
    }
    return OBJOPS_OK;
}

int obj_rotate(int id, float x, float y, float r) {
    int i;
    float xbackup;  
    cgdraw_object *obj = objmgr_get_object(id);

    if (obj == NULL) {
        printf("cgdraw: \033[0;31merror\033[0m: object %d not found\n", id);
        return OBJOPS_ERROR;
    }

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
        case T_DIED:
          printf("cgdraw: \033[0;31merror\033[0m: this object was died due to clipping\n");
          return OBJOPS_ERROR;
        default:
          printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in rotate\n");
          return OBJOPS_ERROR;
    }
    return OBJOPS_OK;
}

int obj_scale(int id, float x, float y, float s) {
    int i;
    cgdraw_object *obj = objmgr_get_object(id);

    if (obj == NULL) {
        printf("cgdraw: \033[0;31merror\033[0m: object %d not found\n", id);
        return OBJOPS_ERROR;
    }

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
        case T_DIED:
          printf("cgdraw: \033[0;31merror\033[0m: this object was died due to clipping\n");
          return OBJOPS_ERROR;
        default:
          printf("cgdraw: \033[0;31minternel error\033[0m: unexpected object type in scale\n");
          return OBJOPS_ERROR;
    }
    return OBJOPS_OK;
}

#define CS_INSIDE    0x0 // 0000
#define CS_LEFT      0x1 // 0001
#define CS_RIGHT     0x2 // 0010
#define CS_BOTTOM    0x4 // 0100
#define CS_TOP       0x8 // 1000

uint8_t _obj_cs_outcode(float x, float y, float x1, float y1, float x2, float y2) {
    uint8_t outcode = 0;

    if (x < x1)
        outcode |= CS_LEFT;
    else if (x > x2)
        outcode |= CS_RIGHT;
    if (y < y1)
        outcode |= CS_BOTTOM;
    else if (y > y2)
        outcode |= CS_TOP;

    return outcode;
}

int obj_clip_cs(int id, float x1, float y1, float x2, float y2) {
    uint8_t outcode1 = 0;
    uint8_t outcode2 = 0;
    cgdraw_object *obj = objmgr_get_object(id);
    uint8_t accept = 0;
    uint8_t state = 0;
    uint8_t outcode_current;
    double xclipped, yclipped;

    double exchange;
    if (x1 > x2) {
        exchange = x2;
        x2 = x1;
        x1 = exchange;
    }
    if (y1 > y2) {
        exchange = y2;
        y2 = y1;
        y1 = exchange;
    }


    if (obj == NULL) {
        printf("cgdraw: \033[0;31merror\033[0m: object %d not found\n", id);
        return OBJOPS_ERROR;
    }

    if (obj->type != T_LINE) {
        printf("cgdraw: \033[0;31merror\033[0m: object is not a line thus not able to be clipped\n");
        return OBJOPS_ERROR;
    }

    outcode1 = _obj_cs_outcode(obj->x1, obj->y1, x1, y1, x2, y2);
    outcode2 = _obj_cs_outcode(obj->x2, obj->y2, x1, y1, x2, y2);
    
    while (1) {
        if ((outcode1 == CS_INSIDE) && (outcode2 == CS_INSIDE)) {
            accept = 1;
            break;
        }
        else if (outcode1 & outcode2) {
			      // bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			      // or BOTTOM), so both must be outside window; exit loop (accept is false)
			      break;
        }

        if (outcode1) { // first deal with x1, y1
            state = 0;
            xclipped = obj->x1;
            yclipped = obj->y1;
            outcode_current = outcode1;
        }
        else {
            state = 1;
            xclipped = obj->x2;
            yclipped = obj->y2;
            outcode_current = outcode2;
        }

        if (outcode_current & CS_TOP) {
            xclipped = obj->x1 + (obj->x2 - obj->x1) * (y2 - obj->y1) / (obj->y2 - obj->y1);
				    yclipped = y2;
        }
        else if (outcode_current & CS_BOTTOM) {
            xclipped = obj->x1 + (obj->x2 - obj->x1) * (y1 - obj->y1) / (obj->y2 - obj->y1);
            yclipped = y1;
        }
        else if (outcode_current & CS_RIGHT) {
            yclipped = obj->y1 + (obj->y2 - obj->y1) * (x2 - obj->x1) / (obj->x2 - obj->x1);
		        xclipped = x2;
        }
        else if (outcode_current & CS_LEFT) {
            yclipped = obj->y1 + (obj->y2 - obj->y1) * (x1 - obj->x1) / (obj->x2 - obj->x1);
		        xclipped = x1;
        }

        if (state == 0) {
            obj->x1 = xclipped;
            obj->y1 = yclipped;
            outcode1 = _obj_cs_outcode(xclipped, yclipped, x1, y1, x2, y2);
        }
        else {
            obj->x2 = xclipped;
            obj->y2 = yclipped;
            outcode2 = _obj_cs_outcode(xclipped, yclipped, x1, y1, x2, y2);
        }
    }

    if (!accept) {
        obj->type = T_DIED;
        printf("cgdraw: \033[0;33mwarning\033[0m: an object died due to clipping\n");
    }

    return OBJOPS_OK;
}

float _obj_clip_lb_find_maximum(float arr[],int n) {
    float m = 0;
    for (int i = 0; i < n; ++i)
        if (m < arr[i])
            m = arr[i];
    return m;
}

float _obj_clip_lb_find_minimum(float arr[], int n) {
    float m = 1;
    for (int i = 0; i < n; ++i)
        if (m > arr[i])
            m = arr[i];
    return m;
}

int obj_clip_lb(int id, float x1, float y1, float x2, float y2) {
    int i;
    cgdraw_object *obj = objmgr_get_object(id);
    float xn1, yn1, xn2, yn2;

    double exchange;
    if (x1 > x2) {
        exchange = x2;
        x2 = x1;
        x1 = exchange;
    }
    if (y1 > y2) {
        exchange = y2;
        y2 = y1;
        y1 = exchange;
    }

    char flag = 0;
    float u1 = 0, u2 =1;
    float p[4], q[4];
    float r;
    p[0] = obj->x1 - obj->x2;
    p[1] = -p[0];
    p[2] = obj->y1 - obj->y2;
    p[3] = -p[2];
 
    q[0] = obj->x1 - x1;
    q[1] = x2 - obj->x1;
    q[2] = obj->y1 - y1;
    q[3] = y2 - obj->y1;
     
    for (i = 0; i < 4; i++) {
        r = (float)q[i] / (float)p[i];
        if (p[i] < 0) {
            u1 = (u1 > r ? u1 : r);
            if(u1 > u2)
                flag = 1;
        }
        if (p[i] > 0) {
            u2 = (u2 < r ? u2 : r);
            if (u1 > u2)
                flag = 1;
        }
        if (p[i] == 0 && q[i] < 0) {
            flag = 1;
        }
    }
    
    if (flag) {
        obj->type = T_DIED;
        printf("cgdraw: \033[0;33mwarning\033[0m: an object died due to clipping\n");
        return OBJOPS_OK;
    }

    xn1 = obj->x1 - u1 *(obj->x1 - obj->x2);
    yn1 = obj->y1 - u1 *(obj->y1 - obj->y2);
    xn2 = obj->x1 - u2 *(obj->x1 - obj->x2);
    yn2 = obj->y1 - u2 *(obj->y1 - obj->y2); 
    obj->x1 = xn1;
    obj->y1 = yn1;
    obj->x2 = xn2;
    obj->y2 = yn2;

    return OBJOPS_OK;
}