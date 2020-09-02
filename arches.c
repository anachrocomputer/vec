/* arches --- plot various kinds of arches                  2011-10-01 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

void plot_ur(const double x0, const double y0, const double width, const double height, const double r1, const double r2);
void circulararch(const double xc, const double y0, const double yc, const double r);
void plot_ll(const double x0, const double y0, const double width, const double height, const double r1, const double r2);
void ellipticalarch(const double xc, const double y0, const double yc, const double a, const double b);
void plot_ul(const double x0, const double y0, const double width, const double height, const double r1, const double r2);
void threecentredarch(const double xc, const double y0, const double yc, const double d, const double r);
void half_ellipse(const double x0, const double y0, const double a, const double b, const double theta);
void plot_lr(const double x0, const double y0, const double width, const double height, const double r1, const double r2);


int main(int argc, char * const argv[])
{
   int opt;
   double maxx, maxy;
   double xc, yc;
   double r1, r2;
   
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
   if (plotbegin(1) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   r1 = maxx / 5.0;
   r2 = maxy / 5.0;
   
   /* Split page into quarters */
   moveto(0.0, yc);
   lineto(maxx, yc);
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   /* Draw four arch plots */
   plot_ll(0.0, 0.0, xc, yc, r1, r2);
   plot_lr(xc,  0.0, xc, yc, r1, r2);
   plot_ul(0.0, yc,  xc, yc, r1, r2);
   plot_ur(xc,  yc,  xc, yc, r1, r2);
   
   plotend();
   
   return (0);
}


void plot_ur(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);

   /* Centre lines */
   moveto(x0, yc);
   lineto(x0 + width, yc);
   moveto(xc, y0);
   lineto(xc, y0 + height);
   
   /* Circle forming arch, drawn as full circle */
   circle(xc, yc, r2);

   /* Thicker pen for outline of arch */
// pencolr(1);
// printf("VS5;\n");

   circulararch(xc, y0, yc, r2);
   circulararch(xc, y0, yc, r2 * 1.15);
}


void circulararch(const double xc, const double y0, const double yc, const double r)
{
   moveto(xc - r, y0);
   lineto(xc - r, yc);

   /* The arch itself, a half circle */
   arc(xc, yc, -180.0);
   
   lineto(xc + r, y0);
}


void plot_ul(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   /* r3: Distance from central axis to centre of smaller arcs */
   /* r4: Radius of smaller arcs */
   const double r3 = r1 / 2.0;
   const double r4 = r2 / 2.0;
   const double r5 = sqrt((r3 * r3) + (r3 * r3)) + r4; /* Radius of larger arc */
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   
   /* Centre lines */
   moveto(x0, yc);
   lineto(x0 + width, yc);
   moveto(xc, y0);
   lineto(xc, y0 + height);

   /* LH centre mark */
   moveto(xc - r3, yc + (3.0 * 40.0));
   lineto(xc - r3, yc - (3.0 * 40.0));
   
   /* LH small circle */
   circle(xc - r3, yc, r4);

   /* RH centre mark */
   moveto(xc + r3, yc + (3.0 * 40.0));
   lineto(xc + r3, yc - (3.0 * 40.0));

   /* RH small circle */
   circle(xc + r3, yc, r4);

   /* 45 degree construction lines */
   moveto(xc - (2.0 * r3), yc + r3);
   lineto(xc, yc - r3);
   lineto(xc + (2.0 * r3), yc + r3);

   /* Bottom centre mark */
   moveto(xc - (3.0 * 40.0), yc - r3);
   lineto(xc + (3.0 * 40.0), yc - r3);

   /* Upper large circle, only drawn as half-circle */
   moveto(xc + r5, yc - r3);
   arc(xc, yc - r3, 180.0);
   
   /* Thicker pen for outline of arch */
// pencolr(1);
// printf("VS5;\n");

   threecentredarch(xc, y0, yc, r3, r4);
   threecentredarch(xc, y0, yc, r3, r4 * 1.3);
}


void threecentredarch(const double xc, const double y0, const double yc, const double d, const double r)
{
   moveto(xc - (d + r), y0);
   lineto(xc - (d + r), yc);

   arc(xc - d, yc, -45.0);
   arc(xc, yc - d, -90.0);
   arc(xc + d, yc, -45.0);

   lineto(xc + (d + r), y0);
}


void plot_ll(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   const double thickness = r1 / 10.0;

   /* Centre lines */
   moveto(x0, yc);
   lineto(x0 + width, yc);
   moveto(xc, y0);
   lineto(xc, y0 + height);
   
   /* Ellipse forming arch, drawn in full */
   ellipse(xc, yc, r1, r2, 0.0);

   /* Thicker pen for outline of arch */
// pencolr(1);
// printf("VS5;\n");

   ellipticalarch(xc, y0, yc, r1, r2);
   ellipticalarch(xc, y0, yc, r1 + thickness, r2 + thickness);
}


void ellipticalarch(const double xc, const double y0, const double yc, const double a, const double b)
{
   moveto(xc + a, y0);
   lineto(xc + a, yc);

   /* The arch itself, a half ellipse */
   half_ellipse(xc, yc, a, b, 0.0);
   
   lineto(xc - a, y0);
}


void half_ellipse(const double x0, const double y0, const double a, const double b, const double theta)
{
   const int npts = 36;
   const double delta = M_PI / (double)npts;
   const double sintheta = sin(theta);
   const double costheta = cos(theta);
   int i;
   
   for (i = 0; i <= npts; i++) {
      const double t = (double)i * delta;
      
      const double x = (a * cos(t) * costheta) - (b * sin(t) * sintheta);
      const double y = (a * cos(t) * sintheta) + (b * sin(t) * costheta);
      
      if (i == 0)
         moveto(x0 + x, y0 + y);
      else
         lineto(x0 + x, y0 + y);
   }
}


void plot_lr(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
}
