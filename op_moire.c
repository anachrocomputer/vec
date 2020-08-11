/* op_moire --- draw moire op-art pattern                   2014-06-01 */
/* Copyright (c) 2014 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "hpgllib.h"

void draw_diagonals(int nlines, double pitch, double xl, double xr,
                    double yoff1, double dyoff1, double yoff2, double dyoff2);


int main(int argc, char * const argv[])
{
   /* Optical and Geometrical Allover Patterns, by Jean Larcher,
      1979, ISBN: 0-486-23758-3 */
   int opt;
   int nlines;
   double xc;
   double scale = 40.0;
   double maxx, maxy;
   double xl, xr;
   double height;
   const double yoff = 10.0 * scale;
   const double pitch = 2.0 * scale;
   double dyoff;
   
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

   if (plotbegin(0) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   
   height = maxy;
   
   xl = xc - height / 2.0;
   xr = xc + height / 2.0;

   nlines = ((height - (yoff + (5.0 * scale))) / pitch) + 1;
   dyoff = (5.0 * scale) / (double)nlines;
   
   /* Draw square border */
   rectangle(xc - (height / 2.0), 0.0, xc + (height / 2.0), maxy);
   
   draw_diagonals(nlines, pitch, xl, xr, yoff, dyoff, 0.0, 0.0);
   draw_diagonals(nlines, pitch, xl, xr, 0.0, 0.0, yoff, dyoff);
   
   plotend();
   
   return (0);
}


void draw_diagonals(int nlines, double pitch, double xl, double xr,
                    double yoff1, double dyoff1, double yoff2, double dyoff2)
{
   int i;
   double y;
   
   /* Draw array of sloped lines */
   for (y = 0.0, i = 0; i < nlines; y += pitch, i++) {
      if (i & 1) {
         moveto(xl, y + yoff1);
         lineto(xr, y + yoff2);
      }
      else {
         moveto(xr, y + yoff2);
         lineto(xl, y + yoff1);
      }

      yoff1 += dyoff1;
      yoff2 += dyoff2;
   }
}
