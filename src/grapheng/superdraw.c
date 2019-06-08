/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    superdraw.c
    Renderer
 */

#include <math.h>
#include <stdlib.h>

#include <grapheng.h>
#include <exec.h>

extern inline float _rotate_point_x(float x, float y, float xr, float yr, float r);
extern inline float _rotate_point_y(float x, float y, float xr, float yr, float r);

#define CURVE_STEP   0.001

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
    deltaerr = fabs(deltay / deltax);
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

void superdraw_ellipse_rotate(float x, float y, float rx, float ry, float r) {
    int xi, yi;
    float d1, d2, dx, dy;
    float xlast1, ylast1;
    float xnew1, ynew1;
    float xlast2, ylast2;
    float xnew2, ynew2;
    float xlast3, ylast3;
    float xnew3, ynew3;
    float xlast4, ylast4;
    float xnew4, ynew4;
    xi = 0;                  // take start position as (0, ry)
    yi = (int)(ry + 0.5);    // finding decision parameter d1
    d1 = (ry * ry) - ((rx * rx) * ry) + (0.25 * (rx * rx));
    dx = 2 * (ry * ry) * xi;
    dy = 2 * (rx * rx) * yi;
    xlast1 = _rotate_point_x(xi, yi, 0, 0, r);
    ylast1 = _rotate_point_y(xi, yi, 0, 0, r);
    xlast2 = _rotate_point_x(-xi, -yi, 0, 0, r);
    ylast2 = _rotate_point_y(-xi, -yi, 0, 0, r);
    xlast3 = _rotate_point_x(xi, -yi, 0, 0, r);
    ylast3 = _rotate_point_y(xi, -yi, 0, 0, r);
    xlast4 = _rotate_point_x(-xi, yi, 0, 0, r);
    ylast4 = _rotate_point_y(-xi, yi, 0, 0, r);

    // region one
    do
    {
        xnew1 = _rotate_point_x(xi, yi, 0, 0, r);
        ynew1 = _rotate_point_y(xi, yi, 0, 0, r);
        xnew2 = _rotate_point_x(-xi, -yi, 0, 0, r);
        ynew2 = _rotate_point_y(-xi, -yi, 0, 0, r);
        xnew3 = _rotate_point_x(xi, -yi, 0, 0, r);
        ynew3 = _rotate_point_y(xi, -yi, 0, 0, r);
        xnew4 = _rotate_point_x(-xi, yi, 0, 0, r);
        ynew4 = _rotate_point_y(-xi, yi, 0, 0, r);
        superdraw_line_bresenham((int)(x + xlast1 + 0.5), (int)(y + ylast1 + 0.5),
            (int)(x + xnew1 + 0.5), (int)(y + ynew1 + 0.5));
        superdraw_line_bresenham((int)(x + xlast2 + 0.5), (int)(y + ylast2 + 0.5),
            (int)(x + xnew2 + 0.5), (int)(y + ynew2 + 0.5));
        superdraw_line_bresenham((int)(x + xlast3 + 0.5), (int)(y + ylast3 + 0.5),
            (int)(x + xnew3 + 0.5), (int)(y + ynew3 + 0.5));
        superdraw_line_bresenham((int)(x + xlast4 + 0.5), (int)(y + ylast4 + 0.5),
            (int)(x + xnew4 + 0.5), (int)(y + ynew4 + 0.5));
        xlast1 = xnew1;
        ylast1 = ynew1;
        xlast2 = xnew2;
        ylast2 = ynew2;
        xlast3 = xnew3;
        ylast3 = ynew3;
        xlast4 = xnew4;
        ylast4 = ynew4;

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
        xnew1 = _rotate_point_x(xi, yi, 0, 0, r);
        ynew1 = _rotate_point_y(xi, yi, 0, 0, r);
        xnew2 = _rotate_point_x(-xi, -yi, 0, 0, r);
        ynew2 = _rotate_point_y(-xi, -yi, 0, 0, r);
        xnew3 = _rotate_point_x(xi, -yi, 0, 0, r);
        ynew3 = _rotate_point_y(xi, -yi, 0, 0, r);
        xnew4 = _rotate_point_x(-xi, yi, 0, 0, r);
        ynew4 = _rotate_point_y(-xi, yi, 0, 0, r);
        superdraw_line_bresenham((int)(x + xlast1 + 0.5), (int)(y + ylast1 + 0.5),
            (int)(x + xnew1 + 0.5), (int)(y + ynew1 + 0.5));
        superdraw_line_bresenham((int)(x + xlast2 + 0.5), (int)(y + ylast2 + 0.5),
            (int)(x + xnew2 + 0.5), (int)(y + ynew2 + 0.5));
        superdraw_line_bresenham((int)(x + xlast3 + 0.5), (int)(y + ylast3 + 0.5),
            (int)(x + xnew3 + 0.5), (int)(y + ynew3 + 0.5));
        superdraw_line_bresenham((int)(x + xlast4 + 0.5), (int)(y + ylast4 + 0.5),
            (int)(x + xnew4 + 0.5), (int)(y + ynew4 + 0.5));
        xlast1 = xnew1;
        ylast1 = ynew1;
        xlast2 = xnew2;
        ylast2 = ynew2;
        xlast3 = xnew3;
        ylast3 = ynew3;
        xlast4 = xnew4;
        ylast4 = ynew4;

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

    while ((t += CURVE_STEP) < 1) {
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

void superdraw_curve_b_spline(int n, float *xarray, float *yarray) {
    double t, t2, t3;
    double x, y, xlast, ylast;

    for(int k = 0; k < n - 3; k++)
    {
        double a0 = (xarray[k] + 4 * xarray[k + 1] + xarray[k + 2]) / 6;
        double a1 = -(xarray[k] - xarray[k + 2]) / 2;
        double a2 = (xarray[k+2] - 2 * xarray[k + 1] + xarray[k]) / 2;
        double a3 = -(xarray[k] - 3 * xarray[k + 1] + 3 * xarray[k + 2] - xarray[k + 3]) / 6;
        double b0 = (yarray[k] + 4 * yarray[k + 1] + yarray[k+2]) / 6;
        double b1 = -(yarray[k] - yarray[k+2]) / 2;
        double b2 = (yarray[k + 2] - 2 * yarray[k + 1] + yarray[k]) / 2;
        double b3 = -(yarray[k] - 3 * yarray[k + 1] + 3 * yarray[k + 2] - yarray[k + 3]) / 6;
        xlast = a0;
        ylast = b0;
        for (t = CURVE_STEP; t < 1; t += CURVE_STEP)
        {
            t2 = t * t;
            t3 = t2 * t;

            x = a0 + a1 * t + a2 * t2 + a3 * t3;
            y = b0 + b1 * t + b2 * t2 + b3 * t3;
            superdraw_line_dda(xlast, ylast, x, y);
            xlast = x;
            ylast = y;
        }
    }
}