/* ellipse --- plot a true ellipse                          2011-10-08 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

void ellipse_foci (double x1, double y1, double x2, double y2, double d);
void ellipse (double x0, double y0, double a, double b, double theta);


int main (int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double x1, y1;
   double x2, y2;
   int i;
   
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
//       exit (EXIT_FAILURE);
      }
   }

   /* Select first pen and draw border */
   plotbegin (1);

   getplotsize (&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   /* Draw axes and foci */
   moveto (0.0, yc);
   lineto (maxx, yc);
   moveto (xc, 0.0);
   lineto (xc, maxy);
   
   x1 = xc - (50.0 * 40.0);
   y1 = yc;
   
   x2 = xc + (50.0 * 40.0);
   y2 = yc;
   
   moveto (x1, y1 + (5.0 * 40.0));
   lineto (x1, y1 - (5.0 * 40.0));
   
   moveto (x2, y2 + (5.0 * 40.0));
   lineto (x2, y2 - (5.0 * 40.0));
   
   /* Draw bunch of ellipses */
// ellipse (xc, yc, 80.0 * 40.0, 70.0 * 40.0, 0.0);
   
   for (i = 0; i < 10; i++)
      ellipse_foci (x1, y1, x2, y2, (110.0 + (10.0 * i)) * 40.0);

   plotend ();
   
   return (0);
}


void ellipse_foci (double x1, double y1, double x2, double y2, double d)
{
   double f2;
   double dx, dy;
   double x0, y0;
   double a, b;
   double theta;
   
   dx = x2 - x1;
   dy = y2 - y1;
   
   f2 = sqrt ((dx * dx) + (dy * dy)) / 2.0;
   a = d / 2.0;
   b = sqrt ((a * a) - (f2 * f2));
   
   x0 = (x1 + x2) / 2.0;
   y0 = (y1 + y2) / 2.0;
   
   theta = atan2 (dy, dx);
   
   ellipse (x0, y0, a, b, theta);
}


void ellipse (double x0, double y0, double a, double b, double theta)
{
   double t;
   double delta;
   double sintheta, costheta;
   double x, y;
   int npts = 72;
   int i;
   
   delta = (2.0 * M_PI) / (double)npts;

   sintheta = sin (theta);
   costheta = cos (theta);
   
   for (i = 0; i <= npts; i++) {
      t = (double)i * delta;
      
      x = (a * cos (t) * costheta) - (b * sin (t) * sintheta);
      y = (a * cos (t) * sintheta) + (b * sin (t) * costheta);
      
      if (i == 0)
         moveto (x0 + x, y0 + y);
      else
         lineto (x0 + x, y0 + y);
   }
}
