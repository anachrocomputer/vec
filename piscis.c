/* piscis --- plot the "fish bladder", vesica piscis        2011-10-02 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double r1;  /* Radius of smaller arcs */
   double r2;  /* Radius of larger arc */
   
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
   
   r1 = 100.0;
   r2 = 120.0;

   /* Two overlapping circles */
   circle(xc - ((r1 / 2.0) * 40.0), yc, r1 * 40.0);
   circle(xc + ((r1 / 2.0) * 40.0), yc, r1 * 40.0);
   
   /* Round-ended oval around picis */
   moveto(xc - ((r1 / 2.0) * 40.0), yc - (r2 * 40.0));
   lineto(xc + ((r1 / 2.0) * 40.0), yc - (r2 * 40.0));
   arc(xc + ((r1 / 2.0) * 40.0), yc, 180.0);
   lineto(xc - ((r1 / 2.0) * 40.0), yc + (r2 * 40.0));
   arc(xc - ((r1 / 2.0) * 40.0), yc, 180.0);

   plotend();
   
   return (0);
}
