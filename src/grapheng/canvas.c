/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    canvas.c
    Manages canvas
 */

#include <stdio.h>

#include <grapheng.h>
#include <libbmp.h>

void init_canvas(int width, int height) {
    size_t x;
    size_t y;
    bmp_img_init_df(&current_canvas, width, height);

    // initialize a blank canvas
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            bmp_pixel_init(&current_canvas.img_pixels[y][x], 255, 255, 255);
        }
    }
}

void free_canvas() {
    bmp_img_free(&current_canvas);
}

void draw_pixel(int R, int G, int B, int x, int y) {
    if (current_canvas.img_header.biHeight - y < 0 || y >= current_canvas.img_header.biHeight ||
        current_canvas.img_header.biWidth - x < 0 || x >= current_canvas.img_header.biWidth){
        printf("cgdraw: \033[0;33mwarning\033[0m: you are drawing pixles outside the canvas which will be droped\n");
        return;
    }

    bmp_pixel_init(&current_canvas.img_pixels[current_canvas.img_header.biHeight - y][x], R, G, B);
}

int save_canvas(const char *path) {
    if (bmp_img_write(&current_canvas, path) != BMP_OK) {
        printf("cgdraw: failed to save current canvas to %s\n", path);
        return SAVE_ERROR;
    }
    return SAVE_OK;
}