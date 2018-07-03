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
void drawline(const double wx1, const double wy1, const double wx2, const double wy2);

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
         fprintf(stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n",
                  argv[0]);
         fprintf(stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit(EXIT_FAILURE);
      }
   }

   /* Basic drawing parameters */
   nx = 8;
   ny = 6;
   Scale = 40.0;
   pitch = 45.0 * Scale;
   
   plotbegin(0);

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
   
   drawline(x + offset, y + hside, x + side - offset, y + hside);
   drawline(x + hside, y + offset, x + hside, y + side - offset);
   
   for (i = 0; i < 3; i++) {
      offset = (double)(i + 1) * step;
      trueside = side - (2.0 * step * (double)(i + 1));
      square_triangle(x + offset, y + offset, trueside, step);
   }
   
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


void drawline(const double wx1, const double wy1, const double wx2, const double wy2)
{
   static int cx = -32767, cy = -32767;
   static double wcx = -1.0, wcy = -1.0;
   int x1, y1, x2, y2;
   const double dx1 = wx1 - wcx;
   const double dy1 = wy1 - wcy;
   const double dx2 = wx2 - wcx;
   const double dy2 = wy2 - wcy;
   const double d1 = sqrt((dx1 * dx1) + (dy1 * dy1));
   const double d2 = sqrt((dx2 * dx2) + (dy2 * dy2));
   char hpgl[32];
   
   if (d1 < d2) {
      x1 = getdevx(wx1);
      y1 = getdevy(wy1);
      x2 = getdevx(wx2);
      y2 = getdevy(wy2);
      wcx = wx2;
      wcy = wy2;
   }
   else {
      x1 = getdevx(wx2);
      y1 = getdevy(wy2);
      x2 = getdevx(wx1);
      y2 = getdevy(wy1);
      wcx = wx1;
      wcy = wy1;
   }
   
   if ((cx != x1) || (cy != y1)) {
      snprintf(hpgl, sizeof (hpgl), "\nPU;PA%d,%d;", x1, y1);
      hpglout(hpgl);
   }

   snprintf(hpgl, sizeof (hpgl), "PD;PA%d,%d;", x2, y2);
   hpglout(hpgl);
   
   cx = x2;
   cy = y2;
}
