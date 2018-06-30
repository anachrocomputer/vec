/* allover12 --- draw all-over pattern, page 12             2013-06-23 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

/* Geometric Allover Patterns, by Ian O. Angell
 * Dover Publications, Inc., 1985, ISBN 0-486-24855-0
 * Page 12
 */

void drawx(const int nx, const int ny, const double r1, const double r2, const double offset, const double angle);
void drawy(const int nx, const int ny, const double r1, const double r2, const double offset, const double angle);


int main(int argc, char * const argv[])
{
   int opt;
   int nx, ny;
   double scale;
   double r1, r2;
   double d2;
   double maxx, maxy;
   
   while ((opt = getopt (argc, argv, "no:p:s:t:v:")) != -1) {
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
         exit(EXIT_FAILURE);
      }
   }

   /* Basic drawing parameters */
   nx = 8;
   ny = 5;
   scale = 40.0;
   r1 = 30.0 * scale;
   r2 = 7.5 * scale;
   d2 = 2.0 * r2;
   
   plotbegin(1);

   getplotsize(&maxx, &maxy);
   
   drawx(nx, ny, r1, r2, 0.0, 180.0);
   drawx(nx, ny, r1, r2, -d2, -180.0);
   drawy(nx, ny, r1, r2, -d2, -180.0);
   drawy(nx, ny, r1, r2, 0.0, 180.0);
   
   plotend();
   
   return (0);
}


void drawx(const int nx, const int ny, const double r1, const double r2, const double offset, const double angle)
{
   int ix, iy;
   double x1, x2;
   double y;
   const double d1 = r1 * 2.0;
   const double pitch = (r1 * 2.0) - (r2 * 2.0);
   
   for (iy = 0; iy < ny; iy++) {
      x1 = offset;
      y = (double)(iy + 1) * pitch;
      moveto(x1, y);
      lineto(x1, y);

      for (ix = 0; ix < nx; ix++) {
         x1 = ((double)ix * pitch) + offset;
         
         x2 = x1 + r1;
         arc(x2, y, angle);
         
         x2 = x1 + d1 - r2;
         arc(x2, y, angle);
      }
   }
}


void drawy(const int nx, const int ny, const double r1, const double r2, const double offset, const double angle)
{
   int ix, iy;
   double x;
   double y1, y2;
   const double d1 = r1 * 2.0;
   const double pitch = (r1 * 2.0) - (r2 * 2.0);
   
   for (ix = 0; ix < nx; ix++) {
      x = (double)(ix + 1) * pitch;
      y1 = offset;
      moveto(x, y1);
      lineto(x, y1);

      for (iy = 0; iy < ny; iy++) {
         y1 = ((double)iy * pitch) + offset;
         
         y2 = y1 + r1;
         arc(x, y2, angle);
         
         y2 = y1 + d1 - r2;
         arc(x, y2, angle);
      }
   }
}
