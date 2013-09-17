/* fraserspiral --- plot the Fraser Spiral optical illusion 2013-05-16 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"


void ringoshapes (double x0, double y0, double radius, int nshapes);
void spiral (double cx, double cy, double radius, double ang, int n);
double diamondsquare (double x0, double y0, double side);
double zigzagring (double x0, double y0, double r1, double r2, int npts, int incr, int flag);
double radials (double x0, double y0, double inner, double length, int n);
double circlediamonds (double x0, double y0, double radius, int ndiam);
double ringofcircles (double x0, double y0, double radius, int ncirc);
double sqwavering (double x0, double y0, double radius, double len, int nwaves);
double ellipsering (double x0, double y0, double a, double b, int nell);
void ellipse (double x0, double y0, double a, double b, double theta);
void superellipse (double x0, double y0, double a, double b, double theta, double d);


int main (int argc, char * const argv[])
{
   int opt;
   int i;
   double xc, yc;
   double maxx, maxy;
   double height;
   double radius;
   double r;
   
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

   radius = height / 24.0;
   
   /* Draw eleven concentric circles of shapes */
   for (i = 0; i < 11; i++) {
      r = radius * (i + 1);
      ringoshapes (xc, yc, r, 16 + i);
   }
   
   plotend ();
   
   return (0);
}


void ringoshapes (double x0, double y0, double radius, int nshapes)
{
   int i;
   double theta;
   double x, y;
   double delta;
   double xc, yc;
   double d;
   
   delta = 2.0 * M_PI / (double)nshapes;
   
   for (i = 0; i < nshapes; i++) {
      theta = i * delta;
      
      x = (radius * cos (theta)) + x0;
      y = (radius * sin (theta)) + y0;
   
      moveto (x, y);
      lineto (x, y);
      
      d = radius / 4.0;
      
      xc = (d * cos (theta + (M_PI / 2.0))) + x0;
      yc = (d * sin (theta + (M_PI / 2.0))) + y0;

      arc (xc, yc, (delta * 1.2) * 57.295);
   }
}


void spiral (double cx, double cy, double radius, double ang, int n)
{
   double theta = ang;
   double thinc = 2.0 * M_PI / 72.0;
   double r;
   int i;
   int ptnumber = 72 * n;
   double x, y;
   
   moveto (cx, cy);
   
   for (i = 0; i < ptnumber; i++) {
      theta += thinc;
      r = (radius * i) / (double)ptnumber;
      x = (r * cos (theta)) + cx;
      y = (r * sin (theta)) + cy;
      
      lineto (x, y);
   }
}


double diamondsquare (double x0, double y0, double side)
{
   side /= 2.0;
   
   /* Plot outer square */
   rectangle (x0 - side, y0 - side, x0 + side, y0 + side);
   
   /* Plot inner diagonal square */
   moveto (x0, y0 - side);
   lineto (x0 + side, y0);
   lineto (x0, y0 + side);
   lineto (x0 - side, y0);
   lineto (x0, y0 - side);
   
   /* Return radius of circumscribing circle */
   return (sqrt ((side * side) * 2.0));
}


