/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    cgdraw.h
    General global variables and functions
 */

#include <stdio.h>

#include <instr.h>

#ifndef CGDRAW_H
#define CGDRAW_H

struct global_args_t {
    char print_version;          /* -V or --version */
    char verbose;                /* -v or --verbose */
    char gui_enabled;            /* -g or --gui */
    char supress_warning;        /* -s or --supress-warning */
    char no_color;               /* -n or --no-color */
    char *instr_file_path;
    char *dest_dir;
    char *gui_file_path;
    FILE *debug_file;
    
} global_args;

FILE *instr_file_pointer;

#define EXEC_OK      0x01
#define EXEC_DONE    0x02
#define EXEC_ERROR   0x03
int exec_next_instr();

#endif