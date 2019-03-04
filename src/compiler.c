/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    compiler.c
    Compiles instruction files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cgdraw.h>
#include <instr.h>
#include <compiler.h>

/*
    _parse_draw_polygon has a loop with a local variable i
    thus shall not be inlined.
 */
struct instruction *_parse_draw_polygon() {
    struct instruction *instr = malloc(sizeof(struct instruction));
    char algorithm[100];
    uint32_t i;

    instr->opcode = I_DRAW_POLYGON;
    fscanf(instr_file_pointer, "%d %d %s", &(instr->id), &(instr->n), algorithm);
    if (!strcmp(algorithm, "DDA")) {
        instr->algorithm = A_DDA;
    }
    else if (!strcmp(algorithm, "Bresenham")) {
        instr->algorithm = A_BRESENHAM;
    }
    else {
        printf("cgdraw: unexpected algorithm \"%s\" for instruction drawPolygon. DDA or Bresenham expected\n", algorithm);
        return (void *)C_ERROR;
    }

    // build up the xarray and yarray
    instr->xarray = malloc(sizeof(float) * instr->n);
    instr->yarray = malloc(sizeof(float) * instr->n);

    i = 0;
    while (!feof(instr_file_pointer) && i < instr->n) {
        fscanf(instr_file_pointer, "%f", &(instr->xarray[i]));
        fscanf(instr_file_pointer, "%f", &(instr->yarray[i]));
        i += 1;
    }

    if (i != instr->n) {
        // less than expected amount of points read
        // shall return error
        return (void *)C_ERROR;
    }

    return instr;
}

/*
    _parse_draw_curve has a loop with a local variable i
    thus shall not be inlined.
 */
struct instruction *_parse_draw_curve() {
    struct instruction *instr = malloc(sizeof(struct instruction));
    char algorithm[100];
    uint32_t i;

    instr->opcode = I_DRAW_CURVE;
    fscanf(instr_file_pointer, "%d %d %s", &(instr->id), &(instr->n), algorithm);
    if (!strcmp(algorithm, "Bezier")) {
        instr->algorithm = A_BEZIER;
    }
    else if (!strcmp(algorithm, "B-spline")) {
        instr->algorithm = A_B_SPLINE;
    }
    else {
        printf("cgdraw: unexpected algorithm \"%s\" for instruction drawCurve. Bezier or B-spline expected\n", algorithm);
        return (void *)C_ERROR;
    }

    // build up the xarray and yarray
    instr->xarray = malloc(sizeof(float) * instr->n);
    instr->yarray = malloc(sizeof(float) * instr->n);

    i = 0;
    while (!feof(instr_file_pointer) && i < instr->n) {
        fscanf(instr_file_pointer, "%f", &(instr->xarray[i]));
        fscanf(instr_file_pointer, "%f", &(instr->yarray[i]));
        i += 1;
    }

    if (i != instr->n) {
        // less than expected amount of points read
        // shall return error
        return (void *)C_ERROR;
    }

    return instr;
}

/*
    get_next_instruct() will read and parse the next instruction
    from the instruction file.
    YOU MUST FREE THE INSTRUCTION AFTER USE. PARSER WILL NOT
    CARE ABOUT THE ALLOCATED INSTRUCTION.
 */
struct instruction *get_next_instruction() {
    char opcode[100];
    
    // scan for opcode
    if (fscanf(instr_file_pointer, "%s", opcode) <= 0) {
        // meets EOF
        return (void *)C_DONE;
    }
    
    if (!strcmp(opcode, "resetCanvas")) 
        return _parse_reset_canvas();
    if (!strcmp(opcode, "saveCanvas")) 
        return _parse_save_canvas();
    if (!strcmp(opcode, "setColor")) 
        return _parse_set_color();
    if (!strcmp(opcode, "drawLine")) 
        return _parse_draw_line();
    if (!strcmp(opcode, "drawPolygon")) 
        return _parse_draw_polygon();
    if (!strcmp(opcode, "drawEllipse")) 
        return _parse_draw_ellipse();
    if (!strcmp(opcode, "drawCurve")) 
        return _parse_draw_curve();
    if (!strcmp(opcode, "translate")) 
        return _parse_translate();
    if (!strcmp(opcode, "rotate")) 
        return _parse_rotate();
    if (!strcmp(opcode, "scale")) 
        return _parse_scale();
    if (!strcmp(opcode, "clip")) 
        return _parse_clip();

    printf("cgdraw: Unexpected instruction %s\n", opcode);
    return NULL;
}

void free_instr(struct instruction *instr) {
    switch(instr->opcode) {
        case I_DRAW_POLYGON:
        case I_DRAW_CURVE:
            // DO NOT FREE XARRAY AND YARRAY
            // BECAUSE THEY ARE DIRECTLY
            // USED BY OBJMGR TO SAVE INFORMATION
            
            //free(instr->xarray);
            //free(instr->yarray);
            free(instr);
            break;
        case I_SAVE_CANVAS:
            free(instr->name);
        case I_RESET_CANVAS:
        case I_SET_COLOR:
        case I_DRAW_LINE:
        case I_DRAW_ELLIPSE:
        case I_TRANSLATE:
        case I_ROTATE:
        case I_SCALE:
        case I_CLIP:
          free(instr);
        default:
          break;
    }
}