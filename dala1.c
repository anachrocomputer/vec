/* dala1 --- plot an HPGL mandala                           2011-09-10 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hpgllib.h"


double diamondsquare (double x0, double y0, double side);
double radials (double x0, double y0, double inner, double length, int n);
double ringoboxes (double x0, double y0, double radius, int nboxes, int ninner);
void superellipse (double x0, double y0, double a, double b, double theta, double d);


double Scale = 40.0;

int main (int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double height;
   double radius;
   
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

   /* 12.5mm square */
   diamondsquare (xc, yc, 12.5 * Scale);

   /* 25mm square */
   diamondsquare (xc, yc, 25.0 * Scale);

   /* 50mm square */
   diamondsquare (xc, yc, 50.0 * Scale);
   
   /* 100mm square */
   radius = diamondsquare (xc, yc, 100.0 * Scale);
   
   /* Plot circle around square */
   circle (xc, yc, radius);
   
   /* Plot radial lines */
   radius = radials (xc, yc, radius + (5.0 * Scale), 30.0 * Scale, 128);
   
   radius = ringoboxes (xc, yc, radius + (5.0 * Scale), 48, 3);
   
   radius = ((height / 2.0) + radius) / 2.0;
   
   superellipse (xc, yc, radius, radius, 0.0, 4.0);
   
   plotend ();
   
   return (0);
}


double diamondsquare (double x0, double y0, double side)
{
   side /= 2.0;
   
   /* Plot outer square */
   moveto (x0 - side, y0 - side);
   lineto (x0 + side, y0 - side);
   lineto (x0 + side, y0 + side);
   lineto (x0 - side, y0 + side);
   lineto (x0 - side, y0 - side);
   
   /* Plot inner diagonal square */
   moveto (x0, y0 - side);
   lineto (x0 + side, y0);
   lineto (x0, y0 + side);
   lineto (x0 - side, y0);
   lineto (x0, y0 - side);
   
   return (sqrt (side * side * 2.0));
}


double radials (double x0, double y0, double inner, double length, int n)
{
   int i;
   double theta;
   double delta;
   double xvec, yvec;
   double x1, y1;
   double x2, y2;
   
   delta = (2.0 * M_PI) / (double)n;
   
   for (i = 0; i < n; i++) {
      theta = (double)i * delta;
      xvec = cos (theta);
      yvec = sin (theta);
      
      x1 = xvec * inner;
      y1 = yvec * inner;
      
      x2 = xvec * (inner + length);
      y2 = yvec * (inner + length);
      
      if (i & 1) {
         moveto (x0 + x1, y0 + y1);
         lineto (x0 + x2, y0 + y2);
      }
      else {
         moveto (x0 + x2, y0 + y2);
         lineto (x0 + x1, y0 + y1);
      }
   }
   
   return (inner + length);
}


double ringoboxes (double x0, double y0, double radius, int nboxes, int ninner)
{
   int i, j, k;
   double side, s2;
   double theta;
   double delta;
   double s, c;
   double x[4], y[4];
   double rx[4], ry[4];
   double inc;
   
   side = (2.0 * M_PI * radius) / (double)nboxes;
   side *= 0.8;
   s2 = side / 2.0;
   inc = s2 / (double)ninner;
   
   delta = (2.0 * M_PI) / (double)nboxes;

   for (i = 0; i < nboxes; i++) {
      theta = (double)i * delta;
      s = sin (theta);
      c = cos (theta);
      
      for (k = 0; k < ninner; k++) {
         /* Set up a square */
         x[0] = -s2;
         y[0] = -s2;
         
         x[1] = s2;
         y[1] = -s2;
         
         x[2] = s2;
         y[2] = s2;
         
         x[3] = -s2;
         y[3] = s2;
         
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
   
   return (radius + side);
}


void superellipse (double x0, double y0, double a, double b, double theta, double d)
{
   double t;
   double delta;
   double st, ct;
   double sinpt, cospt;
   double sintheta, costheta;
   double x, y;
   int npts = 72;
   int i;
   
   delta = (2.0 * M_PI) / (double)npts;

   sintheta = sin (theta);
   costheta = cos (theta);
   
   for (i = 0; i <= npts; i++) {
      t = (double)i * delta;
      
      st = sin (t);
      ct = cos (t);
      
      if (st < 0.0)
         sinpt = -pow (-st, 2.0 / d);
      else
         sinpt = pow (st, 2.0 / d);
      
      if (ct < 0.0)
         cospt = -pow (-ct, 2.0 / d);
      else
         cospt = pow (ct, 2.0 / d);
      
      x = (a * cospt * costheta) - (b * sinpt * sintheta);
      y = (a * cospt * sintheta) + (b * sinpt * costheta);
      
      if (i == 0)
         moveto (x0 + x, y0 + y);
      else
         lineto (x0 + x, y0 + y);
   }
}
