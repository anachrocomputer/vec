/* arches --- plot various kinds of arches                  2011-10-01 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

void circulararch (double x0, double y0, double width, double height, double r);
void threecentredarch (double x0, double y0, double width, double height, double d, double r1);
void ellipticalarch (double x0, double y0, double width, double height, double a, double b);
void ellipse (double x0, double y0, double a, double b, double theta);
void half_ellipse (double x0, double y0, double a, double b, double theta);


int main (int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double xcell;
   double ycell;
   
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
   
   xcell = maxx / 3.0;
   ycell = maxy / 2.0;
   
   /* Divide page into six boxes */
   moveto (0.0, ycell);
   lineto (maxx, ycell);
   
   moveto (xcell, 0.0);
   lineto (xcell, maxy);
   
   moveto (xcell * 2.0, maxy);
   lineto (xcell * 2.0, 0.0);
   
   circulararch (xcell, ycell, xcell, ycell, 50.0);
   
   ellipticalarch (0.0, 0.0, xcell, ycell, 50.0, 35.0);
   
   threecentredarch (0.0, ycell, xcell, ycell, 30.0, 20.0);
   
// threecentredarch (xcell, 0.0, xcell, ycell, 33.0, 22.0);

   plotend ();
   
   return (0);
}


void circulararch (double x0, double y0, double width, double height, double r)
{
   double xc, yc;

   xc = x0 + (width / 2.0);
   yc = y0 + (height / 2.0);

   /* Centre lines */
   moveto (x0, yc);
   lineto (x0 + width, yc);
   moveto (xc, y0);
   lineto (xc, y0 + height);
   
   /* Circle forming arch, drawn as full circle */
   circle (xc, yc, r * 40.0);

   /* Thicker pen for outline of arch */
// pencolr (1);
// printf ("VS5;\n");

   moveto (xc - (r * 40.0), y0);
   lineto (xc - (r * 40.0), yc);

   /* The arch itself, a half circle */
   arc (xc, yc, -180.0);
   
   lineto (xc + (r * 40.0), y0);
}


void threecentredarch (double x0, double y0, double width, double height, double d, double r1)
{
   /* d: Distance from central axis to centre of smaller arcs */
   /* r1: Radius of smaller arcs */
   double r2;  /* Radius of larger arc */
   double xc, yc;
   
   xc = x0 + (width / 2.0);
   yc = y0 + (height / 2.0);

   r2 = sqrt ((d * d) + (d * d)) + r1;
   
   /* Centre lines */
   moveto (x0, yc);
   lineto (x0 + width, yc);
   moveto (xc, y0);
   lineto (xc, y0 + height);

   /* LH centre mark */
   moveto (xc - (d * 40.0), yc + (3.0 * 40.0));
   lineto (xc - (d * 40.0), yc - (3.0 * 40.0));
   
   /* LH small circle */
   circle (xc - (d * 40.0), yc, r1 * 40.0);

   /* RH centre mark */
   moveto (xc + (d * 40.0), yc + (3.0 * 40.0));
   lineto (xc + (d * 40.0), yc - (3.0 * 40.0));

   /* RH small circle */
   circle (xc + (d * 40.0), yc, r1 * 40.0);

   /* 45 degree construction lines */
   moveto (xc - (2.0 * d * 40.0), yc + (d * 40.0));
   lineto (xc, yc - (d * 40.0));
   lineto (xc + (2.0 * d * 40.0), yc + (d * 40.0));

   /* Bottom centre mark */
   moveto (xc - (3.0 * 40.0), yc - (d * 40.0));
   lineto (xc + (3.0 * 40.0), yc - (d * 40.0));

   /* Upper large circle, only drawn as half-circle */
   moveto (xc + (r2 * 40.0), yc - (d * 40.0));
   arc (xc, yc - (d * 40.0), 180.0);
   
   /* Thicker pen for outline of arch */
// pencolr (1);
// printf ("VS5;\n");

   moveto (xc - ((d + r1) * 40.0), y0);
   lineto (xc - ((d + r1) * 40.0), yc);

   arc (xc - (d * 40.0), yc, -45.0);
   arc (xc, yc - (d * 40.0), -90.0);
   arc (xc + (d * 40.0), yc, -45.0);

   lineto (xc + ((d + r1) * 40.0), y0);
}


void ellipticalarch (double x0, double y0, double width, double height, double a, double b)
{
   double xc, yc;

   xc = x0 + (width / 2.0);
   yc = y0 + (height / 2.0);

   /* Centre lines */
   moveto (x0, yc);
   lineto (x0 + width, yc);
   moveto (xc, y0);
   lineto (xc, y0 + height);
   
   /* Ellipse forming arch, drawn in full */
   ellipse (xc, yc, a * 40.0, b * 40.0, 0.0);

   /* Thicker pen for outline of arch */
// pencolr (1);
// printf ("VS5;\n");

   moveto (xc + (a * 40.0), y0);
   lineto (xc + (a * 40.0), yc);

   /* The arch itself, a half ellipse */
   half_ellipse (xc, yc, a * 40.0, b * 40.0, 0.0);
   
   lineto (xc - (a * 40.0), y0);
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


void half_ellipse (double x0, double y0, double a, double b, double theta)
{
   double t;
   double delta;
   double sintheta, costheta;
   double x, y;
   int npts = 36;
   int i;
   
   delta = M_PI / (double)npts;

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
