/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    superdraw.c
    Randerer
 */

#include <math.h>
#include <stdlib.h>

#include <grapheng.h>
#include <exec.h>

#define BEZIER_STEP   0.001

void superdraw_line_dda(float x1, float y1, float x2, float y2) {
    int i, start, end;
    float dx, dy, epsl;
    float x, y, xIncre, yIncre;

    if ((int)(x1 + 0.5) == (int)(x2 + 0.5)) {
        start = (((int)(y1 + 0.5) < (int)(y2 + 0.5)) ? ((int)(y1 + 0.5)) : ((int)(y2 + 0.5)));
        end = (((int)(y1 + 0.5) > (int)(y2 + 0.5)) ? ((int)(y1 + 0.5)) : ((int)(y2 + 0.5)));
        for (i = start; i <= end; i++) {
            draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x1 + 0.5), i);
        }
    }

    dx = x2 - x1;
    dy = y2 - y1;
    x = x1;
    y = y1;

    if (fabs(dx) > fabs(dy)) 
        epsl = fabs(dx);
    else 
        epsl = fabs(dy);
    xIncre = (float)dx / (float)epsl;
    yIncre = (float)dy / (float)epsl;
    for (i = 0; i <= epsl; i++) {
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x + 0.5), (int)(y + 0.5));
        x += xIncre;
        y += yIncre;
    }
}

void superdraw_line_bresenham(float x1, float y1, float x2, float y2) {
    char steep = fabs(y2 - y1) > fabs (x2 - x1);
    float temp;
    float deltax, deltay, error, deltaerr;
    int x, y, ystep, i, start, end;

    if ((int)(x1 + 0.5) == (int)(x2 + 0.5)) {
        start = (((int)(y1 + 0.5) < (int)(y2 + 0.5)) ? ((int)(y1 + 0.5)) : ((int)(y2 + 0.5)));
        end = (((int)(y1 + 0.5) > (int)(y2 + 0.5)) ? ((int)(y1 + 0.5)) : ((int)(y2 + 0.5)));
        for (i = start; i <= end; i++) {
            draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x1 + 0.5), i);
        }
    }

    if (steep) {
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = x2;
        x2 = y2;
        y2 = temp;
    }
    if (x1 > x2) {
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    deltax = x2 - x1;
    deltay = y2 - y1;
    error = 0;
    deltaerr = deltay / deltax;
    y = (int)(y1 + 0.5);
    if (y1 < y2)
        ystep = 1;
    else
        ystep = -1;

    for (x = (int)(x1 + 0.5); x < (int)(x2 + 0.5); x++) {
        if (steep)
            draw_pixel(current_color.R, current_color.G, current_color.B, y, x);
        else
            draw_pixel(current_color.R, current_color.G, current_color.B, x, y);
        error += deltaerr;
        if (error >= 0.5) {
            y += ystep;
            error -= 1;
        }
    }
}

void superdraw_ellipse(float x, float y, float rx, float ry) {
    int xi, yi;
    float d1, d2, dx, dy;
    xi = 0;                  // take start position as (0, ry)
    yi = (int)(ry + 0.5);    // finding decision parameter d1
    d1 = (ry * ry) - ((rx * rx) * ry) + (0.25 * (rx * rx));
    dx = 2 * (ry * ry) * xi;
    dy = 2 * (rx * rx) * yi;

    // region one
    do
    {
        // plot points by using 4 point symmetry
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x + xi + 0.5), (int)(y + yi + 0.5)); 
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x - xi + 0.5), (int)(y - yi + 0.5)); 
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x + xi + 0.5), (int)(y - yi + 0.5)); 
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x - xi + 0.5), (int)(y + yi + 0.5));

        if (d1 < 0)
        {
            xi++;
            dx = dx + (2 * (ry * ry));
            d1 = d1 + dx + (ry * ry);
        }
        else
        {
            xi++;
            yi--;
            dx = dx + (2 * (ry * ry));
            dy = dy - (2 * (rx * rx));
            d1 = d1 + dx - dy + (ry * ry);
        }
    } while(dx < dy);

    d2 = (ry * ry) * (xi + 0.5) * (xi + 0.5) + (rx * rx) * (yi - 1) * (yi - 1) - ((rx * rx) * (ry * ry));

    // region two
    do
    {
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x + xi + 0.5), (int)(y + yi + 0.5)); 
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x - xi + 0.5), (int)(y - yi + 0.5)); 
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x + xi + 0.5), (int)(y - yi + 0.5)); 
        draw_pixel(current_color.R, current_color.G, current_color.B, (int)(x - xi + 0.5), (int)(y + yi + 0.5));

        if(d2 > 0)
        {
            yi--;
            dy = dy - (2 * (rx * rx));
            d2 = d2 - dy + (rx * rx);
        }
        else
        {
            xi++;
            yi--;
            dy = dy - (2 * (rx * rx));
            dx = dx + (2 * (ry * ry));
            d2 = d2 + dx - dy + (rx * rx);
        }
    } while(yi >= 0);
}

void superdraw_curve_bezier(int n, float *xarray, float *yarray) {
    int i, k;
    float t = 0;
    float xlast = xarray[0];
    float ylast = yarray[0];
    float *xworkspace = malloc(sizeof(float) * n);
    float *yworkspace = malloc(sizeof(float) * n);

    while ((t += BEZIER_STEP) < 1) {
        // get next point
        // k = 0
        for (i = 0; i < n; i++) {
            xworkspace[i] = xarray[i];
            yworkspace[i] = yarray[i];
        }
        for (k = 1; k <= n; k++) {
            for (i = 0; i < n - k; i++) {
                xworkspace[i] = (1 - t) * xworkspace[i] + t * xworkspace[i + 1];
                yworkspace[i] = (1 - t) * yworkspace[i] + t * yworkspace[i + 1];
            }
        }
        superdraw_line_bresenham(xlast, ylast, xworkspace[0], yworkspace[0]);
        xlast = xworkspace[0];
        ylast = yworkspace[0];
    }

    superdraw_line_bresenham(xlast, ylast, xarray[n - 1], yarray[n - 1]);

    free(xworkspace);
    free(yworkspace);
}