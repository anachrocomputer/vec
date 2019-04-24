/* spiraqlsq --- plot a spiral optical illusion             2013-05-11 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

#define RADIANS  (M_PI / 180.0)

void ringoboxes2 (double x0, double y0, double radius, int nboxes, int ninner, int offset, double slant);


int main (int argc, char * const argv[])
{
   int opt;
   int i;
   double xc, yc;
   double maxx, maxy;
   double height;
   double radius;
   double r;
   double slant;
   
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

   plotbegin (0);

   getplotsize (&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   height = maxy;

   /* Draw square border */
   rectangle (xc - (height / 2.0), 0.0, xc + (height / 2.0), maxy);

   radius = height / 12.0;
   
   /* Draw four concentric circles of squares */
   for (i = 0; i < 4; i++) {
      if (i & 1)
         slant = 20.0;
      else
         slant = -20.0;
         
      r = radius * (i + 1.5);
      ringoboxes2 (xc, yc, r, 18 * (i + 1), 1, 0, slant * RADIANS);
   }
   
   pencolr (1);
   
   /* Draw four concentric circles of squares */
   for (i = 0; i < 4; i++) {
      if (i & 1)
         slant = 20.0;
      else
         slant = -20.0;
         
      r = radius * (i + 1.5);
      ringoboxes2 (xc, yc, r, 18 * (i + 1), 1, 1, slant * RADIANS);
   }
   
   plotend ();
   
   return (0);
}


void ringoboxes2 (double x0, double y0, double radius, int nboxes, int ninner, int offset, double slant)
{
   int i, j, k, n;
   double side, s2;
   double theta;
   double delta;
   double s, c;
   double x[4], y[4];
   double rx[4], ry[4];
   double inc;
   double sr, cr;
   
   side = (2.0 * M_PI * radius) / (double)nboxes;
   side *= 0.8;
   s2 = side / 2.0;
   inc = s2 / (double)ninner;
   
   delta = (2.0 * M_PI) / (double)nboxes;

   delta *= 2.0;
   nboxes /= 2;
   
   for (i = 0; i < nboxes; i++) {
      theta = (double)i * delta;

      if (offset)
         theta += delta / 2.0;
         
      s = sin (theta);
      c = cos (theta);
      
      for (k = 0; k < ninner; k++) {
         sr = sin (slant);
         cr = cos (slant);
         
         /* Set up a square */
         x[0] = -s2;
         y[0] = -s2;
         
         x[1] = s2;
         y[1] = -s2;
         
         x[2] = s2;
         y[2] = s2;
         
         x[3] = -s2;
         y[3] = s2;
         
         for (n = 0; n < 4; n++) {
            x[n] = (x[n] * cr) - (y[n] * sr);
            y[n] = (x[n] * sr) + (y[n] * cr);
         }

         /* Shrink, rotate and translate square */
         for (j = 0; j < 4; j++) {
            if (x[j] < 0)
               x[j] += k * inc;
            else
               x[j] -= k * inc;

            if (y[j] < 0)
               y[j] += k * inc;
            else
               y[j] -= k * inc;

            rx[j] = (x[j] * c) - (y[j] * s);
            ry[j] = (x[j] * s) + (y[j] * c);
            
            rx[j] += x0 + (c * (radius + s2));
            ry[j] += y0 + (s * (radius + s2));
         }
         
         /* Draw the rotated square */
         moveto (rx[0], ry[0]);
         lineto (rx[1], ry[1]);
         lineto (rx[2], ry[2]);
         lineto (rx[3], ry[3]);
         lineto (rx[0], ry[0]);
      }
   }
}
