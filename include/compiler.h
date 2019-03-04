/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    compiler.h
    Compiler's helpers and golbal variables
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgdraw.h"

#ifndef COMPILER_H
#define COMPILER_H

#define MAX_PATH_LENGTH    1000

#define C_ERROR            0x00
#define C_DONE             0x01
struct instruction *get_next_instruction();
void free_instr();

inline struct instruction *_parse_reset_canvas() 
{
    struct instruction *instr = malloc(sizeof(struct instruction));

    instr->opcode = I_RESET_CANVAS;
    fscanf(instr_file_pointer, "%d %d", &(instr->width), &(instr->height));
    return instr;
};

inline struct instruction *_parse_save_canvas() {
    struct instruction *instr = malloc(sizeof(struct instruction));

    instr->opcode = I_SAVE_CANVAS;
    instr->name = malloc(sizeof(int) * MAX_PATH_LENGTH);
    fscanf(instr_file_pointer, "%s", instr->name);
    return instr;
};

inline struct instruction *_parse_set_color() 
{
    struct instruction *instr = malloc(sizeof(struct instruction));

    instr->opcode = I_SET_COLOR;
    fscanf(instr_file_pointer, "%d %d %d", &(instr->R), &(instr->G), &(instr->B));
    return instr;
};

inline struct instruction *_parse_draw_line() 
{
    struct instruction *instr = malloc(sizeof(struct instruction));
    char algorithm[100];

    instr->opcode = I_DRAW_LINE;
    fscanf(instr_file_pointer, "%d %f %f %f %f %s", &(instr->id), &(instr->x1), &(instr->y1), &(instr->x2), &(instr->y2), algorithm);
    if (!strcmp(algorithm, "DDA")) {
        instr->algorithm = A_DDA;
    }
    else if (!strcmp(algorithm, "Bresenham")) {
        instr->algorithm = A_BRESENHAM;
    }
    else {
        printf("cgdraw: unexpected algorithm \"%s\" for instruction drawLine. DDA or Bresenham expected\n", algorithm);
        return (void *)C_ERROR;
    }
    return instr;
};

/*
    parse_draw_polygon has a loop with a local variable i
    thus shall not be inlined.
 */
struct instruction *_parse_draw_polygon();

inline struct instruction *_parse_draw_ellipse() 
{
    struct instruction *instr = malloc(sizeof(struct instruction));

    instr->opcode = I_DRAW_ELLIPSE;
    fscanf(instr_file_pointer, "%d %f %f %f %f", &(instr->id), &(instr->x), &(instr->y), &(instr->rx), &(instr->ry));
    return instr;
};

/*
    parse_draw_curve has a loop with a local variable i
    thus shall not be inlined.
 */
struct instruction *_parse_draw_curve();

inline struct instruction *_parse_translate() 
{
    struct instruction *instr = malloc(sizeof(struct instruction));

    instr->opcode = I_TRANSLATE;
    fscanf(instr_file_pointer, "%d %f %f", &(instr->id), &(instr->dx), &(instr->dy));
    return instr;
};

inline struct instruction *_parse_rotate() 
{
    struct instruction *instr = malloc(sizeof(struct instruction));

    instr->opcode = I_ROTATE;
    fscanf(instr_file_pointer, "%d %f %f %f", &(instr->id), &(instr->x), &(instr->y), &(instr->r));
    return instr;
};

inline struct instruction *_parse_scale() 
{
    struct instruction *instr = malloc(sizeof(struct instruction));

    instr->opcode = I_SCALE;
    fscanf(instr_file_pointer, "%d %f %f %f", &(instr->id), &(instr->x), &(instr->y), &(instr->s));
    return instr;
};

inline struct instruction *_parse_clip() 
{
    struct instruction *instr = malloc(sizeof(struct instruction));
    char algorithm[100];

    instr->opcode = I_CLIP;
    fscanf(instr_file_pointer, "%d %f %f %f %f %s", &(instr->id), &(instr->x1), &(instr->y1), &(instr->x2), &(instr->y2), algorithm);
    if (!strcmp(algorithm, "Cohen-Sutherland")) {
        instr->algorithm = A_COHEN_SUTHERLAND;
    }
    else if (!strcmp(algorithm, "Liang-Barsky")) {
        instr->algorithm = A_LIANG_BARSKY;
    }
    else {
        printf("cgdraw: unexpected algorithm \"%s\" for instruction parseClip. Cohen-Sutherland or Liang-Barsky expected\n", algorithm);
        return (void *)C_ERROR;
    }

    return instr;
};

#endif