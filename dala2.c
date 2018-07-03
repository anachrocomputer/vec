/* dala2 --- plot an HPGL mandala                           2011-09-10 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"


void catcher (int sig);

/* Central disc patterns */
double lissajous (double x0, double y0, double side, double f1, double f2, double theta, int npts);
double spiraldisc (double x0, double y0, double radius, int n, int turns);
double sunflowerspiral (double x0, double y0, double radius);
double lotus (double x0, double y0, double radius, int nleaves, int nlayers);
double diamondsquare (double x0, double y0, double side);

/* Intermediate ring patterns */
double zigzagring (double x0, double y0, double r1, double r2, int npts, int incr, int flag);
double radials (double x0, double y0, double inner, double length, int n);
double ringoboxes (double x0, double y0, double radius, int nboxes, int ninner);
double circlediamonds (double x0, double y0, double radius, int ndiam);
double ringofcircles (double x0, double y0, double radius, int ncirc);
double sqwavering (double x0, double y0, double radius, double len, int nwaves);
double ellipsering (double x0, double y0, double a, double b, int nell);
double ringofpoly (double x0, double y0, double radius, int nside, int npoly);
double circlering (double x0, double y0, double r1, double r2, int n);

/* Outer circle-to-square transitions */
double circletosuper (double x0, double y0, double r1, double r2, int n);
double sqradials (double x0, double y0, double r1, double r2, int n);
void curvestitchtrans (double x0, double y0, double r1, double r2, int n);

/* Square border drawing functions */
void saltireborder (double x0, double y0, int nx, int ny, double w, double h);
void roundrectborder (double x0, double y0, double r1, double r2, int n);
void circleborder (double x0, double y0, int nx, int ny, double w, double h);
void celticstepborder (double x0, double y0, int nx, int ny, double w, double h);

/* Utility drawing functions */
void radialfan (double x0, double y0, double r1, double startangle, double dx, double dy, double xvec, double yvec, int n);
void spiral (double cx, double cy, double radius, double ang, int n);
void bordergrid (double x0, double y0, double w, double h, int nx, int ny, int diag);
void superellipse (double x0, double y0, double a, double b, double theta, double d);
double lotuszigzag (double x0, double y0, double r1, double r2, int npts, int flag);
void roundrect (double x1, double y1, double x2, double y2, double radius);
void drawstep (double x0, double y0, double w, double h, int flipx, int flipy);

