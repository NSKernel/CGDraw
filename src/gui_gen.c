/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    gui_gen.c
    Generates necessary files for CGDraw GUI
 */

#include <stdio.h>
#include <stdlib.h>

#include <cgdraw.h>
#include <exec.h>
#include <compiler.h>
#include <grapheng.h>
#include <objmgr.h>
#include <objops.h>

int gui_save_image(const char *target_path) 
{
    if (canvas_valid) {
        objmgr_full_render();
        if (save_canvas(target_path) != SAVE_OK) {
            if (global_args.no_color) {
                printf("cgdraw: error: failed to save bmp to %s\n", target_path);
            }
            else {
                printf("cgdraw: \033[0;31merror\033[0m: failed to save bmp to %s\n", target_path);
            }
            return EXEC_ERROR;
        }
        return EXEC_OK;
    }
    if (global_args.no_color) {
        printf("cgdraw: error: no canvas to be saved\n");
    }
    else {
        printf("cgdraw: \033[0;31merror\033[0m: no canvas to be saved\n");
    }
    return EXEC_ERROR;
};

int gui_save_object(const char *target_path) {
    FILE *target_file = fopen(target_path, "w+");
    if (target_file == NULL) {
        if (global_args.no_color) {
            printf("cgdraw: error: failed to write csv\n");
        }
        else {
            printf("cgdraw: \033[0;31merror\033[0m: failed to write csv\n");
        }
        
        return EXEC_ERROR;
    }
    objmgr_dump_to_csv(target_file);
    fclose(target_file);
    return EXEC_OK;
}

int gui_save(const char *target_path) {
    // gen target bmp path and target csv path
    char *target_bmp_path = malloc((strlen(target_path) + 5) * sizeof(char));
    char *target_csv_path = malloc((strlen(target_path) + 5) * sizeof(char));
    strncpy(target_bmp_path, target_path, strlen(target_path));
    strncpy(target_bmp_path + strlen(target_path), ".bmp", 5);
    target_bmp_path[(strlen(target_path) + 4)] = 0;
    strncpy(target_csv_path, target_path, strlen(target_path));
    strncpy(target_csv_path + strlen(target_path), ".csv", 5);
    target_csv_path[(strlen(target_path) + 4)] = 0;

    if (gui_save_image(target_bmp_path) != EXEC_OK) {
        return EXEC_ERROR;
    }
    if (gui_save_object(target_csv_path) != EXEC_OK) {
        return EXEC_ERROR;
    }
    return EXEC_OK;
}
