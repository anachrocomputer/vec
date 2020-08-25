/* allover13 --- draw all-over pattern, page 13             2013-06-23 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

/* Geometric Allover Patterns, by Ian O. Angell
 * Dover Publications, Inc., 1985, ISBN 0-486-24855-0
 * Page 13
 */

void draw_module(const double x, const double y, const double side, const int firstx, const int lastx, const int firsty, const int lasty);
void square_triangle(const double x, const double y, const double side, const double ht);

double Scale;

int main(int argc, char * const argv[])
{
   int opt;
   int ix, iy;
   int nx, ny;
   double pitch;
   double x0, y0;
   double width, height;
   double xoffset, yoffset;
   double maxx, maxy;
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 'n':
      case 'o':
      case 'p':
      case 's':
      case 't':
      case 'v':
         plotopt(opt, optarg);
         break;
      default: /* '?' */
         fprintf(stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n", argv[0]);
         fprintf(stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit(EXIT_FAILURE);
      }
   }

   /* Basic drawing parameters */
   nx = 8;
   ny = 6;
   Scale = 40.0;
   pitch = 45.0 * Scale;
   
   if (plotbegin(0) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   width = nx * pitch;
   height = ny * pitch;
   
   xoffset = (maxx - width) / 2.0;
   yoffset = (maxy - height) / 2.0;
   
   rectangle(xoffset, yoffset, xoffset + width, yoffset + height);
   
   for (ix = 0; ix < nx; ix++) {
      for (iy = 0; iy < ny; iy++) {
         x0 = xoffset + ((double)ix * pitch);
         y0 = yoffset + ((double)iy * pitch);
         draw_module(x0, y0, pitch, ix == 0, ix == nx - 1, iy == 0, iy == ny - 1);
      }
   }

   plotend();
   
   return (0);
}


void draw_module(const double x, const double y, const double side, const int firstx, const int lastx, const int firsty, const int lasty)
{
   int i;
   double trueside;
   const double step = 5.5 * Scale;
   const double radius = sqrt ((step * step) + (step * step));
   double hside = side / 2.0;
   double offset = 3.0 * step;
   
   // Draw central cross
   moveto(x + offset,        y + hside);
   lineto(x + side - offset, y + hside);
   moveto(x + hside,         y + offset);
   lineto(x + hside,         y + side - offset);
   
   // Draw squares surrounded by triangles
   for (i = 0; i < 3; i++) {
      offset = (double)(i + 1) * step;
      trueside = side - (2.0 * step * (double)(i + 1));
      square_triangle(x + offset, y + offset, trueside, step);
   }
   
   // Draw circles, allowing for special cases that draw arcs at the edges
   if (firsty) {
      moveto(x + radius, y);

      if (firstx)
         arc(x, y, 90.0);
      else
         arc(x, y, 180.0);
   }
   else if (firstx) {
      moveto(x, y - radius);
      arc(x, y, 180.0);
   }
   else
      circle(x, y, radius);
   
   if (lastx) {
      moveto(x + side, y + radius);

      if (firsty)
         arc(x + side, y, 90.0);
      else
         arc(x + side, y, 180.0);
   }

   if (lasty) {
      if (firstx) {
         moveto(x, y + side - radius);
         arc(x, y + side, 90.0);
      }
      else {
         moveto(x - radius, y + side);
         arc(x, y + side, 180.0);
      }
   }

   if (lastx && lasty) {
      moveto(x + side - radius, y + side);
      arc(x + side, y + side, 90.0);
   }
}


void square_triangle(const double x, const double y, const double side, const double ht)
{
   const double hside = side / 2.0;
   
   rectangle(x, y, x + side, y + side);

   moveto(x, y);
   lineto(x - ht, y + hside);
   lineto(x, y + side);
   lineto(x + hside, y + ht + side);
   lineto(x + side, y + side);
   lineto(x + side + ht, y + hside);
   lineto(x + side, y);
   lineto(x + hside, y - ht);
   lineto(x, y);
}
