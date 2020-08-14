/* lissajous --- plot a Lissajous curve in HPGL             2011-10-19 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

void lissajous(const double x0, const double y0, const double side, const double f1, const double f2, const double theta, const int npts);


int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double h4, w4;
   double maxx, maxy;
   double side;
   
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
   
   side = maxy / 3.0;
   
   /* Split page into quarters */
   moveto(0.0, yc);
   lineto(maxx, yc);
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   /* Draw four simple Lissajous curves */
   lissajous(w4, h4, side, 1.0, 3.0, 0.0, 144);
   lissajous(xc + w4, h4, side, 5.0, 6.0, 0.0, 144);
   lissajous(w4, yc + h4, side, 3.0, 5.0, 0.0, 144);
   lissajous(xc + w4, yc + h4, side, 7.0, 9.0, 0.0, 144);
   
   plotend();
   
   return (0);
}


void lissajous(const double x0, const double y0, const double side, const double f1, const double f2, const double theta, const int npts)
{
   const double delta = (2.0 * M_PI) / (double)npts;
   const double sintheta = sin(theta);
   const double costheta = cos(theta);
   const double r = side / 2.0;
   int i;
   
   for (i = 0; i <= npts; i++) {
      const double t = (double)i * delta;
      
      const double t1 = t * f1;
      const double t2 = t * f2;
      
      const double x = (r * cos(t1) * costheta) - (r * sin(t2) * sintheta);
      const double y = (r * cos(t1) * sintheta) + (r * sin(t2) * costheta);
      
      if (i == 0)
         moveto(x0 + x, y0 + y);
      else
         lineto(x0 + x, y0 + y);
   }
}