int main (int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double height;
   double radius;
   double inner_r;
   double scale = 40.0;
   
   while ((opt = getopt (argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
         if (strchr (optarg, '1'))
            scale = 80.0;
         else if (strchr (optarg, '2'))
            scale = 56.57;
         else if (strchr (optarg, '4'))
            scale = 28.28;
         else if (strchr (optarg, '5'))
            scale = 20.0;
            
      case 'n':
      case 'o':
      case 'p':
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

   radius = 20.0 * scale;
   
   /* Draw central spirals */
// radius = spiraldisc (xc, yc, radius, 3, 2);
   radius = lotus (xc, yc, radius, 16, 7);
// radius = lissajous (xc, yc, 38.0 * scale, 3.0, 4.0, 0.0, 144);

   /* Plot ring of ellipses around spirals */
   circle (xc, yc, radius);
   
   radius = ellipsering (xc, yc, radius + (1.0 * scale), radius + (9.0 * scale), 3);

   radius += 1.0 * scale;
   
   /* Pre-draw circles around zigzags */
   circle (xc, yc, radius);
   circle (xc, yc, radius + (50.0 * scale));
   
   /* Plot radial lines */
   radius = radials (xc, yc, radius, 5.0 * scale, 63);

   /* Plot two rings of zigzags */
   radius = zigzagring (xc, yc, radius, radius + (20.0 * scale), 63, 4, 0);
   radius = zigzagring (xc, yc, radius, radius + (25.0 * scale), 63, 8, 1);
   
   /* Plot steampunk cog */
// radius = sqwavering (xc, yc, radius + (2.0 * scale), 12.0 * scale, 18);
   
   /* Plot ring of concentric boxes */
   radius = ringoboxes (xc, yc, radius + (2.0 * scale), 48, 3);

   /* Plot ring of polygons */
// radius = ringofpoly (xc, yc, radius + (12.0 * scale), 7, 5);
   
   /* Plot circle with diamonds */
   circle (xc, yc, radius + (2.0 * scale));
   radius = circlediamonds (xc, yc, radius + (2.0 * scale), 36);
   circle (xc, yc, radius);

   radius += 2.0 * scale;
   
   /* Plot superellipses with increasingly sharp corners */
// circletosuper (xc, yc, radius, height / 2.0, 6);
// sqradials (xc, yc, radius, height / 2.0, 160);
// circlering (xc, yc, radius, height / 2.0, 4);
// ringofcircles (xc, yc, radius, 96);
   inner_r = (height / 2.0) - (height / 12.0);
// circle (xc, yc, inner_r);
   curvestitchtrans (xc, yc, radius, inner_r, 20);
   
   /* Draw a square border pattern */
// roundrectborder (xc, yc, inner_r, height / 2.0, 5);
// saltireborder (xc - (height / 2.0), 0.0, 12, 12, height / 12.0, height / 12.0);
// circleborder (xc - (height / 2.0), 0.0, 12, 12, height / 12.0, height / 12.0);
   celticstepborder (xc - (height / 2.0), 0.0, 12, 12, height / 12.0, height / 12.0);
   
   plotend ();
   
   return (0);
}


/* lissajous --- draw a lissajous pattern */

double lissajous (double x0, double y0, double side, double f1, double f2, double theta, int npts)
{
   double t;
   double t1, t2;
   double delta;
   double sintheta, costheta;
   double x, y;
   double r;
   int i;
   
   side /= 2.0;

   delta = (2.0 * M_PI) / (double)npts;

   r = side;
   
   sintheta = sin (theta);
   costheta = cos (theta);
   
   for (i = 0; i <= npts; i++) {
      t = (double)i * delta;
      
      t1 = t * f1;
      t2 = t * f2;
      
      x = (r * cos (t1) * costheta) - (r * sin (t2) * sintheta);
      y = (r * cos (t1) * sintheta) + (r * sin (t2) * costheta);
      
      if (i == 0)
         moveto (x0 + x, y0 + y);
      else
         lineto (x0 + x, y0 + y);
   }
   
   /* Return radius of circumscribing circle */
   return (sqrt ((side * side) * 2.0));
}


/* spiraldisc --- draw multiple spirals from a common centre point */

double spiraldisc (double x0, double y0, double radius, int n, int turns)
{
   int i;
   double delta;
   double theta;
   
   delta = (M_PI * 2.0) / (double)n;
   
   for (i = 0; i < n; i++) {
      theta = (double)i * delta;
      
      spiral (x0, y0, radius, theta, turns);
   }

   return (radius);
}


/* diamondsquare --- draw concentric squares at 45-degree offsets */

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


/* zigzagring --- draw a ring of zigzags */

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


/* radials --- draw simple radial lines */

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


/* ringofpoly --- draw overlapping polygons to form a ring */

double ringofpoly (double x0, double y0, double radius, int nside, int npoly)
{
   int i, j;
   double delta;
   double theta;
   double angperside;
   double intang;
   double x, y;
   
   delta = (M_PI * 2.0) / (double)npoly;
   angperside = (M_PI * 2.0) / (double)nside;

   for (i = 0; i < npoly; i++) {
      theta = (double)i * delta;
      
      x = radius * cos (theta);
      y = radius * sin (theta);
      
      moveto (x0 + x, y0 + y);
      
      for (j = 0; j < nside; j++) {
         intang = (double)(j + 1) * angperside;

         x = radius * cos (theta + intang);
         y = radius * sin (theta + intang);
      
         lineto (x0 + x, y0 + y);
      }
   }
   
   return (radius);
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


/* sqwavering --- draw a cog-like ring of square waves */

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


/* circletosuper --- draw transition from circle to superellipses */

double circletosuper (double x0, double y0, double r1, double r2, int n)
{
   static double pow2[6] = {
      2.8, 4.0, 5.65, 8.0, 11.3, 16.0
   };
   double step;
   double a, b;
   int i;

   step = (r2 - r1) / (double)n;
   
   for (i = 0; i < n; i++) {
      a = r1 + (i * step);
      b = r1 + (i * step);

      superellipse (x0, y0, a, b, 0.0, pow2[i]);
   }
   
   return (r2);
}


double circlering (double x0, double y0, double r1, double r2, int n)
{
   int i;
   double delta;
   double theta;
   double x, y;
   double r;
   
   delta = (M_PI * 2.0) / (double)n;
   
   r = (r2 - r1) / 2.0;

   for (i = 0; i < n; i++) {
      theta = (double)i * delta;
      
      x = r * cos (theta);
      y = r * sin (theta);
      
      circle (x0 + x, y0 + y, r1 + r);
   }
   
   return (r2);
}


/* sqradials --- draw radial lines from inner circle to outer square */

double sqradials (double x0, double y0, double r1, double r2, int n)
{
   double side;
   double r45;
   
   r45 = M_PI / 4.0; /* 45 degrees in radians */
   
   side = 2.0 * r2; /* Length of side of overall square */
   
   radialfan (x0, y0, r1, r45,        r2,  r2, -side,   0.0, n / 4); 
   radialfan (x0, y0, r1, r45 * 3.0, -r2,  r2,   0.0, -side, n / 4);
   radialfan (x0, y0, r1, r45 * 5.0, -r2, -r2, side,    0.0, n / 4);
   radialfan (x0, y0, r1, r45 * 7.0,  r2, -r2,   0.0,  side, n / 4);
   
   return (r2);
}


/* roundrectborder --- draw border as nested rounded rectangles */

void roundrectborder (double x0, double y0, double r1, double r2, int n)
{
   int i;
   double xmin, ymin;
   double xmax, ymax;
   double radius;
   double incr;
   
   incr = (r2 - r1) / (double)n;
   
   /* Innermost rectangle does not have rounded corners */
   rectangle (x0 - r1, y0 - r1, x0 + r1, y0 + r1);
   
   for (i = 1; i < n; i++) {
      radius = incr * (double)i;
      xmin = x0 - r1 - radius;
      ymin = y0 - r1 - radius;
      xmax = x0 + r1 + radius;
      ymax = y0 + r1 + radius;

      roundrect (xmin, ymin, xmax, ymax, radius);
   }
}


/* saltireborder --- draw border squares as saltires */

void saltireborder (double x0, double y0, int nx, int ny, double w, double h)
{
   /* nx and ny must be even, but need not be equal */
   double xmax, ymax;
   int i;
   
   xmax = x0 + (nx * w);
   ymax = y0 + (ny * h);
   
   bordergrid (x0, y0, w, h, nx, ny, 1);
   
   /* Zig-zag across bottom */
   moveto (x0, y0 + (2.0 * h));
   lineto (x0 + (2.0 * w), y0);
   
   for (i = 3; i < (nx - 1); i += 2) {
      lineto (x0 + (i * w), y0 + h);
      lineto (x0 + ((i + 1) * w), y0);
   }

   /* Zig-zag up RHS */
   lineto (xmax, y0 + (2.0 * h));
   
   for (i = 3; i < (ny - 1); i += 2) {
      lineto (xmax - w, y0 + (i * h));
      lineto (xmax, y0 + ((i + 1) * h));
   }
   
   /* Zig-zag across top, R to L */
   lineto (xmax - (2.0 * w), ymax);
   
   for (i = nx - 3; i > 1; i -= 2) {
      lineto (x0 + (i * w), ymax - h);
      lineto (x0 + ((i - 1) * w), ymax);
   }
   
   /* Zig-zag down LHS */
   lineto (x0, ymax - (2.0 * h));
   
   for (i = ny - 3; i > 1; i -= 2) {
      lineto (x0 + w, y0 + (i * h));
      lineto (x0, y0 + ((i - 1) * h));
   }
   
   /* Zig-zag across bottom again, L to R */
   moveto (x0, y0 + h);
   lineto (x0 + w, y0);
   
   for (i = 2; i < nx; i += 2) {
      lineto (x0 + (i * w), y0 + h);
      lineto (x0 + ((i + 1) * w), y0);
   }
   
   /* Zig-zag up RHS */
   lineto (xmax, y0 + h);

   for (i = 2; i < ny; i += 2) {
      lineto (xmax - w, y0 + (i * h));
      lineto (xmax, y0 + ((i + 1) * h));
   }
   
   /* Zig-zag across top, R to L */
   lineto (xmax - w, ymax);
   
   for (i = nx - 2; i > 1; i -= 2) {
      lineto (x0 + (i * w), ymax - h);
      lineto (x0 + ((i - 1) * w), ymax);
   }
   
   /* Zig-zag down LHS */
   lineto (x0, ymax - h);
   
   for (i = ny - 2; i > 1; i -= 2) {
      lineto (x0 + w, y0 + (i * h));
      lineto (x0, y0 + ((i - 1) * h));
   }
}


/* circleborder --- draw border squares as circles */

void circleborder (double x0, double y0, int nx, int ny, double w, double h)
{
   /* w and h must be the same for the circles to fit in the grid */
   double xmax, ymax;
   int i;
   
   xmax = x0 + (nx * w);
   ymax = y0 + (ny * h);
   
   bordergrid (x0, y0, w, h, nx, ny, 0);
   
   for (i = 1; i < nx; i++) {
      circle (x0 + (w / 2.0) + (w * (double)i), h / 2.0, h / 2.0);
   }
   
   for (i = 1; i < ny; i++) {
      circle (xmax - (w / 2.0), y0 + (h / 2.0) + (h * (double)i), h / 2.0);
   }
   
   for (i = nx - 2; i >= 0; i--) {
      circle (x0 + (w / 2.0) + (w * (double)i), ymax - (h / 2.0), h / 2.0);
   }
   
   for (i = ny - 2; i >= 0; i--) {
      circle (x0 + (w / 2.0), y0 + (h / 2.0) + (h * (double)i), h / 2.0);
   }
}


/* celticstepborder --- draw a border grid of Celtic step pattern */

void celticstepborder (double x0, double y0, int nx, int ny, double w, double h)
{
   double xmax, ymax;
   int i;
   
   xmax = x0 + (nx * w);
   ymax = y0 + (ny * h);
   
   bordergrid (x0, y0, w, h, nx, ny, 0);

   for (i = 1; i < nx; i++) {
      drawstep (x0 + (w * (double)i), y0, w, h, i & 1, 0);
   }
   
   for (i = 1; i < ny; i++) {
      drawstep (xmax - w, y0 + (h * (double)i), w, h, 1, i & 1);
   }
   
   for (i = nx - 2; i >= 0; i--) {
      drawstep (x0 + (w * (double)i), ymax - h, w, h, i & 1, 1);
   }
   
   for (i = ny - 2; i >= 0; i--) {
      drawstep (x0, y0 + (h * (double)i), w, h, 0, i & 1); 
   }
}


/* curvestitchtrans --- transition by curve-stiching (pin-and-cotton) */

void curvestitchtrans (double x0, double y0, double r1, double r2, int n)
{
   int i;
   double xmin, xmax;
   double ymin, ymax;
   double xinc, yinc;
   
   xinc = r2 / (double)n;
   yinc = r2 / (double)n;

   /* Draw lower RH corner */
   xmin = x0;
   ymin = y0 - r2;
   xmax = x0 + r2;
   ymax = y0;
   
   for (i = 1; i < n; i++) {
      if (i & 1) {
         moveto (xmin + (xinc * (double)i), ymin);
         lineto (xmax, ymin + (yinc * (double)i));
      }
      else {
         moveto (xmax, ymin + (yinc * (double)i));
         lineto (xmin + (xinc * (double)i), ymin);
      }
   }

   /* Draw upper RH corner */
   xmin = x0;
   ymin = y0;
   xmax = x0 + r2;
   ymax = y0 + r2;
   
   for (i = 1; i < n; i++) {
      if (i & 1) {
         moveto (xmax, ymin + (yinc * (double)i));
         lineto (xmax - (xinc * (double)i), ymax);
      }
      else {
         moveto (xmax - (xinc * (double)i), ymax);
         lineto (xmax, ymin + (yinc * (double)i));
      }
   }

   /* Draw upper LH corner */
   xmin = x0 - r2;
   ymin = y0;
   xmax = x0;
   ymax = y0 + r2;
   
   for (i = 1; i < n; i++) {
      if (i & 1) {
         moveto (xmax - (xinc * (double)i), ymax);
         lineto (xmin, ymax - (yinc * (double)i));
      }
      else {
         moveto (xmin, ymax - (yinc * (double)i));
         lineto (xmax - (xinc * (double)i), ymax);
      }
   }

   /* Draw lower LH corner */
   xmin = x0 - r2;
   ymin = y0 - r2;
   xmax = x0;
   ymax = y0;
   
   for (i = 1; i < n; i++) {
      if (i & 1) {
         moveto (xmin, ymax - (yinc * (double)i));
         lineto (xmin + (xinc * (double)i), ymin);
      }
      else {
         moveto (xmin + (xinc * (double)i), ymin);
         lineto (xmin, ymax - (yinc * (double)i));
      }
   }
}


void radialfan (double x0, double y0, double r1, double startangle, double dx, double dy, double xvec, double yvec, int n)
{
   int i;
   double delta1;
   double delta2;
   double theta;
   double x1, y1; 
   double x2, y2;
   
   delta1 = (M_PI / 2.0) / (double)n;
   delta2 = 1.0 / (double)n;
   
   for (i = 0; i < n; i++) {
      theta = startangle + ((double)i * delta1);
      
      x1 = x0 + (r1 * cos (theta));
      y1 = y0 + (r1 * sin (theta));
      
      x2 = x0 + dx + (xvec * ((double)i * delta2));
      y2 = y0 + dy + (yvec * ((double)i * delta2));
      
      if (i & 1) {
         moveto (x1, y1);
         lineto (x2, y2);
      }
      else {
         moveto (x2, y2);
         lineto (x1, y1);
      }
   }
}


/* bordergrid --- draw grid around page with optional diagonals in corners */

void bordergrid (double x0, double y0, double w, double h, int nx, int ny, int diag)
{
   double xmax, ymax;
   int i;
   
   xmax = x0 + (nx * w);
   ymax = y0 + (ny * h);
   
   /* Draw four lines in the form of #, in optimum sequence */
   moveto (x0 + w, y0);
   lineto (x0 + w, ymax);
   
   moveto (x0, ymax - h);  
   lineto (xmax, ymax - h);
   
   moveto (xmax - w, ymax);
   lineto (xmax - w, y0);
   
   moveto (xmax, y0 + h);
   lineto (x0, y0 + h);
   
   /* Draw remainder of grid, along the bottom */
   for (i = 2; i < (nx - 1); i++) {
      moveto (x0 + (i * w), y0);
      lineto (x0 + (i * w), y0 + h);
   }

   /* Draw lower RH corner diagonal */
   if (diag) {
      moveto (xmax, y0);
      lineto (xmax - w, y0 + h);
   }
   
   /* Up the right-hand side */
   for (i = 2; i < (ny - 1); i++) {
      moveto (xmax, y0 + (i * h));
      lineto (xmax - w, y0 + (i * h));
   }

   /* Draw upper RH corner diagonal */
   if (diag) {
      moveto (xmax, ymax);
      lineto (xmax - w, ymax - h);
   }
   
   /* Along the top, leftwards */
   for (i = nx - 2; i > 1; i--) {
      moveto (x0 + (i * w), ymax);
      lineto (x0 + (i * w), ymax - h);
   }

   /* Draw upper LH corner diagonal */
   if (diag) {
      moveto (x0, ymax);
      lineto (x0 + w, ymax - h);
   }

   /* Left-hand side, downwards */
   for (i = ny - 2; i > 1; i--) {
      moveto (x0, y0 + (i * h));
      lineto (x0 + w, y0 + (i * h));
   }

   /* Draw lower LH corner diagonal */
   if (diag) {
      moveto (x0, y0);
      lineto (x0 + w, y0 + h);
   }
}


/* superellipse --- draw a superellipse */

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


/* spiral --- draw a spiral given angle and number of turns */

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


/* lotus --- draw a central disc in the form of a lotus pattern */

double lotus (double x0, double y0, double radius, int nleaves, int nlayers)
{
   double incr;
   int i;
   
   incr = radius / (double)(nlayers + 1);
   radius = incr;

   for (i = 0; i < nlayers; i++)
      radius = lotuszigzag (x0, y0, radius, radius + incr, nleaves, i & 1);
   
   return (radius);
}


/* lotuszigzag --- draw special zigzag for the lotus pattern disc */

double lotuszigzag (double x0, double y0, double r1, double r2, int npts, int flag)
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
         theta1 = (delta * (double)i) - (delta / 2.0);
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
   n2 = 0;
   
   for (i = 0; i < npts; i++) {
      lineto (x1[n1], y1[n1]);
      lineto (x2[n2], y2[n2]);
      
      n1 = (n1 + 1) % npts;
      n2 = (n2 + 1) % npts;
   }
   
   lineto (x1[0], y1[0]);
   
   return (r2);
}


/* roundrect --- draw a rounded rectangle */

void roundrect (double x1, double y1, double x2, double y2, double radius)
{
   moveto (x1 + radius, y1);
   lineto (x2 - radius, y1);
   
   arc (x2 - radius, y1 + radius, 90.0);
   
// moveto (x2, y1 + radius);
   lineto (x2, y2 - radius);
   
   arc (x2 - radius, y2 - radius, 90.0);
   
// moveto (x2 - radius, y2);
   lineto (x1 + radius, y2);
   
   arc (x1 + radius, y2 - radius, 90.0);
   
// moveto (x1, y2 - radius);
   lineto (x1, y1 + radius);
   
   arc (x1 + radius, y1 + radius, 90.0);
}


/* drawstep --- draw a single block of Celtic step pattern */

void drawstep (double x0, double y0, double w, double h, int flipx, int flipy)
{
/* Celtic Design: A Beginner's Manual, Aidan Meehan, 1991
   ISBN: 0-500-27629-3, Chapter 1 */
   struct coord {
      double x, y;
   };
   struct Grid {
      double x, y;
      struct coord ne, se, nw, sw;
   };
   double xoff, yoff;
   struct Grid grid[4][4];
   int x, y;
   int i, j;
   
   xoff = w / 20.0;
   yoff = h / 20.0;
   
   if (flipx)
      xoff = -xoff;
   
   if (flipy)
      yoff = -yoff;
   
   for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++) {
         if (flipx)
            x = 3 - j;
         else
            x = j;
            
         if (flipy)
            y = 3 - i;
         else
            y = i;
            
         grid[y][x].x = x0 + (j * (w / 3.0));
         grid[y][x].y = y0 + (i * (h / 3.0));
         grid[y][x].ne.x = grid[y][x].se.x = grid[y][x].x + xoff;
         grid[y][x].nw.x = grid[y][x].sw.x = grid[y][x].x - xoff;
         grid[y][x].ne.y = grid[y][x].nw.y = grid[y][x].y + yoff;
         grid[y][x].se.y = grid[y][x].sw.y = grid[y][x].y - yoff;
      }
   
#ifdef DB
   for (y = 0; y < 3; y++)
      for (x = 0; x < 3; x++) {
         moveto (grid[y][x].ne.x, grid[y][x].ne.y);
         lineto (grid[y+1][x+1].sw.x, grid[y+1][x+1].sw.y);
         moveto (grid[y][x+1].nw.x, grid[y][x+1].nw.y);
         lineto (grid[y+1][x].se.x, grid[y+1][x].se.y);
      }
#endif
      
   /* Box */
   moveto (grid[1][0].x, grid[1][0].y);
   lineto (grid[1][1].x, grid[1][1].y);
   lineto (grid[0][1].x, grid[0][1].y);
   
   /* Zig-zag */
   moveto (grid[1][3].x, grid[1][3].y);
   lineto (grid[1][2].x, grid[1][2].y);
   lineto (grid[2][2].x, grid[2][2].y);
   lineto (grid[2][1].x, grid[2][1].y);
   lineto (grid[3][1].x, grid[3][1].y);
   
   /* Inner box */
   rectangle (grid[0][0].ne.x, grid[0][0].ne.y, grid[1][1].sw.x, grid[1][1].sw.y);
   
   /* Inner zig-zag */
   moveto (grid[0][1].ne.x, grid[0][1].ne.y);
   lineto (grid[0][3].nw.x, grid[0][3].nw.y);
   lineto (grid[1][3].sw.x, grid[1][3].sw.y);
   lineto (grid[1][2].sw.x, grid[1][2].sw.y);
   lineto (grid[2][2].sw.x, grid[2][2].sw.y);
   lineto (grid[2][1].sw.x, grid[2][1].sw.y);
   lineto (grid[3][1].sw.x, grid[3][1].sw.y);
   lineto (grid[3][0].se.x, grid[3][0].se.y);
   lineto (grid[1][0].ne.x, grid[1][0].ne.y);
   lineto (grid[1][1].ne.x, grid[1][1].ne.y);
   lineto (grid[0][1].ne.x, grid[0][1].ne.y);
   
   /* Inner L */
   moveto (grid[1][2].ne.x, grid[1][2].ne.y);
   lineto (grid[1][3].nw.x, grid[1][3].nw.y);
   lineto (grid[3][3].sw.x, grid[3][3].sw.y);
   lineto (grid[3][1].se.x, grid[3][1].se.y);
   lineto (grid[2][1].ne.x, grid[2][1].ne.y);
   lineto (grid[2][2].ne.x, grid[2][2].ne.y);
   lineto (grid[1][2].ne.x, grid[1][2].ne.y);
}
