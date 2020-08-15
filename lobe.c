/* lobe --- plot circular lobes on the plotter              1997-04-27 */
/* Copyright (c) 1997 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

#define RADIANS  (M_PI / 180.0)


void drawlobes(const double x0, const double y0, const double a, const double b, const double l);


int main(int argc, char *const argv[])
{
   int opt;
   double a, b;
   double maxx, maxy;
   double xc, yc;
   double w4, h4;
   int i;
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
      case 'n':
      case 'o':
      case 'p':
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

   if (plotbegin(1) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   w4 = maxx / 4.0;
   h4 = maxy / 4.0;
   
   moveto(0.0, yc);
   lineto(maxx, yc);
   
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   a = maxy / 4.0;
   b = a / 3.0;
   
   for (i = 0; i < 9; i++) {
      pencolr(i % 4);
      drawlobes(xc, yc, a, b, (double)i);
   }

   plotend();
      
   return (0);
}


void drawlobes(const double x0, const double y0, const double a, const double b, const double l)
{
   int i;

   moveto(x0 + a + b, y0);
   
   for (i = 1; i <= 360; i++) {
      const double theta = i * RADIANS;
      const double r = a + (b * cos(l * theta));
      
      const double x = r * cos(theta);
      const double y = r * sin(theta);
      
      lineto(x0 + x, y0 + y);
   }
}
