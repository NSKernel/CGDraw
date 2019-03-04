/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    instr.h
    Instruction structures
 */

#include <stdint.h>

#ifndef INSTR_H
#define INSTR_H

// instruction opcodes
#define I_RESET_CANVAS     0x01
#define I_SAVE_CANVAS      0x02
#define I_SET_COLOR        0x03
#define I_DRAW_LINE        0x04
#define I_DRAW_POLYGON     0x05
#define I_DRAW_ELLIPSE     0x06
#define I_DRAW_CURVE       0x07
#define I_TRANSLATE        0x08
#define I_ROTATE           0x09
#define I_SCALE            0x0A
#define I_CLIP             0x0B

// algorithms
#define A_NAIVE            0x00
#define A_DDA              0x01
#define A_BRESENHAM        0x02
#define A_BEZIER           0x03
#define A_B_SPLINE         0x04
#define A_COHEN_SUTHERLAND 0x05
#define A_LIANG_BARSKY     0x06

struct instruction {
    uint32_t opcode;
    union {
        int width;
        int R;
        int id;
    };
    union {
        int height;
        int G;
        int n;
    };
    union {
        int B;
    };
    union {
        char *name;
    };
    uint8_t algorithm;
    float *xarray;
    float *yarray;
    union {
        float x;
        float dx;
        float x1;
    };
    union {
        float y;
        float dy;
        float y1;
    };
    union {
        float rx;
        float r;
        float s;
        float x2;
    };
    union {
        float ry;
        float y2;
    };
};

#endif