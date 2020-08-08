/* ellipse --- plot a true ellipse                          2011-10-08 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

void ellipse_foci(const double x1, const double y1, const double x2, const double y2, const double d);


int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double x1, y1;
   double x2, y2;
   const double scale = 40.0; /* 40 plotter units per mm */
   int i;
   
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

   /* Select first pen and draw border */
   plotbegin(1);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   /* Draw axes and foci */
   moveto(0.0, yc);
   lineto(maxx, yc);
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   x1 = xc - (50.0 * scale);
   y1 = yc;
   
   x2 = xc + (50.0 * scale);
   y2 = yc;
   
   moveto(x1, y1 + (5.0 * scale));
   lineto(x1, y1 - (5.0 * scale));
   
   moveto(x2, y2 + (5.0 * scale));
   lineto(x2, y2 - (5.0 * scale));
   
   /* Draw bunch of ellipses */
   for (i = 0; i < 10; i++)
      ellipse_foci(x1, y1, x2, y2, (110.0 + (10.0 * i)) * scale);

   plotend();
   
   return (0);
}


void ellipse_foci(const double x1, const double y1, const double x2, const double y2, const double d)
{
   const double dx = x2 - x1;
   const double dy = y2 - y1;

   const double f2 = sqrt((dx * dx) + (dy * dy)) / 2.0;

   const double a = d / 2.0;
   const double b = sqrt((a * a) - (f2 * f2));
   
   const double x0 = (x1 + x2) / 2.0;
   const double y0 = (y1 + y2) / 2.0;
   
   const double theta = atan2(dy, dx);
   
   ellipse(x0, y0, a, b, theta);
}