double zigzagring (double x0, double y0, double r1, double r2, int npts, int incr, int flag)
{
   int i;
   double x1[128], y1[128];
   double x2[128], y2[128];
   double theta1, theta2;
   double delta;
   int n1, n2;
   
   delta = (2.0 * M_PI) / (double)npts;
   
   for (i = 0; i < npts; i++) {
      if (flag) {
         theta2 = delta * (double)i;
         theta1 = (delta * (double)i) + (delta / 2.0);
      }
      else {
         theta1 = delta * (double)i;
         theta2 = (delta * (double)i) + (delta / 2.0);
      }
      
      x1[i] = (cos (theta1) * r1) + x0;
      y1[i] = (sin (theta1) * r1) + y0;
      
      x2[i] = (cos (theta2) * r2) + x0;    
      y2[i] = (sin (theta2) * r2) + y0;
   }
   
   moveto (x1[0], y1[0]);
   n1 = 0;
   n2 = incr / 2;
   
   for (i = 0; i < npts; i++) {
      lineto (x1[n1], y1[n1]);
      lineto (x2[n2], y2[n2]);
      
      n1 = (n1 + incr) % npts;
      n2 = (n2 + incr) % npts;
   }
   
   lineto (x1[0], y1[0]);
   
   return (r2);
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


double circlediamonds (double x0, double y0, double radius, int ndiam)
{
   int i;
   double theta;
   double delta;
   double diag;
   double r1, r2;
   double x1[128], y1[128];
   double x2[128], y2[128];
   
   diag = (2.0 * M_PI * radius) / (double)ndiam;
   
   radius += diag / 2.0;
   
   circle (x0, y0, radius);

   r1 = radius - (diag / 2.0);
   r2 = radius + (diag / 2.0);
   
   delta = (M_PI * 2.0) / (double)ndiam;

   for (i = 0; i < ndiam; i++) {
      theta = (double)i * delta;
      
      x1[i] = (r1 * cos (theta)) + x0;
      y1[i] = (r1 * sin (theta)) + y0;
      
      x2[i] = (r2 * cos (theta)) + x0;
      y2[i] = (r2 * sin (theta)) + y0;
   }
   
   moveto (x1[0], y1[0]);
   
   for (i = 0; i < ndiam; i++) {
      if (i & 1)
         lineto (x2[i], y2[i]);
      else
         lineto (x1[i], y1[i]);
   }

   lineto (x1[0], y1[0]);

   moveto (x2[0], y2[0]);
   
   for (i = 0; i < ndiam; i++) {
      if (i & 1)
         lineto (x1[i], y1[i]);
      else
         lineto (x2[i], y2[i]);
   }

   lineto (x2[0], y2[0]);

   return (r2);
}


double ringofcircles (double x0, double y0, double radius, int ncirc)
{
   double delta;
   double theta;
   double r2;
   double x, y;
   int i;
   
   delta = (M_PI * 2.0) / (double)ncirc;
   
   /* Compute radius as if centres are on circumference of 'radius' */
   r2 = radius * sin (delta);
   
   /* Increase radius by radius of small circles */
   radius += r2;
   
   /* Recompute radius of smaller circles */
   r2 = radius * sin (delta);

   for (i = 0; i < ncirc; i++) {
      theta = (double)i * delta;
      
      x = radius * cos (theta);
      y = radius * sin (theta);
      
      circle2 (x0 + x, y0 + y, r2, 10.0);
   }
   
   return (radius + r2);
}


double sqwavering (double x0, double y0, double radius, double len, int nwaves)
{
   double delta;
   double theta;
   double degrees;
   double xvec, yvec;
   double x1, y1;
   double x2, y2;
   int i;
   
   /* Number of half-waves; must be even */
   nwaves *= 2;
   
   delta = (2.0 * M_PI) / (double)nwaves;
   degrees = 360.0 / (double)nwaves;

   for (i = 0; i < nwaves; i++) {
      theta = (double)i * delta;
      xvec = cos (theta);
      yvec = sin (theta);
      
      x1 = xvec * radius;
      y1 = yvec * radius;
      
      x2 = xvec * (radius + len);
      y2 = yvec * (radius + len);
      
      if (i == 0)
         moveto (x0 + x2, y0 + y2);

      if (i & 1)
         lineto (x0 + x2, y0 + y2);
      else
         lineto (x0 + x1, y0 + y1);

      arc (x0, y0, degrees);
   }
   
   return (radius + len);
}


double ellipsering (double x0, double y0, double a, double b, int nell)
{
   double delta;
   int i;
   
   delta = M_PI / (double)nell;

   for (i = 0; i < nell; i++)
      ellipse (x0, y0, a, b, delta * i);
      
   if (a > b)
      return (a);
   else
      return (b);
}


void ellipse (double x0, double y0, double a, double b, double theta)
{
   double t;
   double delta;
   double sintheta, costheta;
   double x, y;
   int npts = 72;
   int i;
   
   delta = (2 * M_PI) / (double)npts;

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
