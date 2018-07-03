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
   double maxx, maxy;
   const double scale = 40.0; /* Plotter units per mm */
   
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
         fprintf(stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n",
                  argv[0]);
         fprintf(stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
//       exit(EXIT_FAILURE);
      }
   }

   /* Select first pen and draw border */
   plotbegin(1);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   /* Draw axes */
   moveto(0.0, yc);
   lineto(maxx, yc);
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   /* Draw a simple Lissajous curve */
   lissajous(xc, yc, 150.0 * scale, 5.0, 6.0, 0.0, 144);
   
   plotend();
   
   return (0);
}


void lissajous(const double x0, const double y0, const double side, const double f1, const double f2, const double theta, const int npts)
{
   double t;
   double t1, t2;
   const double delta = (2.0 * M_PI) / (double)npts;
   const double sintheta = sin(theta);
   const double costheta = cos(theta);
   double x, y;
   const double r = side / 2.0;
   int i;
   
   for (i = 0; i <= npts; i++) {
      t = (double)i * delta;
      
      t1 = t * f1;
      t2 = t * f2;
      
      x = (r * cos(t1) * costheta) - (r * sin(t2) * sintheta);
      y = (r * cos(t1) * sintheta) + (r * sin(t2) * costheta);
      
      if (i == 0)
         moveto(x0 + x, y0 + y);
      else
         lineto(x0 + x, y0 + y);
   }
}
