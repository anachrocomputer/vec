/* superellipse --- plot a superellipse                     2011-10-17 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "hpgllib.h"


void plot_ll(const double xc, const double yc, const double r1, const double r2);
void plot_lr(const double xc, const double yc, const double r1, const double r2);
void plot_ul(const double xc, const double yc, const double r1, const double r2);
void plot_ur(const double xc, const double yc, const double r1, const double r2);
void superellipse(const double x0, const double y0, const double a, const double b, const double theta, const double d);


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
   
   /* Draw four superellipse plots */
   plot_ll(w4, h4, r1, r2);
   plot_lr(xc + w4, h4, r1, r2);
   plot_ul(w4, yc + h4, r1, r2);
   plot_ur(xc + w4, yc + h4, r1, r2);

   plotend();
   
   return (0);
}


void plot_ll(const double xc, const double yc, const double r1, const double r2)
{
   double a, b;
   /* Table of powers of sqrt(2.0), could be calculated; like f-stops */
   static double sq[17] = {
   /*  -4    -3     -2     -1    0     1 */
      0.25, 0.354, 0.500, 0.707, 1.0,  1.4,
   /*   2    3    4    5    6    7 */
       2.0,   2.8,   4.0,   5.6, 8.0, 11.0,
   /*   8    9      10      11    12 */
      16.0,  22.6,  32.0,  45.0, 64.0
   };
   double theta = 0.0;
#if 0
   double delta;
   double twroot2;   /* 12th root of 2.0 */
#endif
   int i;
   
   /* Draw bunch of nested superellipses */
   for (i = 6; i < 13; i++) {
      /* if a==b, we'll get squares and circles */
      a = (20.0 + (4.0 * i)) * 40.0;
      b = (20.0 + (2.5 * i)) * 40.0;
      superellipse(xc, yc, a, b, theta, sq[i]);
   }
   
   a = (20.0 + (4.0 * i)) * 40.0;
   b = (20.0 + (2.5 * i)) * 40.0;
   rectangle(xc - a, yc - b, xc + a, yc + b);

#if 0
   delta = (M_PI * 2.0) / 5.0;
   
   for (i = 0; i < 5; i++) {
      a = 87.0 * 40.0;
      b = 87.0 * 40.0;
      theta = delta * (double)i;
      superellipse(xc, yc, a, b, theta, 2.8);
   }

   twroot2 = pow(2.0, 1.0 / 12.0);
   theta = M_PI / 4.0;

   for (i = 0; i <= 12; i++) {
      /* if a==b, we'll get squares and circles */
      a = (100.0 + (6.0 * i)) * 40.0; 
      b = (100.0 + (6.0 * i)) * 40.0;
      superellipse(X0, Y0, a, b, theta, pow(twroot2, (double)(12 - i)));
   }
#endif
}


void plot_lr(const double xc, const double yc, const double r1, const double r2)
{
   int i;
   const double dr1 = r1 / 10.0;
   const double dr2 = r2 / 10.0;
   
   for (i = 0; i < 5; i++) {
      const double a = (r1 / 2.0) + (dr1 * (double)i);
      const double b = (r2 / 2.0) + (dr2 * (double)i);
      const double theta = 0.0;
      
      superellipse(xc, yc, a, b, theta, 4.0);
   }
}


void plot_ul(const double xc, const double yc, const double r1, const double r2)
{
   const double r3 = r1 / 2.0;
   const double r4 = r2 / 2.0;
// const double scale = sqrt(2.0);
   const double scale = 1.19;
   
   superellipse(xc, yc, r3 * scale, r4 * scale, 0.0, 4.0);
   superellipse(xc, yc, r1, r2, 0.0, 4.0);
   superellipse(xc - r3, yc, r3, r4, 0.0, 4.0);
   superellipse(xc, yc + r4, r3, r4, 0.0, 4.0);
   superellipse(xc, yc - r4, r3, r4, 0.0, 4.0);
   superellipse(xc + r3, yc, r3, r4, 0.0, 4.0);
}


void plot_ur(const double xc, const double yc, const double r1, const double r2)
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
      
      superellipse(x, y, r1 / 2.0, r2 / 2.0, theta, 2.8);
   }
}


/* superellipse --- draw a superellipse at (x0, y0) */

void superellipse(const double x0, const double y0, const double a, const double b, const double theta, const double d)
{
   const int npts = 72;
   const double delta = (2.0 * M_PI) / (double)npts;
   const double sintheta = sin(theta);
   const double costheta = cos(theta);
   int i;

   for (i = 0; i <= npts; i++) {
      double sinpt, cospt;
      const double t = (double)i * delta;

      const double st = sin(t);
      const double ct = cos(t);

      if (st < 0.0)
         sinpt = -pow(-st, 2.0 / d);
      else
         sinpt = pow(st, 2.0 / d);

      if (ct < 0.0)
         cospt = -pow(-ct, 2.0 / d);
      else
         cospt = pow(ct, 2.0 / d);

      const double x = (a * cospt * costheta) - (b * sinpt * sintheta);
      const double y = (a * cospt * sintheta) + (b * sinpt * costheta);

      if (i == 0)
         moveto(x0 + x, y0 + y);
      else
         lineto(x0 + x, y0 + y);
   }
}
