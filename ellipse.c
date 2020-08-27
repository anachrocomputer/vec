/* ellipse --- plot four patterns based on ellipses         2011-10-08 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

void plot_ll(const double xc, const double yc, const double r1, const double r2);
void plot_lr(const double xc, const double yc, const double r1, const double r2);
void plot_ul(const double xc, const double yc, const double r1, const double r2);
void plot_ur(const double xc, const double yc, const double r1, const double r2);


int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double w4, h4;
   double r1, r2;
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
   
   h4 = maxy / 4.0;
   w4 = maxx / 4.0;
   
   r1 = maxx / 5.0;
   r2 = maxy / 5.0;
   
   /* Split page into quarters */
   moveto(0.0, yc);
   lineto(maxx, yc);
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   /* Draw four ellipse plots */
   plot_ll(w4, h4, r1, r2);
   plot_lr(xc + w4, h4, r1, r2);
   plot_ul(w4, yc + h4, r1, r2);
   plot_ur(xc + w4, yc + h4, r1, r2);
   
   plotend();
   
   return (0);
}


void plot_ll(const double xc, const double yc, const double r1, const double r2)
{
   circle(xc, yc, r2);
   ellipse(xc, yc, r1, r2, 0.0);
   ellipse(xc, yc, r1 / 3.0, r2, 0.0);
}


void plot_lr(const double xc, const double yc, const double r1, const double r2)
{
   /* Inspired by "Japanese Optical and Geometrical Art" by
      Hajime Ouchi, ISBN 0-486-23553-X, page 23, bottom */
   int i;
   const double delta = (2.0 * M_PI) / 8.0;
   const double radius = r2 / 2.0;
   
   for (i = 0; i < 8; i++) {
      const double theta = delta * (double)i;
      
      const double x = xc + (radius * cos(theta));
      const double y = yc + (radius * sin(theta));
      
      ellipse(x, y, r1 / 2.0, r2 / 2.0, theta);
   }
}


void plot_ul(const double xc, const double yc, const double r1, const double r2)
{
   const double r3 = r1 / 2.0;
   const double r4 = r2 / 2.0;
   
   ellipse(xc, yc, r3 * sqrt(2.0), r4 * sqrt(2.0), 0.0);
   ellipse(xc, yc, r1, r2, 0.0);
   ellipse(xc - r3, yc, r3, r4, 0.0);
   ellipse(xc, yc + r4, r3, r4, 0.0);
   ellipse(xc, yc - r4, r3, r4, 0.0);
   ellipse(xc + r3, yc, r3, r4, 0.0);
}


void plot_ur(const double xc, const double yc, const double r1, const double r2)
{
   circle(xc, yc, r2);
#if 0
   int i;

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
#endif
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
