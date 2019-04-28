/* coords --- plot an HPGL coordinate chart                 2010-09-07 */
/* Copyright (c) 2010 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hpgllib.h"


int main(int argc, char * const argv[])
{
   int opt;
   int i;
   char str[32];
   int ix, iy;
   double xc, yc;
   double radius;
   double small, big;
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
//       exit(EXIT_FAILURE);
      }
   }
   
   plotbegin(1);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   radius = maxy - yc;

   /* Plot centrelines */
   moveto(0.0, yc);
   lineto(maxx, yc);
   
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   /* Ticks on X-axis */
   small = 800.0;

   big = ((int)(maxx / 800.0)) * 800.0;
   
   for (i = small; i <= big; i += 800) {
      moveto((double)i, 0.0);
      lineto((double)i, 200.0);
      ix = getdevx((double)i);
      sprintf(str, "%d %dmm", ix, i / 40);
      vlabel((double)(i + 100), 300.0, 5.0, str);
   }
   
   /* Ticks on Y-axis */
   small = 800.0;

   big = ((int)(maxy / 800.0)) * 800.0;
   
   for (i = small; i <= big; i += 800) {
      moveto(0.0, (double)i);  
      lineto(200.0, (double)i);
      iy = getdevy((double)i);
      sprintf(str, "%d %dmm", iy, i / 40);
      hlabel(300.0, (double)(i - 100), 5.0, str);
   }
   
   /* Plot main circles */
   circle(xc, yc, radius);
   circle(xc, yc, radius / 2.0);
   
   plotend();
   
   return (0);
}
