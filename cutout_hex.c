/* cutout_hex --- plot hexagons that can be cut out easily  2020-08-26 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

#define RADIANS  (M_PI / 180.0)

void hexgrid(const double side, const int nx, const int ny);

int main(int argc, char * const argv[])
{
   int opt;
   double side;
   double maxx, maxy;
   
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
   
// side = 50.08 * 40.0;
// hexgrid(side, 4, 2);
   side = 45.0 * 40.0;
   hexgrid(side, 5, 3);

   plotend();
   
   return (0);
}

   
void hexgrid(const double side, const int nx, const int ny)
{
   int i, j;
   const double width = 2.0 * side * cos(30.0 * RADIANS);
   
   for (i = 0; i < ny; i++) {
      const double y = 2.0 * side * (double)i;
      const double y1 = y + (side * sin(30.0 * RADIANS));
#ifdef TRISECT
      const double y2 = y + side;
#endif
      const double y3 = y1 + side;
      const double y4 = y + (2.0 * side);
      
      for (j = 0; j < nx; j++) {
         const double x = width * (double)j;
         const double x1 = x + (side * cos(30 * RADIANS));
         const double x2 = x + width;
         
         moveto(x, y1);
         lineto(x1, y);
         lineto(x2, y1);
      }
   
      for (j = 0; j < (nx + 1); j++) {
         const double x = width * (double)j;
         
         moveto(x, y1);
         lineto(x, y3);
      }

      for (j = 0; j < nx; j++) {
         const double x = width * (double)j;
         const double x1 = x + (side * cos(30 * RADIANS));
         const double x2 = x + width;
         
         moveto(x, y3);
         lineto(x1, y4);
         lineto(x2, y3);
      }

#ifdef TRISECT
      for (j = 0; j < nx; j++) {
         const double x = width * (double)j;
         const double x1 = x + (side * cos(30 * RADIANS));
         const double x2 = x + width;
         
         moveto(x, y1);
         lineto(x1, y2);
         lineto(x2, y1);
      }

      for (j = 0; j < nx; j++) {
         const double x = width * (double)j;
         const double x1 = x + (side * cos(30 * RADIANS));
         
         moveto(x1, y2);
         lineto(x1, y4);
      }
#endif
   }
}
