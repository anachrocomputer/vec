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

void draw_module (double x1, double y1, double side, int lastx, int lasty);
void square_triangle (double x, double y, double side, double ht);
void drawline (double x1, double y1, double x2, double y2);

double Scale;

int main (int argc, char * const argv[])
{
   int opt;
   int ix, iy;
   int nx, ny;
   double pitch;
   double x1, y1;
   double maxx, maxy;
   
   while ((opt = getopt (argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 'n':
      case 'o':
      case 'p':
      case 's':
      case 't':
      case 'v':
         plotopt (opt, optarg);
         break;
      default: /* '?' */
         fprintf (stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n",
                  argv[0]);
         fprintf (stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit (EXIT_FAILURE);
      }
   }

   /* Basic drawing parameters */
   nx = 8;
   ny = 6;
   Scale = 40.0;
   pitch = 45.0 * Scale;
   
   plotbegin (1);

   getplotsize (&maxx, &maxy);
   
   for (ix = 0; ix < nx; ix++) {
      for (iy = 0; iy < ny; iy++) {
         x1 = (double)ix * pitch;
         y1 = (double)iy * pitch;
         draw_module (x1, y1, pitch, ix == nx - 1, iy == ny - 1);
      }
   }

   plotend ();
   
   return (0);
}


void draw_module (double x, double y, double side, int lastx, int lasty)
{
   int i;
   double radius;
   double offset;
   double trueside;
   double step = 5.5 * Scale;
   double hside = side / 2.0;
   
   offset = (double)3 * step;
   
   drawline (x + offset, y + hside, x + side - offset, y + hside);
   drawline (x + hside, y + offset, x + hside, y + side - offset);
   
   for (i = 0; i < 3; i++) {
      offset = (double)(i + 1) * step;
      trueside = side - (2.0 * step * (double)(i + 1));
      square_triangle (x + offset, y + offset, trueside, step);
   }
   
   radius = sqrt ((step * step) + (step * step));
   
   circle (x, y, radius);
   
   if (lastx)
      circle (x + side, y, radius);
      
   if (lasty)
      circle (x, y + side, radius);
      
   if (lastx && lasty)
      circle (x + side, y + side, radius);
}


void square_triangle (double x, double y, double side, double ht)
{
   double hside;
   
   rectangle (x, y, x + side, y + side);

   hside = side / 2.0;
   
   moveto (x, y);
   lineto (x - ht, y + hside);
   lineto (x, y + side);
   lineto (x + hside, y + ht + side);
   lineto (x + side, y + side);
   lineto (x + side + ht, y + hside);
   lineto (x + side, y);
   lineto (x + hside, y - ht);
   lineto (x, y);
}


void drawline (double wx1, double wy1, double wx2, double wy2)
{
   static int cx = -32767, cy = -32767;
   static double wcx = -1.0, wcy = -1.0;
   int x1, y1, x2, y2;
   double dx1, dy1, dx2, dy2;
   double d1, d2;
   char hpgl[32];
   
   dx1 = wx1 - wcx;
   dy1 = wy1 - wcy;
   dx2 = wx2 - wcx;
   dy2 = wy2 - wcy;
   
   d1 = sqrt ((dx1 * dx1) + (dy1 * dy1));
   d2 = sqrt ((dx2 * dx2) + (dy2 * dy2));
   
   if (d1 < d2) {
      x1 = getdevx (wx1);
      y1 = getdevy (wy1);
      x2 = getdevx (wx2);
      y2 = getdevy (wy2);
      wcx = wx2;
      wcy = wy2;
   }
   else {
      x1 = getdevx (wx2);
      y1 = getdevy (wy2);
      x2 = getdevx (wx1);
      y2 = getdevy (wy1);
      wcx = wx1;
      wcy = wy1;
   }
   
   if ((cx != x1) || (cy != y1)) {
      snprintf (hpgl, 32, "\nPU;PA%d,%d;", x1, y1);
      hpglout (hpgl);
   }
      
   snprintf (hpgl, 32, "PD;PA%d,%d;", x2, y2);   
   hpglout (hpgl);
   
   cx = x2;
   cy = y2;
}
