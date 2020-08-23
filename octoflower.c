/* octoflower --- plot an eight-petal flower                2020-08-23 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

void draw_petal(const double xc, const double yc, const double delta, const double theta, const double radius);

int main(int argc, char * const argv[])
{
   int opt;
   int i;
   double xc, yc;
   double maxx, maxy;
   double delta;
   int npetals = 8;
   double radius;
   
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
   
   delta = (2.0 * M_PI) / (double)npetals;
   radius = (maxy / 3.0);

   for (i = 0; i < npetals; i++) {
      const double theta = (double)i * delta;

      const double x = xc + (radius * cos(theta));
      const double y = yc + (radius * sin(theta));
      
      moveto(xc, yc);
      lineto(x, y);
   }
   
   for (i = 0; i < npetals; i++) {
      const double theta = delta * (double)i;
      
      draw_petal(xc, yc, delta, theta, radius);
   }
   
   plotend();
   
   return (0);
}


void draw_petal(const double xc, const double yc, const double delta, const double theta, const double radius)
{
   int i;
   const double tipx = xc + (radius * cos(theta));
   const double tipy = yc + (radius * sin(theta));
   
   for (i = 1; i <= 6; i++) {
      const double len = 4.0 * 40.0 * (double)i;
      const double offx = len * sin(theta);
      const double offy = -len * cos(theta);
      const double arcx = tipx + offx;
      const double arcy = tipy + offy;

      const double baslen = len / tan(delta / 2.0);
      const double basx = baslen * cos(theta);
      const double basy = baslen * sin(theta);
      
      moveto(xc + basx + offx, yc + basy + offy);
      lineto(arcx, arcy);
      arc(tipx, tipy, 180.0);
      lineto(xc + basx - offx, yc + basy - offy);
   }
}
